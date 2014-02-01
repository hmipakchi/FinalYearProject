#include "stdafx.h"
#include "test_cascade_inference.h"

int main(int argc, char* argv[]) {
    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("\nGenerate stochastic block model networks. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
    TExeTm ExeTm;
    Try
    
    const int noNodes = Env.GetIfArgPrefixInt("-n:", 512, "Number of nodes in synthetic graph (default: 512)\n");
    const double pIn = Env.GetIfArgPrefixFlt("-pIn:", 0.8, "pIn (default: 0.8)\n");
    const double pOut = Env.GetIfArgPrefixFlt("-pOut:", 0.2, "pOut (default: 0.2)\n");
    const int noCommunities = Env.GetIfArgPrefixInt("-k:", 2, "Number of communities in graph (default: 2)\n");
    
    TGraphAlgo graphAlgo;
    
    graphAlgo.generateNetwork(noNodes, noCommunities, pIn, pOut);
    
    TStr networkFilename = TStr("test-network-sbm.txt");
    TStr networkAdjacencyMatrixFilename = TStr("test-network-sbm-adjacency-matrix.txt");
    TStr networkGexfFilename = TStr("test-network-sbm.gexf");
    TStr networkLouvainFormatFilename = TStr("test-network-sbm-louvain.txt");
    TStr louvainTreeFilename = TStr("test-network-sbm-louvain.tree");
    TStr communityLabelsFilename = TStr("test-network-sbm-assignments.txt");
    
    graphAlgo.saveGroundTruth(networkFilename);
    graphAlgo.saveGroundTruthAdjacencyMatrix(networkAdjacencyMatrixFilename);
    graphAlgo.saveGroundTruthGexf(networkGexfFilename);
    graphAlgo.convertGroundTruthToLouvainFormat(networkLouvainFormatFilename);
    graphAlgo.saveCommunityLabels(communityLabelsFilename, noNodes, noCommunities);
    
    Catch
    printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    
    return 0;
}