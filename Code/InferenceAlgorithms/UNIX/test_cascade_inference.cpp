#include "stdafx.h"
#include "test_cascade_inference.h"

double TCascade::TransProb(const int& NId1, const int& NId2) const {
	if (! IsNode(NId1) || ! IsNode(NId2)) { return Eps; }
	if (GetTm(NId1) >= GetTm(NId2)) { return Eps; }
	if (Model==0)
		return Alpha*exp(-Alpha*(GetTm(NId2)-GetTm(NId1))); // exponential
	else if (Model==1)
		return (Alpha-1)*pow((GetTm(NId2)-GetTm(NId1)), -Alpha); // power-law
	else
		return Alpha*(GetTm(NId2)-GetTm(NId1))*exp(-0.5*Alpha*pow(GetTm(NId2)-GetTm(NId1), 2)); // rayleigh
    
	return (-1);
}

double TCascade::GetProb(const PNGraph& G) {
    double P = 0;
    for (int n = 0; n < Len(); n++) {
        const int DstNId = GetNode(n);
        const double DstTm = GetTm(DstNId);
        TNGraph::TNodeI NI = G->GetNI(DstNId);
        double MxProb = log(Eps);
        int BestParent = -1;
        for (int e = 0; e < NI.GetInDeg(); e++) {
            const int SrcNId = NI.GetInNId(e);
            if (IsNode(SrcNId) && GetTm(SrcNId) < DstTm) {
                const double Prob = log(TransProb(SrcNId, DstNId));
                if (MxProb < Prob) { MxProb = Prob;  BestParent = SrcNId; }
            }
        }
        NIdHitH.GetDat(DstNId).Parent = BestParent;
        P += MxProb;
    }
    
    return P;
}

// init prob of a cascade in an empty graph
void TCascade::InitProb() {
    CurProb = log(Eps) * Len();
    for (int i = 0; i < Len(); i++) {
        NIdHitH[i].Parent = -1; }
}

// update the cascade probability given a new edge (N1, N2) in the graph
double TCascade::UpdateProb(const int& N1, const int& N2, const bool& UpdateProb) {
    if (!IsNode(N1) || !IsNode(N2)) { return CurProb; }
    if (GetTm(N1) >= GetTm(N2)) { return CurProb; }
    const double P1 = log(TransProb(GetParent(N2), N2));
    const double P2 = log(TransProb(N1, N2)); // N1 influences N2
    if (P1 < P2) {
        if (UpdateProb) { // the edge is there, update the CurProb and best Parent
            CurProb = CurProb - P1 + P2;
            NIdHitH.GetDat(N2).Parent = N1;
        } else {
            return CurProb - P1 + P2; }
    }
    return CurProb;
}

void TGreedyAlg::generateNetwork(const int& noNodes) {
    int maxNoEdgesOut = 3;
    
    groundTruthGraph = TKColourNet::New();
    for (int i = 0; i < noNodes; i++) {
        int colourId = i%k;
        groundTruthGraph->AddNode(i, TKColourNode(colourId));
    }
    int srcNodeId, dstNodeId;
    
    // set random seed
    TInt::Rnd.Randomize();
    
    for (int i = 0; i < noNodes; i++) {
        int noEdges = 0;
        while (noEdges < maxNoEdgesOut) {
            srcNodeId = i;
            dstNodeId = TInt::Rnd.GetUniDevInt(0, noNodes-1);
            if ((srcNodeId != dstNodeId) && (!groundTruthGraph->IsEdge(srcNodeId, dstNodeId))) {
                groundTruthGraph->AddEdge(srcNodeId, dstNodeId);
                noEdges++;
            }
        }
    }
    printf("noNodes = %d, noEdges = %d\n", noNodes, groundTruthGraph->GetEdges());
}

void TGreedyAlg::saveGroundTruth(const TStr& networkFilename) {
    TFOut FOut(networkFilename);
    
	// write nodes to file
	for (TKColourNet::TNodeI NI = groundTruthGraph->BegNI(); NI < groundTruthGraph->EndNI(); NI++) {
		FOut.PutStr(TStr::Fmt("%d,%d,", NI.GetId(), NI.GetId()));
        FOut.PutStr(NI.GetDat().getColour());
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
            int srcNodeColourId = srcNI.GetDat().getColourId();
            int dstNodeColourId = dstNI.GetDat().getColourId();
            FOut.PutStr(getEdgeColourTransitionInfo(srcNodeColourId, dstNodeColourId));
            FOut.PutStr(TStr::Fmt("\r\n"));
		}
	}
}

void TGreedyAlg::saveGroundTruthAdjacencyMatrix(const TStr& networkFilename) {
    TFOut FOut(networkFilename);
    
    // create adjacency matrix
    const int noNodes = groundTruthGraph->GetNodes();
    
    TFltVV AdjMtx(noNodes, noNodes);
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
                AdjMtx.At(NodeId, DstNId) = 1;
            }
        }
    }
    
    for (int row = 0; row < AdjMtx.GetRows(); row++) {
        for (int col = 0; col < AdjMtx.GetCols(); col++) {
            FOut.PutStr(TStr::Fmt("%d,", (int) AdjMtx.At(row, col)));
        }
        FOut.PutStr("\r\n");
    }
    
    //    printf("Saved Adjacency matrix with : noRows = %d, noCols = %d\n", AdjMtx.GetRows(), AdjMtx.GetCols());
}

void TGreedyAlg::generateCascades(const int& noCasacdes, const double& pInit, const double& p, const double& q) {
    int noNodes = groundTruthGraph->GetNodes();
    //    printf("Generating cascade for graph with noNodes = %d:\n\n", noNodes);
    if (noNodes == 0) {
        return;
    }
    
    // set random seed
    TInt::Rnd.Randomize();
    
    for (int casacdeI = 0; casacdeI < noCasacdes; casacdeI++) {
        TCascade cascade;
    
        double globalTime = 0;
        int noActiveNodes = 0;
        int noSusceptibleNodes = 0;
        int noInactiveNodes = 0;
        TIntIntH nodeStates;
        
        // flip biased coin for each node to collect seeds
        for (TKColourNet::TNodeI NI = groundTruthGraph->BegNI(); NI < groundTruthGraph->EndNI(); NI++) {
            const int nodeId = NI.GetId();
            double flipResult = TInt::Rnd.GetUniDev();
//            printf("nodeId = %d, flipResult = %f\n", nodeId, (float) flipResult);
            if (flipResult <= pInit) {
                nodeStates.AddDat(nodeId) = activeState;
    //            printf("#####     ADD TO CASCADE:   nodeId = %d, globalTime = %f     #####\n", nodeId, globalTime);
                cascade.Add(nodeId, globalTime);
                noActiveNodes++;
            }
            else {
                nodeStates.AddDat(nodeId) = susceptibleState;
                noSusceptibleNodes++;
            }
        }
        
        globalTime++;
        
        while (noActiveNodes > 0) {
            //        printf("\n*****   noActiveNodes = %d, noSusceptibleNodes = %d, noInactiveNodes = %d, globalTime = %f   *****\n\n", noActiveNodes, noSusceptibleNodes, noInactiveNodes, (float) globalTime);
            
            const TIntIntH beginningNodeStates = nodeStates;
            // for each node in the graph
            for (TKColourNet::TNodeI NI = groundTruthGraph->BegNI(); NI < groundTruthGraph->EndNI(); NI++) {
                const int nodeId = NI.GetId();
                const int nodeColourId = NI.GetDat().getColourId();
                const int nodeState = beginningNodeStates.GetDat(nodeId);
                //            printf("nodeId = %d, nodeColourId = %d, nodeState = %d\n", nodeId, nodeColourId, nodeState);
                // if node is active, infect susceptible child nodes with probability pij
                if (nodeState == activeState) {
                    const TKColourNet::TNodeI LNI = groundTruthGraph->GetNI(nodeId);
                    for (int e = 0; e < LNI.GetOutDeg(); e++) {
                        const int childNodeId = LNI.GetOutNId(e);
                        const int childNodeColourId = LNI.GetOutNDat(e).getColourId();
                        const int childNodeState = nodeStates.GetDat(childNodeId);
                        //                    printf("childNodeId = %d, childNodeColourId = %d, childNodeState = %d\n", childNodeId, childNodeColourId, childNodeState);
                        if (childNodeState == susceptibleState) {
                            double probablityInfection = (nodeColourId == childNodeColourId) ? p : q;
                            double flipResult = TInt::Rnd.GetUniDev();
                            //                        printf("childNodeId = %d, probabilityInfection = %f, flipResult = %f\n", childNodeId, (float) probablityInfection, (float) flipResult);
                            // infection occurred: childNode goes from susceptible -> active
                            if (flipResult <= probablityInfection) {
                                nodeStates[childNodeId] = activeState;
                                //                            printf("#####     ADD TO CASCADE:   nodeId = %d, globalTime = %f     #####\n", childNodeId, globalTime);
                                cascade.Add(childNodeId, globalTime);
                                noActiveNodes++;
                                noSusceptibleNodes--;
                            }
                        }
                    }
                    // main node goes from active -> inactive
                    nodeStates[nodeId] = inactiveState;
                    noActiveNodes--;
                    noInactiveNodes++;
                }
            }
            globalTime++;
        }
        addCascade(cascade);
    }
}

void TGreedyAlg::saveCascades(const TStr& casacdesFilename) {
    TFOut FOut(casacdesFilename);
    
    // write nodes to file
	for (TKColourNet::TNodeI NI = groundTruthGraph->BegNI(); NI < groundTruthGraph->EndNI(); NI++) {
		FOut.PutStr(TStr::Fmt("%d,%d\r\n", NI.GetId(), NI.GetId()));
	}
    
    FOut.PutStr("\r\n");
    
	// write cascades to file
	for (int i = 0; i < cascadeV.Len(); i++) {
		TCascade &C = cascadeV[i];
		int j = 0;
		for (THash<TInt, THitInfo>::TIter NI = C.NIdHitH.BegI(); NI < C.NIdHitH.EndI(); NI++, j++) {
			if (j > 0)
				FOut.PutStr(TStr::Fmt(";%d,%f", NI.GetDat().NId.Val, NI.GetDat().Tm.Val));
			else
				FOut.PutStr(TStr::Fmt("%d,%f", NI.GetDat().NId.Val, NI.GetDat().Tm.Val));
		}
        
		if (C.Len() >= 1)
			FOut.PutStr(TStr::Fmt("\r\n"));
	}
}

void TGreedyAlg::addGroundTruthGraph(const PKColourNet& pNet) {
    groundTruthGraph = pNet;
}

void TGreedyAlg::addCascade(const TCascade& cascade) {
    cascadeV.Add(cascade);
}

void TGreedyAlg::addCascade(const TStr& cascadeStr) {
    TStrV NIdV;
    cascadeStr.SplitOnAllCh(';', NIdV);
    TCascade C;
    for (int i = 0; i < NIdV.Len(); i++) {
    	TStr NId, Tm; NIdV[i].SplitOnCh(NId, ',', Tm);
        IAssert( IsNodeNm(NId.GetInt()) );
        GetNodeInfo(NId.GetInt()).Vol = GetNodeInfo(NId.GetInt()).Vol + 1;
        C.Add(NId.GetInt(), Tm.GetFlt());
    }
    C.Sort();
    cascadeV.Add(C);
}

void TGreedyAlg::addNodeNm(const int& NId, const TNodeInfo& Info) {
    nodeNmH.AddDat(NId, Info);
}

TStr TGreedyAlg::getNodeNm(const int& NId) const {
    return nodeNmH.GetDat(NId).Name;
}

void TGreedyAlg::loadCascadesFromFile(TSIn& SIn) {
    TStr line;
	SIn.GetNextLn(line);
	while (!SIn.Eof() && line != "") {
		TStrV NIdV;
        line.SplitOnAllCh(',', NIdV);
	    addNodeNm(NIdV[0].GetInt(), TNodeInfo(NIdV[1], 0)); SIn.GetNextLn(line);
    }
	printf("All nodes read!\n");
	while (!SIn.Eof()) {
        SIn.GetNextLn(line); addCascade(line);
    }
	printf("All cascades read!\n");
}

void TGreedyAlg::runGreedyAlgorithm() {
    outputGraph = TKColourNet::New();
    
    for (THash<TInt, TNodeInfo>::TIter NI = nodeNmH.BegI(); NI < nodeNmH.EndI(); NI++) {
        outputGraph->AddNode(NI.GetKey(), TKColourNode());
        printf("Added node %d to output graph\n", (int) NI.GetKey());
    }
    
    // for each node i
    for (THash<TInt, TNodeInfo>::TIter NI = nodeNmH.BegI(); NI < nodeNmH.EndI(); NI++) {
        int nodeI = NI.GetKey();
        printf("*****     Considering node i: %d     *****\n", nodeI);
        
        // initialise unaccounted cascades U
        TVec<TCascade> cascades = cascadeV;
        
        // initialise parental neighbourhood
        TIntV parentalNeighbourhood;
        
        bool uselessUnaccountedCasacdesLeft = false;
        while ((cascades.Len() != 0) && (!uselessUnaccountedCasacdesLeft)) {
//            printf("%d Casacdes left...\n", cascades.Len());
            
            // find node j that could possible have infected node i that has been observed for largest number of cascades
            int argmax = -1;
            int maxNoCascades = 0;
            
            bool areUnaccountedCascadesUseless = true;
            for (THash<TInt, TNodeInfo>::TIter LNI = nodeNmH.BegI(); LNI < nodeNmH.EndI(); LNI++) {
                int nodeJ = LNI.GetKey();
//                printf("nodeJ: %d\n", nodeJ);
                int countPotentialNoCascades = 0;
                if (nodeI != nodeJ) {
                    for (int c = 0; c < cascades.Len(); c++) {
                        TCascade cascade = cascades[c];
                        if (cascade.IsNode(nodeJ) && cascade.IsNode(nodeI)) {
//                            if (cascade.GetTm(nodeI) > cascade.GetTm(nodeJ)) {
                            if (cascade.GetTm(nodeJ) == (cascade.GetTm(nodeI) - 1)) {
                                countPotentialNoCascades++;
                            }
                        }
                    }
                    if (countPotentialNoCascades > maxNoCascades) {
                        maxNoCascades = countPotentialNoCascades;
                        argmax = nodeJ;
                        areUnaccountedCascadesUseless = false;
                    }
                    else {
                        areUnaccountedCascadesUseless = areUnaccountedCascadesUseless && true;
                    }
                }
            }
            
            if (areUnaccountedCascadesUseless) {
                uselessUnaccountedCasacdesLeft = true;
            }
            
            if (argmax != -1) {
//                printf("argmax (k) = %d, noCasacdesAppearedIn = %d\n", argmax, maxNoCascades);
                
                // add arg max (k) to the set of parental neighbours
                parentalNeighbourhood.Add(argmax);
            }
            
            // remove the cascades which k belongs to
            TIntV cascadesToRemove;
            for (int c = 0; c < cascades.Len(); c++) {
                TCascade cascade = cascades[c];
                if (cascade.IsNode(argmax) && cascade.IsNode(nodeI)) {
                    if (cascade.GetTm(nodeI) > cascade.GetTm(argmax)) {
                        cascadesToRemove.Add(c);
                    }
                }
            }
            cascadesToRemove.Sort();
            
//            printf("cascadesToRemove: ");
            for (int i = 0; i < cascadesToRemove.Len(); i++) {
//                printf("%d, ", (int) cascadesToRemove[i]);
                cascades.Del(cascadesToRemove[i]-i);
            }
//            printf("\n");
        }
        
        // add edges from node i to each of parent
        for (int i = 0; i < parentalNeighbourhood.Len(); i++) {
            int srcNodeId = parentalNeighbourhood[i];
            int dstNodeId = nodeI;
            outputGraph->AddEdge(srcNodeId, dstNodeId);
//            printf("#####     Added Edge: %d -> %d     #####\n", srcNodeId, dstNodeId);
        }
    }
}

void TGreedyAlg::saveOutput(const TStr& outputNetworkFilename) {
    TFOut FOut(outputNetworkFilename);
    
	// write nodes to file
	for (TKColourNet::TNodeI NI = outputGraph->BegNI(); NI < outputGraph->EndNI(); NI++) {
		FOut.PutStr(TStr::Fmt("%d,%d,", NI.GetId(), NI.GetId()));
        FOut.PutStr(NI.GetDat().getColour());
        FOut.PutStr(TStr::Fmt("\r\n"));
	}
    
	FOut.PutStr("\r\n");
    
	// write edges to file (not allowing self loops in the network)
	for (TKColourNet::TEdgeI EI = outputGraph->BegEI(); EI < outputGraph->EndEI(); EI++) {
		// not allowing self loops
		if (EI.GetSrcNId() != EI.GetDstNId()) {
            FOut.PutStr(TStr::Fmt("%d,%d,", EI.GetSrcNId(), EI.GetDstNId()));
            TKColourNet::TNodeI srcNI = outputGraph->GetNI(EI.GetSrcNId());
            TKColourNet::TNodeI dstNI = outputGraph->GetNI(EI.GetDstNId());
            int srcNodeColourId = srcNI.GetDat().getColourId();
            int dstNodeColourId = dstNI.GetDat().getColourId();
            FOut.PutStr(getEdgeColourTransitionInfo(srcNodeColourId, dstNodeColourId));
            FOut.PutStr(TStr::Fmt("\r\n"));
		}
	}
}

void TGreedyAlg::saveOutputAdjacencyMatrix(const TStr& outputNetworkFilename) {
    TFOut FOut(outputNetworkFilename);
    
    // create adjacency matrix
    const int noNodes = outputGraph->GetNodes();
    
    TFltVV AdjMtx(noNodes, noNodes);
    TIntH NodeIdH;
    for (TKColourNet::TNodeI NI = outputGraph->BegNI(); NI < outputGraph->EndNI(); NI++) {
        NodeIdH.AddKey(NI.GetId());
    }
    for (TKColourNet::TNodeI NI = outputGraph->BegNI(); NI < outputGraph->EndNI(); NI++) {
        const int NodeId = NodeIdH.GetKeyId(NI.GetId());
        for (int e = 0; e < NI.GetOutDeg(); e++) {
            const int DstNId = NodeIdH.GetKeyId(NI.GetOutNId(e));
            // no self edges
            if (NodeId != DstNId) {
                AdjMtx.At(NodeId, DstNId) = 1;
            }
        }
    }
    
    for (int row = 0; row < AdjMtx.GetRows(); row++) {
        for (int col = 0; col < AdjMtx.GetCols(); col++) {
            FOut.PutStr(TStr::Fmt("%d,", (int) AdjMtx.At(row, col)));
        }
        FOut.PutStr("\r\n");
    }
    
    //    printf("Saved Adjacency matrix with : noRows = %d, noCols = %d\n", AdjMtx.GetRows(), AdjMtx.GetCols());
}

void TGreedyAlg::compareGroundTruthAndOutputAdjacencyMatrix(const TStr& groundTruthAdjacencyMatrixFilename, const TStr& outputAdjacencyMatrixFilename) {
    
    TFIn fInpGT(groundTruthAdjacencyMatrixFilename);
    TFIn fInpO(outputAdjacencyMatrixFilename);
    
    int elementGT, elementO;
    int noErrors = 0;
    int totalNoElements = 0;
    TStr lineGT, lineO;
    
    while (!(fInpGT.Eof() || fInpO.Eof())) {
        fInpGT.GetNextLn(lineGT);
        TStrV rowVGT;
        lineGT.SplitOnAllCh(',', rowVGT);
        
        fInpO.GetNextLn(lineO);
        TStrV rowVO;
        lineO.SplitOnAllCh(',', rowVO);
        
        int noColsAdjacencyMatrixGT = rowVGT.Len();
        int noColsAdjacencyMatrixO = rowVO.Len();
        
        if (noColsAdjacencyMatrixGT != noColsAdjacencyMatrixO) {
            printf("*** Ground Truth and Output graph Mismatch Error: Adjacency matrix for Ground Truth has: %d columns, but Adjacency Matrix for Output has: %d columns !!! ***\n", noColsAdjacencyMatrixGT, noColsAdjacencyMatrixO);
        }
        
        for (int e = 0; e < noColsAdjacencyMatrixGT; e++) {
            elementGT = rowVGT[e].GetInt();
            elementO = rowVO[e].GetInt();
            if (elementGT != elementO) {
                noErrors++;
            }
            totalNoElements++;
        }
    }
    
    if (totalNoElements != 0) {
        printf("\n#####     No Errors = %d, totalNoElements = %d, percentage incorrect = %f     #####\n", noErrors, totalNoElements, (float) 100*noErrors/totalNoElements);
    }
    else {
        printf("\n#####     No elements in Adjacency Matrix !!!!!     #####\n");
    }
}
