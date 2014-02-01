#include "stdafx.h"
#include "test_cascade_inference.h"

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

void TGraphAlgo::convertGroundTruthToLouvainFormat(const TStr& networkFilename) {
    TFOut FOut(networkFilename);
    
    // write edges to file
	for (TKColourNet::TEdgeI EI = groundTruthGraph->BegEI(); EI < groundTruthGraph->EndEI(); EI++) {
		// not allowing self loops
		if (EI.GetSrcNId() != EI.GetDstNId()) {
            FOut.PutStr(TStr::Fmt("%d %d", EI.GetSrcNId(), EI.GetDstNId()));
            TKColourNet::TNodeI srcNI = groundTruthGraph->GetNI(EI.GetSrcNId());
            TKColourNet::TNodeI dstNI = groundTruthGraph->GetNI(EI.GetDstNId());
            int srcNodeColourId = srcNI.GetDat().getCommunityLabel();
            int dstNodeColourId = dstNI.GetDat().getCommunityLabel();
            FOut.PutStr(TStr::Fmt("\r\n"));
		}
	}
}

void TGraphAlgo::saveCommunityLabels(const TStr& networkFilename, const int& noNodes, const int& noCommunities) {
    TFOut FOut(networkFilename);
    FOut.PutStr(TStr::Fmt("%d %d", noNodes, noCommunities));
    FOut.PutStr(TStr::Fmt("\r\n"));
    for (int i = 0; i < noNodes; i++) {
        FOut.PutStr(TStr::Fmt("%d %d", i, (int) communityLabels[i]));
        FOut.PutStr(TStr::Fmt("\r\n"));
    }
}