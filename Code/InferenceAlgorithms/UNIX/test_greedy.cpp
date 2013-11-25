#include "stdafx.h"
#include "test_cascade_inference.h"

int main(int argc, char* argv[]) {
    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("\nGenerate output of Greedy Algorithm. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
    TExeTm ExeTm;
    Try
    
    const TStr inputCascadesFilename  = Env.GetIfArgPrefixStr("-i:", "test-cascades.txt", "Input cascades file (one file)");
    const TStr outputNetworkFilename = Env.GetIfArgPrefixStr("-o:", "test-output-network", "Output filename prefix (default: test-output-network)\n");
    const int compareGroundTruthAndOutput = Env.GetIfArgPrefixInt("-c:", 1, "Compare Ground Truth with Output - 0 for No, 1 for Yes (default: 1)\n");
    
    TGreedyAlg greedyAlg;
    
    TFIn FIn(inputCascadesFilename);
    greedyAlg.loadCascadesFromFile(FIn);
    
    greedyAlg.runGreedyAlgorithm();
    
    greedyAlg.saveOutput(TStr::Fmt("%s.txt", outputNetworkFilename.CStr()));
    greedyAlg.saveOutputAdjacencyMatrix(TStr::Fmt("%s-adjacency-matrix.txt", outputNetworkFilename.CStr()));
    
    if (compareGroundTruthAndOutput == 1) {
        TStr groundTruthAdjacencyMatrixFilename = TStr("test-network-adjacency-matrix.txt");
        TStr outputAdjacencyMatrixFilename = TStr::Fmt("%s-adjacency-matrix.txt", outputNetworkFilename.CStr());
        greedyAlg.compareGroundTruthAndOutputAdjacencyMatrix(groundTruthAdjacencyMatrixFilename, outputAdjacencyMatrixFilename);
    }
    
    Catch
    printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    
    return 0;
}
