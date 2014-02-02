#if !defined(EDGE_INCLUDED)
#define EDGE_INCLUDED

/*
 The edge class which stores information on the edges
 weight, and its originating and termination nodes in the network.
 */
class edge {

 public:
  double  we;					// edge betweenness weight
  double  Globalwe;                             // inputed edge weight
  int     so;					// originating node
  int     si;					// terminating node
  int     IDso;                                 // inputed originating node
  int     IDsi;                                 // inputed terminating node
  int     key;                                  // unique edge key

  bool    removed;                              //has edge been remove from network

  edge();
  edge(int i, int f, double w);
  edge(int i, int f, double w, int k);
  edge(int i, int f, double w, int k, int I, int F);
  ~edge();						// default destructor
  void print();
  void print(fstream *fout);
  edge Clone();

};
  
edge::edge(){ 
so = 0; si = 0; IDso = 0; IDsi = 0; removed = false;
}
  
edge::edge(int i, int f, double w){ 
so = i; si = f; IDso = 0; IDsi = 0; we = 0; Globalwe = w; removed = false;
}
   
edge::edge(int i, int f, double w, int k){ 
so = i; si = f; IDso = 0; IDsi = 0; we = 0; Globalwe = w; key = k; removed = false;
}

edge::edge(int i, int f, double w, int k, int I, int F){
  so = i; si = f; IDso = I; IDsi = F; we = 0; Globalwe = w; key = k; removed = false;
}
  
edge::~edge() { }

void edge::print(){ 
  cout << " (so -- si) " << so << "(" << IDso << ") -- " << si << "(" << IDsi << ") " << " (weight) " << we << " (Globalweight) " << Globalwe 
       << " (key) " << key << " (removed) "<< removed <<endl;}


void edge::print(fstream* fout ){ 
  (*fout) << so << "\t" << IDso << "\t" << si << "\t" << IDsi << "\t" 
	  << we << "\t" << Globalwe << "\t" << key << endl;}

//void edge::print(fstream* fout ){ 
//  (*fout) << " (so -- si) " << so << "(" << IDso << ") -- " << si << "(" << IDsi << ") " 
//	  << " (weight) " << we << " (Globalweight) " << Globalwe 
//	  << " (key) " << key << " (removed) "<< removed << endl;}


inline edge edge::Clone(){ return *this; }


#endif


