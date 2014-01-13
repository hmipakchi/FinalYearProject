#include "stdafx.h"
#include "test_cascade_inference.h"

void TGraphAlgo::TSpectralClusteringAlg::generateGroundTruthUndirectedTestAdjacencyMatrix() {
    printf("\n#####      generating GroundTruthUndirectedTestAdjacencyMatrix...\n");
    // create adjacency matrix
    const int noNodes = groundTruthUndirectedTestGraph->GetNodes();
    
    TFltVV groundTruthUndirectedTestAdjacencyMatrix(noNodes, noNodes);
    TIntH NodeIdH;
    for (TUNGraph::TNodeI NI = groundTruthUndirectedTestGraph->BegNI(); NI < groundTruthUndirectedTestGraph->EndNI(); NI++) {
        NodeIdH.AddKey(NI.GetId());
    }
    for (TUNGraph::TNodeI NI = groundTruthUndirectedTestGraph->BegNI(); NI < groundTruthUndirectedTestGraph->EndNI(); NI++) {
        const int NodeId = NodeIdH.GetKeyId(NI.GetId());
        for (int e = 0; e < NI.GetOutDeg(); e++) {
            const int DstNId = NodeIdH.GetKeyId(NI.GetOutNId(e));
            // no self edges
            if (NodeId != DstNId) {
                groundTruthUndirectedTestAdjacencyMatrix.At(NodeId, DstNId) = 1;
            }
        }
    }
    
    this->groundTruthUndirectedTestAdjacencyMatrix = groundTruthUndirectedTestAdjacencyMatrix;
}

void TGraphAlgo::TSpectralClusteringAlg::initialiseLaplacianMatrix(const PUNGraph& graph, const int& d_regular) {
    printf("\n#####      initiliasing LaplacianMatrix...\n");
    
    int noRows = groundTruthUndirectedTestAdjacencyMatrix.GetRows();
    int noCols = groundTruthUndirectedTestAdjacencyMatrix.GetCols();
    
    printf("#####     adj mtx no rows: %d, adj mtx no cols : %d\n", noRows, noCols);
    
    TFltVV laplacianMatrix(noRows, noCols);
    TIntH NodeIdH;
    for (TUNGraph::TNodeI NI = graph->BegNI(); NI < graph->EndNI(); NI++) {
        NodeIdH.AddKey(NI.GetId());
    }
    for (int row = 0; row < noRows; row++) {
        for (int col = 0; col < noCols; col++) {
            if (row == col) {
                const int NodeId = NodeIdH.GetKeyId(row);
//                laplacianMatrix.At(row, col) = graph->GetNI(NodeId).GetOutDeg() - groundTruthUndirectedTestAdjacencyMatrix.At(row, col);
                laplacianMatrix.At(row, col) = 1 - (float) (groundTruthUndirectedTestAdjacencyMatrix.At(row, col) / (float) d_regular);
            }
            else {
//                laplacianMatrix.At(row, col) = -1 * groundTruthUndirectedTestAdjacencyMatrix.At(row, col);
                laplacianMatrix.At(row, col) = 0 - (float) (groundTruthUndirectedTestAdjacencyMatrix.At(row, col) / (float) d_regular);
            }
        }
    }
    
    this->laplacianMatrix = laplacianMatrix;
}

void TGraphAlgo::TSpectralClusteringAlg::calculateLaplacianEigenvectors() {
    printf("\n#####      calculating LaplacianEigenvectors...\n");
    
    TSnap::GetEigVec(groundTruthUndirectedTestGraph, noColours/*graph->GetNodes()*/, laplacianEigenvalues, laplacianEigenvectors);
    
    int noEigenvectors = laplacianEigenvectors.Len();
    int noElementsPerEigenvector = laplacianEigenvectors[0].Len();
    
    printf("#####     laplacianEigenvectors noEigenvectors: %d, laplacianEigenvectors noElementsPerEigenvector: %d\n", noEigenvectors, noElementsPerEigenvector);
    
    TFltVV embeddedVectors(noElementsPerEigenvector, noEigenvectors);
    for (int j = 0; j < noElementsPerEigenvector; j++) {
        for (int i = 0; i < noEigenvectors; i++) {
            embeddedVectors.At(j, i) = laplacianEigenvectors[i][j];
            printf("embeddedVectors(n, k) -> n = %d, k = %d, at = %f\n", j, i, (float) embeddedVectors.At(j, i));
        }
    }
    
    for (int i = 0; i < laplacianEigenvalues.Len(); i++) {
        printf("i = %d -> laplacianEigenvalues = %f\n", i, (float) laplacianEigenvalues[i]);
    }
    
    this->embeddedVectors = embeddedVectors;
    
    printf("#####     embeddedVectors x dimension: %d, embeddedVectors y dimension: %d\n", embeddedVectors.GetXDim(), embeddedVectors.GetYDim());
}

void TGraphAlgo::TSpectralClusteringAlg::applyKMeansClusteringToEmbeddedVectors(const int& noClusters) {
    printf("\n#####      applying KMeansClusteringToEmbeddedVectors...\n");
    
    int noDataPoints = embeddedVectors.GetXDim();
    int noElementsPerDataPoint = embeddedVectors.GetYDim();
    
    TIntVV binaryIndicators(noDataPoints, noClusters);
    
    printf("#####     binaryIndicators x dimension: %d, binaryIndicators y dimension: %d\n", binaryIndicators.GetXDim(), binaryIndicators.GetYDim());
    
    // set random seed
    TInt::Rnd.Randomize();
    
    TFltVV mu(noClusters, noElementsPerDataPoint);
    
    printf("#####     mu x dimension: %d, mu y dimension: %d\n", mu.GetXDim(), mu.GetYDim());
    
    // initialise mean for each cluster
    for (int k = 0; k < noClusters; k++) {
        for (int element = 0; element < noElementsPerDataPoint; element++) {
            mu.At(k, element) = (float) TInt::Rnd.GetUniDev();
//            printf("initial -> mu(k, element) -> k = %d, element = %d, at = %f\n", k, element, (float) mu.At(k, element));
        }
    }
    
    double objectiveFunctionValue = 0.0;
    double iterationObjectiveFunctionDelta = 1.0;
    double convergenceBound = 0.0001;
    int iteration = 0;
    
    while (iterationObjectiveFunctionDelta > convergenceBound) {
        // Step 1
        for (int n = 0; n < noDataPoints; n++) {
            int argmin = 0;
            double minNormValue = LONG_MAX;
            for (int k = 0; k < noClusters; k++) {
                TFltV dataMeanDifferenceVector(noElementsPerDataPoint);
                double normValue = 0;
                for (int element = 0; element < noElementsPerDataPoint; element++) {
                    dataMeanDifferenceVector[element] = embeddedVectors.At(n, element) - mu.At(k, element);
                    normValue += TMath::Sqr(dataMeanDifferenceVector[element]);
                }
                if (normValue < minNormValue) {
                    argmin = k;
                    minNormValue = normValue;
                }
            }
            for (int k = 0; k < noClusters; k++) {
                binaryIndicators.At(n, k) = (k == argmin) ? 1 : 0;
//                printf("binaryIndicators.At(n, k) -> n = %d, k = %d, at = %d\n", n, k, (int) binaryIndicators.At(n, k));
            }
        }
        
        // Step 2
        for (int k = 0; k < noClusters; k++) {
            TFltV sumBinaryIndicatorsData(noElementsPerDataPoint);
            double sumBinaryIndicators = 0;
            for (int n = 0; n < noDataPoints; n++) {
                int binaryIndicator = binaryIndicators.At(n, k);
                sumBinaryIndicators += binaryIndicator;
                for (int element = 0; element < noElementsPerDataPoint; element++) {
                    sumBinaryIndicatorsData[element] += (binaryIndicator * embeddedVectors.At(n, element));
                }
            }
//            printf("for k = %d -> sumBinaryIndicators = %f\n", k, sumBinaryIndicators);
            for (int element = 0; element < noElementsPerDataPoint; element++) {
                if (sumBinaryIndicators != 0) {
                    mu.At(k, element) = sumBinaryIndicatorsData[element] / sumBinaryIndicators;
                }
                else {
                  mu.At(k, element) = LONG_MAX;
                }
//                printf("sumBinaryIndicatorsData(element) -> element = %d, at = %f\n", element, (float) sumBinaryIndicatorsData[element]);
//                printf("mu(k, element) -> k = %d, element = %d, at = %f\n", k, element, (float) mu.At(k, element));
            }
        }
        
        // check objective function value
        double previousObjectiveFunctionValue = objectiveFunctionValue;
        objectiveFunctionValue = 0.0;
        for (int n = 0; n < noDataPoints; n++) {
            for (int k = 0; k < noClusters; k++) {
                TFltV dataMeanDifferenceVector(noElementsPerDataPoint);
                double normValue = 0;
                for (int element = 0; element < noElementsPerDataPoint; element++) {
                    dataMeanDifferenceVector[element] = embeddedVectors.At(n, element) - mu.At(k, element);
                    normValue += TMath::Sqr(dataMeanDifferenceVector[element]);
                }
                objectiveFunctionValue += binaryIndicators.At(n, k) * normValue;
            }
        }
        iterationObjectiveFunctionDelta = objectiveFunctionValue - previousObjectiveFunctionValue;
        
        printf("iteration = %d, previousObjectiveFunctionValue = %f, objectiveFunctionValue = %f, iterationObjectiveFunctionDelta = %f\n", iteration, previousObjectiveFunctionValue, objectiveFunctionValue, iterationObjectiveFunctionDelta);
        
        if (iterationObjectiveFunctionDelta < 0) {
            iterationObjectiveFunctionDelta *= -1;
        }
        
        iteration++;
    }
    
    clusterAssignment = binaryIndicators;
}

void TGraphAlgo::generateNetwork(const int& noNodes, const int& noCommunities, const double& pIn, const double& pOut) {
    
    groundTruthGraph = TKColourNet::New();
    for (int i = 0; i < noNodes; i++) {
        int communityLabel = i%noCommunities;
        communityLabels.Add(communityLabel);
        groundTruthGraph->AddNode(i, TKColourNode(communityLabel));
    }
    int srcNodeId, dstNodeId;
    double coinBias;
    
    // set random seed
    TInt::Rnd.Randomize();
    
    for (int i = 0; i < noNodes; i++) {
        for (int j = 0; j < noNodes; j++) {
            srcNodeId = i;
            dstNodeId = j;
            if(!(groundTruthGraph->IsEdge(srcNodeId, dstNodeId)) && (i != j)) {
                coinBias = (communityLabels[srcNodeId] == communityLabels[dstNodeId]) ? pIn : pOut;
                double flipResult = TInt::Rnd.GetUniDev();
                if (flipResult <= coinBias) {
                    groundTruthGraph->AddEdge(srcNodeId, dstNodeId);
                }
            }
        }
    }
}

void TGraphAlgo::TSpectralClusteringAlg::generateUndirectedTestGraph(const int& noNodes, int d_regular, const double& probabilityWithinCommunity) {
    
    printf("\n#####      generating UndirectedTestGraph...\n");
    
    if (d_regular > (int) noNodes/4) {
        d_regular = (int) noNodes/4;
    }
    
    groundTruthUndirectedTestGraph = PUNGraph::New();
    for (int i = 0; i < noNodes; i++) {
        groundTruthUndirectedTestGraph->AddNode(i);
    }
    int srcNodeId, dstNodeId;
    
    // set random seed
    TInt::Rnd.Randomize();
    
    for (int i = 0; i < noNodes; i++) {
        int noEdgesWithinCommunity = 0;
        int noEdgesBetweenCommunity = 0;
        
        while ((noEdgesWithinCommunity + noEdgesBetweenCommunity) < d_regular) {
            double flipResult = TInt::Rnd.GetUniDev();
            
            if (flipResult <= probabilityWithinCommunity) {
                srcNodeId = i;
                dstNodeId = srcNodeId;
                while (((srcNodeId % noColours) != (dstNodeId % noColours)) || (srcNodeId == dstNodeId) || (groundTruthUndirectedTestGraph->IsEdge(srcNodeId, dstNodeId))) {
                    dstNodeId = TInt::Rnd.GetUniDevInt(0, noNodes-1);
                }
                groundTruthUndirectedTestGraph->AddEdge(srcNodeId, dstNodeId);
                groundTruthUndirectedTestGraph->AddEdge(dstNodeId, srcNodeId);
                
                noEdgesWithinCommunity++;
            }
            else {
                srcNodeId = i;
                dstNodeId = srcNodeId;
                while (((srcNodeId % noColours) == (dstNodeId % noColours)) || (srcNodeId == dstNodeId) || (groundTruthUndirectedTestGraph->IsEdge(srcNodeId, dstNodeId))) {
                    dstNodeId = TInt::Rnd.GetUniDevInt(0, noNodes-1);
                }
                groundTruthUndirectedTestGraph->AddEdge(srcNodeId, dstNodeId);
                groundTruthUndirectedTestGraph->AddEdge(dstNodeId, srcNodeId);
                
                noEdgesBetweenCommunity++;
            }
        }
    }
    printf("***   Total number of  Edges: %d\n", groundTruthUndirectedTestGraph->GetEdges());
}

void TGraphAlgo::saveGroundTruth(const TStr& networkFilename) {
    TFOut FOut(networkFilename);
    
	// write nodes to file
	for (TKColourNet::TNodeI NI = groundTruthGraph->BegNI(); NI < groundTruthGraph->EndNI(); NI++) {
		FOut.PutStr(TStr::Fmt("%d,%d,", NI.GetId(), NI.GetId()));
        FOut.PutStr(TStr::Fmt("%d",NI.GetDat().getCommunityLabel()));
        FOut.PutStr(TStr::Fmt("\r\n"));
	}
    
	FOut.PutStr("\r\n");
    
	// write edges to file (not allowing self loops in the network)
	for (TKColourNet::TEdgeI EI = groundTruthGraph->BegEI(); EI < groundTruthGraph->EndEI(); EI++) {
		// not allowing self loops
		if (EI.GetSrcNId() != EI.GetDstNId()) {
            FOut.PutStr(TStr::Fmt("%d,%d,", EI.GetSrcNId(), EI.GetDstNId()));
            TKColourNet::TNodeI srcNI = groundTruthGraph->GetNI(EI.GetSrcNId());
            TKColourNet::TNodeI dstNI = groundTruthGraph->GetNI(EI.GetDstNId());
            int srcNodeColourId = srcNI.GetDat().getCommunityLabel();
            int dstNodeColourId = dstNI.GetDat().getCommunityLabel();
            FOut.PutStr(TStr::Fmt("[%d -> %d],", srcNodeColourId, dstNodeColourId));
            FOut.PutStr(TStr::Fmt("\r\n"));
		}
	}
}

void TGraphAlgo::saveGroundTruthAdjacencyMatrix(const TStr& networkFilename) {
    TFOut FOut(networkFilename);
    
    // create adjacency matrix
    const int noNodes = groundTruthGraph->GetNodes();
    
    TFltVV groundTruthAdjMtx(noNodes, noNodes);
    TIntH NodeIdH;
    for (TKColourNet::TNodeI NI = groundTruthGraph->BegNI(); NI < groundTruthGraph->EndNI(); NI++) {
        NodeIdH.AddKey(NI.GetId());
    }
    
    for (TKColourNet::TNodeI NI = groundTruthGraph->BegNI(); NI < groundTruthGraph->EndNI(); NI++) {
        const int NodeId = NodeIdH.GetKeyId(NI.GetId());
        for (int e = 0; e < NI.GetOutDeg(); e++) {
            const int DstNId = NodeIdH.GetKeyId(NI.GetOutNId(e));
            // no self edges
            if (NodeId != DstNId) {
                groundTruthAdjMtx.At(NodeId, DstNId) = 1;
            }
        }
    }
    
    for (int row = 0; row < groundTruthAdjMtx.GetRows(); row++) {
        for (int col = 0; col < groundTruthAdjMtx.GetCols(); col++) {
            FOut.PutStr(TStr::Fmt("%d ", (int) groundTruthAdjMtx.At(row, col)));
        }
        FOut.PutStr("\r\n");
    }
    
    this->groundTruthAdjMtx = groundTruthAdjMtx;
    
    //    printf("Saved Adjacency matrix with : noRows = %d, noCols = %d\n", groundTruthAdjMtx.GetRows(), groundTruthAdjMtx.GetCols());
}

void TGraphAlgo::saveGroundTruthGexf(const TStr& networkFilename) {
    TFOut FOut(networkFilename);
    
    FOut.PutStr(TStr::Fmt("<gexf version=\"1.1\"><meta><creator>Hesam Ipakchi</creator></meta><graph defaultedgetype=\"directed\" idtype=\"string\" type=\"static\">"));
    
    // write nodes to file
    const int noNodes = groundTruthGraph->GetNodes();
    FOut.PutStr(TStr::Fmt("<nodes count=\"%d\">",noNodes));
	for (TKColourNet::TNodeI NI = groundTruthGraph->BegNI(); NI < groundTruthGraph->EndNI(); NI++) {
		FOut.PutStr(TStr::Fmt("<node id=\"%d\" label=\"%d\"/>", NI.GetId(), NI.GetId()));
	}
	FOut.PutStr(TStr::Fmt("</nodes>"));
    
    // write edges to file
    const int noEdges = groundTruthGraph->GetEdges();
    FOut.PutStr(TStr::Fmt("<edges count=\"%d\">",noEdges));
    int edgeId = 0;
    for (TKColourNet::TEdgeI EI = groundTruthGraph->BegEI(); EI < groundTruthGraph->EndEI(); EI++) {
        FOut.PutStr(TStr::Fmt("<edge id=\"%d\" source=\"%d\" target=\"%d\"/>", edgeId, EI.GetSrcNId(), EI.GetDstNId()));
        edgeId++;
    }
    FOut.PutStr(TStr::Fmt("</edges>"));
    
    FOut.PutStr(TStr::Fmt("</graph></gexf>"));
}
