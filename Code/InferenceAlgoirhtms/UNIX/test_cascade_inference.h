#ifndef test_casacde_inference_h
#define test_casacde_inference_h

#include "Snap.h"

// 2 colours
#define k 2

// 0 => susceptible, 1 => active, 2 => inactive
#define susceptibleState 0
#define activeState 1
#define inactiveState 2

// Hit info (timestamp, candidate parent) about a node in a cascade
class THitInfo {
public:
  TInt NId, Parent;
  TFlt Tm;
public:
  THitInfo(const int& NodeId=-1, const double& HitTime=0) : NId(NodeId), Parent(-1), Tm(HitTime) { }
  THitInfo(TSIn& SIn) : NId(SIn), Parent(SIn), Tm(SIn) { }
  void Save(TSOut& SOut) const { NId.Save(SOut); Parent.Save(SOut); Tm.Save(SOut); }
  bool operator < (const THitInfo& Hit) const {
    return Tm < Hit.Tm; }
};

// Cascade
class TCascade {
public:
  THash<TInt, THitInfo> NIdHitH;
  TFlt CurProb, Alpha;
  TInt Model;
  double Eps;
public:
  TCascade() : NIdHitH(), CurProb(0), Alpha(1.0), Eps(1e-64), Model(0) { }
  TCascade(const double &alpha) : NIdHitH(), CurProb(0), Eps(1e-64), Model(0) { Alpha = alpha; }
  TCascade(const double &alpha, const int &model) : NIdHitH(), CurProb(0), Eps(1e-64) { Alpha = alpha; Model = model; }
  TCascade(const double &alpha, const double &eps) : NIdHitH(), CurProb(0), Model(0) { Alpha = alpha; Eps = eps;}
  TCascade(const double &alpha, const int &model, const double &eps) : NIdHitH(), CurProb(0) { Alpha = alpha; Model = model; Eps = eps; }
  TCascade(TSIn& SIn) : NIdHitH(SIn), CurProb(SIn), Alpha(SIn) { }
  void Save(TSOut& SOut) const  { NIdHitH.Save(SOut); CurProb.Save(SOut); Alpha.Save(SOut); }
  void Clr() { NIdHitH.Clr(); CurProb = 0; Alpha = 1.0; }
  int Len() const { return NIdHitH.Len(); }
  int GetNode(const int& i) const { return NIdHitH.GetKey(i); }
  int GetParent(const int NId) const { return NIdHitH.GetDat(NId).Parent; }
  double GetAlpha() const { return Alpha; }
  double GetTm(const int& NId) const { return NIdHitH.GetDat(NId).Tm; }
  void Add(const int& NId, const double& HitTm) { NIdHitH.AddDat(NId, THitInfo(NId, HitTm)); }
  void Del(const int& NId) { NIdHitH.DelKey(NId); }
  bool IsNode(const int& NId) const { return NIdHitH.IsKey(NId); }
  void Sort() { NIdHitH.SortByDat(true); }
  double TransProb(const int& NId1, const int& NId2) const;
  double GetProb(const PNGraph& G);
  void InitProb();
  double UpdateProb(const int& N1, const int& N2, const bool& UpdateProb=false);
};

// Node info (name and number of cascades)
class TNodeInfo {
public:
  TStr Name;
  TInt Vol;
public:
  TNodeInfo() { }
  TNodeInfo(const TStr& NodeNm, const int& Volume) : Name(NodeNm), Vol(Volume) { }
  TNodeInfo(TSIn& SIn) : Name(SIn), Vol(SIn) { }
  void Save(TSOut& SOut) const { Name.Save(SOut); Vol.Save(SOut); }
};

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

// Greedy algorithm class
class TGreedyAlg {
    
public:
    class TKColourNet;
    typedef TPt<TKColourNet> PKColourNet;
    
    static TStr convertColourIdToColour(const int& colourId) {
        switch (colourId) {
            case 0:
                return TStr("Red");
            case 1:
                return TStr("Blue");
            default:
                return TStr("Red");
        }
    }
    
    TStr getEdgeColourTransitionInfo(const int& srcNodeColourId, const int& dstNodeColourId) {
        return convertColourIdToColour(srcNodeColourId) + TStr("->") + convertColourIdToColour(dstNodeColourId);
    }
    
    /* K Coloured Node */
    class TKColourNode {
    private:
        TInt colourId;
        TStr colour;
        
    public:
        TKColourNode() {
            colourId = 0;
            colour = TStr("Red");
        }
        
        TKColourNode(const int& colourId) {
            this->colourId = colourId;
            this->colour = TGreedyAlg::convertColourIdToColour(colourId);
        }
        
        TKColourNode(TSIn& SIn) {}
        
        void Save(TSOut &SOut) const {}
        
        int getColourId() const {
            return colourId;
        }
        
        TStr getColour() const {
            return colour;
        }
        
        friend class TKColourNet;
    };
    
    /* K Coloured Network */
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
    
    TNodeInfo GetNodeInfo(const int& NId) const { return nodeNmH.GetDat(NId); }
    
    bool IsNodeNm(const int& NId) const { return nodeNmH.IsKey(NId); }
    
    void generateNetwork(const int& noNodes);
    
    void saveGroundTruth(const TStr& networkFilename);
    
    void saveGroundTruthAdjacencyMatrix(const TStr& networkFilename);
    
    void generateCascades(const int& noCasacdes, const double& pInit, const double& p, const double& q);
    
    void saveCascades(const TStr& casacdesFilename);
    
    void addGroundTruthGraph(const PKColourNet& pNet);
    
    void addCascade(const TCascade& cascade);
    
    void addCascade(const TStr& cascadeStr);
    
    void addNodeNm(const int& NId, const TNodeInfo& Info);
    
    TStr getNodeNm(const int& NId) const;
    
    void loadCascadesFromFile(TSIn& SIn);
    
    void runGreedyAlgorithm();
    
    void saveOutput(const TStr& outputNetworkFilename);
    
    void saveOutputAdjacencyMatrix(const TStr& outputNetworkFilename);
    
    void compareGroundTruthAndOutputAdjacencyMatrix(const TStr& groundTruthAdjacencyMatrixFilename, const TStr& outputAdjacencyMatrixFilename);
    
public:
    TVec<TCascade> cascadeV;
    PKColourNet outputGraph, groundTruthGraph;
    THash<TInt, TNodeInfo> nodeNmH;
    
};

#endif
