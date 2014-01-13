#include "stdafx.h"
#include "test_cascade_inference.h"

int main(int argc, char* argv[]) {
    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("\nGenerate different synthetic networks & cascades. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
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
    
    graphAlgo.saveGroundTruth(networkFilename);
    graphAlgo.saveGroundTruthAdjacencyMatrix(networkAdjacencyMatrixFilename);
    graphAlgo.saveGroundTruthGexf(networkGexfFilename);
    
//    TGreedyAlg::TSpectralClusteringAlg spectralClusteringAlg;
//    
//    spectralClusteringAlg.generateUndirectedTestGraph(noNodes, d_regular, pEdgeWithinCommunityNodes);
//    
//    spectralClusteringAlg.generateGroundTruthUndirectedTestAdjacencyMatrix();
//    
//    spectralClusteringAlg.initialiseLaplacianMatrix(spectralClusteringAlg.getGroundTruthUndirectedTestGraph(), d_regular);
//    
//    TLAMisc::SaveMatlabTFltVV(spectralClusteringAlg.laplacianMatrix, TStr("laplacian-matrix.txt"));
    
//    spectralClusteringAlg.calculateLaplacianEigenvectors();
//    
//    spectralClusteringAlg.applyKMeansClusteringToEmbeddedVectors(noColours);
//    
//    spectralClusteringAlg.printClusterAssignments();
    
    Catch
    printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    
    return 0;
}