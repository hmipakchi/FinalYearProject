#ifndef test_casacde_inference_h
#define test_casacde_inference_h

#include "Snap.h"
#include <climits>

// 3 colours
#define noColours 3

// 0 => susceptible, 1 => active, 2 => inactive
#define susceptibleState 0
#define activeState 1
#define inactiveState 2

// Edge info (name and number of cascades)
class TEdgeInfo {
public:
  TInt Vol;
  TFlt MarginalGain, MarginalBound, MedianTimeDiff, AverageTimeDiff; // we can skip MarginalBound for efficiency if not explicitly required
public:
  TEdgeInfo() { }
  TEdgeInfo(const int& v,
		    const double& mg,
		    const double& mb,
		    const double& mt,
			const double& at) : Vol(v), MarginalGain(mg), MarginalBound(mb), MedianTimeDiff(mt), AverageTimeDiff(at) { }
  TEdgeInfo(const int& v,
		    const double& mg,
		    const double& mt,
			const double& at) : Vol(v), MarginalGain(mg), MarginalBound(0), MedianTimeDiff(mt), AverageTimeDiff(at) { }
  TEdgeInfo(TSIn& SIn) : Vol(SIn), MarginalGain(SIn), MarginalBound(SIn), MedianTimeDiff(SIn), AverageTimeDiff(SIn) { }
  void Save(TSOut& SOut) const { Vol.Save(SOut); SOut.Save(MarginalGain); SOut.Save(MarginalBound); SOut.Save(MedianTimeDiff); SOut.Save(AverageTimeDiff); } //
};

// Graph algorithm class
class TGraphAlgo {
    
public:
    class TKColourNet;
    typedef TPt<TKColourNet> PKColourNet;
    
    /* K Coloured Node */
    class TKColourNode {
    private:
        TInt communityLabel;
        
    public:
        TKColourNode() {
            communityLabel = 0;
        }
        
        TKColourNode(const int& communityLabel) {
            this->communityLabel = communityLabel;
        }
        
        TKColourNode(TSIn& SIn) {}
        
        void Save(TSOut &SOut) const {}
        
        int getCommunityLabel() const {
            return communityLabel;
        }
        
        friend class TKColourNet;
    };
    
    /* K Community Network */
    class TKColourNet : public TNodeNet<TKColourNode> {
    public:
        typedef TNodeNet<TKColourNode> TNet;
        typedef TPt<TNodeNet<TKColourNode> > PNet;
    public:
        TKColourNet() {}
        
        TKColourNet(TSIn& SIn) : TNet(SIn) {}
        
        void Save(TSOut& SOut) const {
            TNet::Save(SOut);
        }
        
        static PKColourNet New() {
            return new TKColourNet;
        }
        
        friend class TPt<TKColourNet>;
    };
    
    // spectral clustering
    class TSpectralClusteringAlg {
    public:
        PUNGraph groundTruthUndirectedTestGraph;
        TFltVV groundTruthUndirectedTestAdjacencyMatrix;
        TFltVV laplacianMatrix;
        TFltV laplacianEigenvalues;
        TVec<TVec<TFlt> >/*TFltVV*/ laplacianEigenvectors;
        TFltVV embeddedVectors;
        TIntVV clusterAssignment;
    public:
        TSpectralClusteringAlg() { printf("\n#####     create TSpectralClusteringAlg object...\n"); }
        
        PUNGraph getGroundTruthUndirectedTestGraph() const { return groundTruthUndirectedTestGraph; }
        
        TFltVV getGroundTruthUndirectedTestAdjacencyMatrix() const { return groundTruthUndirectedTestAdjacencyMatrix; }
        
        void generateUndirectedTestGraph(const int& noNodes, int d_regular, const double& probabilityWithinCommunity);
        
        void generateGroundTruthUndirectedTestAdjacencyMatrix();
        
        void initialiseLaplacianMatrix(const PUNGraph& graph, const int& d_regular);
        
        void calculateLaplacianEigenvectors();
        
        void applyKMeansClusteringToEmbeddedVectors(const int& noClusters);
        
        void printClusterAssignments() const {
            printf("\n#####     printing Cluster Assignments...\n");
            for (int n = 0; n < clusterAssignment.GetXDim(); n++) {
                printf("n = %d -> [", n);
                for (int k = 0; k < clusterAssignment.GetYDim(); k++) {
                    printf("%d ", (int) clusterAssignment.At(n,k));
                }
                printf("]\n");
            }
        }
    };
    
    TFltVV getGroundTruthAdjMtx() const { return groundTruthAdjMtx; }
    
    void generateNetwork(const int& noNodes, const int& noCommunities, const double& pIn, const double& pOut);
    
    void saveGroundTruth(const TStr& networkFilename);
    
    void saveGroundTruthAdjacencyMatrix(const TStr& networkFilename);
    
    void saveGroundTruthGexf(const TStr& networkFilename);
    
public:
    PKColourNet groundTruthGraph;
    TFltVV groundTruthAdjMtx;
    TIntV communityLabels;
};

#endif