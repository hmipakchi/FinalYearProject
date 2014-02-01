#ifndef test_casacde_inference_h
#define test_casacde_inference_h

#include "Snap.h"
#include <climits>

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
    
    TFltVV getGroundTruthAdjMtx() const { return groundTruthAdjMtx; }
    
    void generateNetwork(const int& noNodes, const int& noCommunities, const double& pIn, const double& pOut);
    
    void saveGroundTruth(const TStr& networkFilename);
    
    void saveGroundTruthAdjacencyMatrix(const TStr& networkFilename);
    
    void saveGroundTruthGexf(const TStr& networkFilename);
    
    void convertGroundTruthToLouvainFormat(const TStr& networkFilename);
    
    void saveCommunityLabels(const TStr& networkFilename, const int& noNodes, const int& noCommunities);
    
public:
    PKColourNet groundTruthGraph;
    TFltVV groundTruthAdjMtx;
    TIntV communityLabels;
};

#endif