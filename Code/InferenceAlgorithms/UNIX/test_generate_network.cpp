#include "stdafx.h"
#include "test_cascade_inference.h"

int main(int argc, char* argv[]) {
    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("\nGenerate different synthetic networks & cascades. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
    TExeTm ExeTm;
    Try
    
    const double pInit = Env.GetIfArgPrefixFlt("-i:", 0.4, "pInit (default: 0.4)\n");
    const double p = Env.GetIfArgPrefixFlt("-p:", 0.8, "p (default: 0.8)\n");
    const double q = Env.GetIfArgPrefixFlt("-q:", 0.2, "q (default: 0.2)\n");
    const int noNodes = Env.GetIfArgPrefixInt("-n:", 512, "Number of nodes in synthetic graph (default: 512)\n");
    const int noCascades = Env.GetIfArgPrefixInt("-c:", 5, "Number of cascades generated (default: 5)\n");
    
    TGreedyAlg greedyAlg;
    
    greedyAlg.generateNetwork(noNodes);
    
    TStr networkFilename = TStr("test-network.txt");
    TStr networkAdjacencyMatrixFilename = TStr("test-network-adjacency-matrix.txt");
    TStr cascadesFilename = TStr("test-cascades.txt");
    
    greedyAlg.saveGroundTruth(networkFilename);
    greedyAlg.saveGroundTruthAdjacencyMatrix(networkAdjacencyMatrixFilename);
    
    greedyAlg.generateCascades(noCascades, pInit, p, q);
    greedyAlg.saveCascades(cascadesFilename);
    
    Catch
    printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    
    return 0;
}