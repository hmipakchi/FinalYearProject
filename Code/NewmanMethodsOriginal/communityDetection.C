
//----------------------------------------------
// Community Detection Algrithm
// Colin Mclean -- <cmclean5@staffmail.ed.ac.uk> (2011)
//-----------------------------------------------
/////////////////////////////////////////////////
// Based on the fast community algorithm designed by Aaron Clauset <aaron@cs.unm.edu>, Chris Moore, Mark Newman,
// , the R IGraph library Copyright (C) 2007  Gabor Csardi <csardi@rmki.kfki.hu> and the LUDecomposition C++ 
// classes given in Numerical Recipies 3rd Edition W. Press, S. Teukolsky, W. Vetterling, B. Flannery.
//-----------------------------------------------

#include "Headers.h"
#include "nr3.h"
#include "ran.h"
#include "ludcmp.h"
#include "eigen_sym.h"
#include "edge.h"
#include "node.h"
#include "Helper.h"
#include "readInputFile.h"

using namespace std;



//--- METHOD DECLARATIONS
//-------------------------------------------------------------------------------------

//--- Methods for Geodesic edge Betweenness algorithm
//-------------------------------------------------------------------------------------
//--- Finding and evaluating community structure in networks, M. Newman and M. Girvan
//--- Physical Review E69, 026113 (2004). --- [1]
void   calculateEdgeBetweennessGeodesic();
void   assignNodeWeights( int );

//--- Methods for Random edge Betweenness algorithm
//-------------------------------------------------------------------------------------
//--- Finding and evaluating community structure in networks, M. Newman and M. Girvan
//--- Physical Review E69, 026113 (2004). --- [1]
void calculateEdgeBetweennessRandom();
void calculateRandomWalk( int, vector<node> );
void setupMatrices  ();
void upDateMatrices ();
void getSubMatrix   ( int, vector<node> & );
void removeMatrixRow( MatDoub Unr, MatDoub &outnr );
void removeMatrixRow( MatDoub &out );
void removeMatrixRow(int row, MatDoub &out);
void addMatrixRows(MatDoub U, int c, MatDoub &Ui);
void addMatrixRow(MatDoub U, int row, MatDoub &out);

//--- Methods for Spectral Modularity algorithm
//-------------------------------------------------------------------------------------
//--- Finding community structure in networks using eigenvectors of matrices
//--- M. Newman, arXix:physics/0605087v3 (2006). --- [2]
void calculateSpectralModularity();
void calculateB(MatDoub, MatDoub &);
void calculateEigenVectors();
void findLeadingEigenVectors(int & );
void maximiseIndexVectors(int);
void modifySplit(double, int );
void maxModularity( double & );
void deltaModularity( double &);
void deltaModularityMax( int, double &);
int  maxCommunity();

void splitP( MatDoub, VecInt, int, double );
void splitN( MatDoub, VecInt, int, double );

//--- Methods shared between algorithms
void Modularity       ( double &, double & );
int  findMaxModularity( vector<double> );
int  deltaCommunity   ( int, int );

void getSubSample( vector<int> & );
vector<node> storeNodes();
int findMax( vector<double> );

void printHelpMessage( const char *);
void DrawProgressBar(int, double);

//--- GLOBAL VARIABLES
//-------------------------------------------------------------------------------------
vector<node>  n(3000);
vector<node>  Gn(3000);

vector<edge>  elist(10000);
vector<edge>  Gelist(10000);
vector<edge>  totallist(10000);

vector<node>  emptyn;
vector<edge>  emptye;

int        *dataI   = new int   [5000];
int        *datainI = new int   [5000];
double     *data    = new double[5000*5000];
double     *datain  = new double[5000*5000];
double     *datainO = new double[5000*5000];
double      temp_score [5000];
stack<int> node_dist;

readInputFile reader;
Helper        ihelper;

fstream* modularityscore;
fstream* communityout;
fstream* removededges;
fstream* consensusout;

fstream* forcytoscape;

vector<double>         vec_mod;
vector<double>         vec_mod_err;
vector<int>            vec_com_max;
vector< vector<node> > vec_nodes;

MatDoub R       = MatDoub(2000,2000);
MatDoub Ri      = MatDoub(2000,2000);
MatDoub Rh      = MatDoub(2000,2000);
MatDoub A       = MatDoub(2000,2000);
MatDoub Ai      = MatDoub(2000,2000);
MatDoub Bi      = MatDoub(2000,2000);

VecInt C        = VecInt(2000);

MatDoub S       = MatDoub(2000,1);
MatDoub V       = MatDoub(2000,1);
MatDoub T       = MatDoub(2000,2000);
MatDoub Ti      = MatDoub(2000,2000);
MatDoub Rc      = MatDoub(2000,2000);
MatDoub Vi      = MatDoub(2000,1);

MatDoub B       = MatDoub(2000,2000);
MatDoub Bm      = MatDoub(2000,2000);

MatDoub Bgi     = MatDoub(2000,2000);

VecInt keys_p   = VecInt(2000);
VecInt keys_n   = VecInt(2000);

MatDoub SI      = MatDoub(2000,2);
VecDoub si      = VecDoub(2000);
VecDoub visited = VecDoub(2000);

MatDoub u       = MatDoub(2000,2000);
VecDoub betai   = VecDoub(2000);

enum {NONE};

double specQ; //Recorded Modularity value from the Spectral method
double two_m; //2*m = Sum_i { K_i } = Sum_ij { A_ij }
double _norm; //1.0/(4*m)

int    com_max;

clock_t cstart; 
clock_t cend;

Ran _rand(0);

//--- MAIN PROGRAM
//-------------------------------------------------------------------------------------
int main(int argc, char * argv[]) {

  int seed;
  int a_type;
  int w_type;
  string title;
  string if_weighted;
  string if_help;
  const char *file_network;
  const char *file_names;  
    
  if ( argc != 5 ){ 
    printHelpMessage( argv[0] );
  }

  if_help = argv[1];
  if( if_help.compare("-h") == 0 || if_help.compare("-help") == 0 ){
    printHelpMessage( argv[0] );
  }

  seed = atoi(argv[1]);
  cout << "> seed is " << seed << endl;

  //--- Initialize random seed:	
  _rand.setSeed(seed);

  a_type = atoi(argv[2]);
  
  if( a_type < 1 || a_type > 3 ){
    cout << "argument 2: the type of algorithm to run needs to be either (1,2,3): " << endl;
    cout << "          : 1 = Geodesic edge Betweenness" << endl;
    cout << "          : 2 = Random edge Betweenness"   << endl;
    cout << "          : 3 = Spectral Betweenness"      << endl;
    exit(1);
  }

  switch(a_type){
    
  case 1:
    cout << "> Using Geodesic edge Betweenness." << endl;
    title = "Geodesic edge Betweenness.";
    break;
    
  case 2:
    cout << "> Using Random edge Betweenness." << endl;
    title = "RandomWalk edge Betweenness.";
    break;

  case 3:
    cout << "> Using Spectral Betweenness." << endl;
    title = "Spectral Betweenness.";
    break;

  default:
    break;

  }

  if_weighted = argv[3];

  if( if_weighted.compare("w") == 0 ){
    w_type = 3;
    cout << "> Using a weighted network " << endl;
  } else {
    if( if_weighted.compare("nw") == 0 ){
      w_type = 2;
      cout << "> Using a non-weighted network " << endl;
  } else {
      cout << "argument 3: specify if network file is weighted or not: " << endl;
      cout << "          : w  = Using a weighted network file " << endl;
      cout << "          : nw = Using a non-weighted network file " << endl;    
      exit(1);
    }
  }

  file_network = argv[4];  

  //--- Default values for parameters which may be modified from the commandline
  ihelper = Helper();
  reader.readFile(file_network, w_type);
  Gn      = reader.getNodeSet();
  Gelist  = reader.getEdgeSet();  

  vector<int> key_listi;
  vector<int> key_listj;
  vector<int> key_listk;
  
  cout << "> The Global node list..." << endl;
  for(int i=1; i<Gn.size(); i++){
    key_listi.push_back(Gn[i].ID);
    key_listj.push_back(Gn[i].ID);
    key_listk.push_back(-1);
    Gn[i].print();
    Gn[i].printEdges();
  }

  //--- To use getSubSample, comment following two lines, and 
  //--- uncomment getSubSample(key_listj).
  n     = Gn;
  elist = Gelist;
  //getSubSample(key_listj);

  //cout << "The sub-node list ... " << endl;
  //for(int i=1; i<n.size(); i++){
  //n[i].print();
  //n[i].printEdges();    
  //}

  cout << "> The Global edge list..." << endl;
  for(int i=0; i<elist.size(); i++){
    elist[i].print();
  }
  
  forcytoscape = new fstream("OUT/communities_newman.txt",ios_base::out);
  (*forcytoscape) << "communities" << endl;
    
  removededges = new fstream("OUT/removededges.txt",ios_base::out);
  (*removededges) << "Removed Edges" << endl;
  (*removededges) << "so \t IDso \t si \t IDsi \t we \t Globalweight \t key" << endl;

  totallist = ihelper.cloneEdgeList(elist);
  
  com_max      = 0;
  specQ        = 0.0;
  double Q     = 0.0; double Q_SD     = 0.0;
  double Q_old = 0.0; double Q_SD_old = 0.0;

  int loop       = elist.size();
  int E          = loop;
  double Q_max   = 0.0;
  double Q_limit = 1.0;
  bool stopping  = false;

  int N = n.size()-1;      
 
  R.resize(N,N);
  Ri.resize(N,N);
  A.resize(N,N);
  Ai.resize(N,N);    
  Bi.resize(N,N);
  C.resize(N); 

  S.resize(N,1);
  V.resize(N,1);
  T.resize(N,N);
  Ti.resize(N,N);
  Rc.resize((N-1),(N-1));
  Vi.resize(C.size(),1);

  B.resize(N,N);
  Bm.resize(N,N);
  Bgi.resize(N,N);

  keys_p.resize(N);
  keys_n.resize(N);

  u.resize(N,N);  //eigenvectors	
  betai.resize(N);//eigenvalues

  SI.resize(N,2);
  si.resize(N);
  visited.resize(N);

  setupMatrices();
 
  cout << "> Running " << title.c_str() << endl;

  cstart = clock();
  
  if( a_type == 3 ){
    //--- Calculate betweenness using the Spectral algorithm   
    calculateSpectralModularity();
  } else {

  while( loop !=0 && !stopping ){
    
    int old_max_com = com_max;
    
    //--- Calculate betweenness using Geodesic or RandomWalk algorithms 
    if( a_type == 1 )
      calculateEdgeBetweennessGeodesic();
    else
      calculateEdgeBetweennessRandom();            

    //--- Calculate the Modularity
    Q_old = Q; Q_SD_old = Q_SD; 
    Q     = 0.0; Q_SD   = 0.0;      
    
    Modularity(Q, Q_SD);
    
    //--- Store networks state if Modularity has increased during this iteraction
    if(com_max > old_max_com){
      vec_mod.push_back(Q); vec_mod_err.push_back(Q_SD); vec_com_max.push_back(com_max);
      vec_nodes.push_back(storeNodes());
    }
      
      
    //--- Record the maximum Modularity value
    if( Q > Q_max ){
      Q_max = Q;
    } else {
      if( Q_max > 0.0 && (Q_max - Q)/Q_max > Q_limit ) stopping = true;	
    }
      
    
    //--- Find edge with maximum edge betweenness score and remove
    edge _max;
    _max = totallist[1].Clone();
    for(int i=1; i<totallist.size(); i++){
      
      if( totallist[i].removed == false ){
	
	if(totallist[i].we >= _max.we){
	  
	  if(totallist[i].we > _max.we)
	    _max = totallist[i];
	  else{
	    int rdm = rand()%2;
	    if(rdm == 1) _max = totallist[i];
	  }		
	}
      }
      totallist[i].we = 0;
    }
    
    //--- Record the removed edges.
    _max.print( removededges );
    
    n[elist[_max.key-1].so].removeEdge(_max.key);
    n[elist[_max.key-1].si].removeEdge(_max.key);
    n[elist[_max.key-1].so].setDegree( (n[elist[_max.key-1].so].getDegree() - 1) );
    n[elist[_max.key-1].si].setDegree( (n[elist[_max.key-1].si].getDegree() - 1) );
    totallist[_max.key].removed = true;
    elist[_max.key-1].removed   = true;
    --loop;
      
    //--- Calculate the remaining processor time
    DrawProgressBar( 20, ((double)E - (double)loop)/(double)E );

  }
  }

  //--- Recored the CPU-time taken
  cend = clock();
  double cpu_time_used = ((double) (cend - cstart)) / CLOCKS_PER_SEC;
  cout << "" << endl;
  cout << "> cputime: " << cpu_time_used << " seconds " << endl;
  cout << "> Network (nodes): " << N << " (edges): " << E << endl;

  if( a_type != 3 ){

    //--- Print all stored Modularity values 
    modularityscore = new fstream("OUT/modularityscore.txt",ios_base::out);
    (*modularityscore) << title.c_str() << endl;
    for(int i=0; i<vec_mod.size(); i++){
      (*modularityscore) << vec_mod[i] << " " << vec_mod_err[i] << " " << vec_com_max[i] << endl;
    }
    modularityscore->close();
  
    int ind   = findMax(vec_mod);
    int com   = 1;
    int _size = 0;
    int c_max = com_max;

    //--- Print node communities for maximum Modularity value, for Geodesic or RandomWalk runs
    communityout = new fstream("OUT/communityout.txt",ios_base::out);	
    (*communityout) << "Max Q: " << vec_mod[ind] << " +- " << vec_mod_err[ind] << endl;  
    (*communityout) << "cputime: " << cpu_time_used << " seconds " << endl;
    (*communityout) << "Network (nodes): " << N << " (edges): " << E << endl;
  
    while(com<(c_max+1)){
      _size = 0;
      for(int i=0; i<vec_nodes[ind].size(); i++){
	if(vec_nodes[ind][i].c == com ){ 
	  (*communityout) << vec_nodes[ind][i].ID << "\t" << vec_nodes[ind][i].c << endl;
	  //vec_nodes[ind][i].print( communityout ); 
	  _size++; }
      }
      if(_size != 0)
	(*communityout) << "community: " << com << " size: " << _size << endl;
      com++;
    }
    
    for(int i=0; i<vec_nodes[ind].size(); i++){
      for(int j=0; j<key_listi.size(); j++){
	if(vec_nodes[ind][i].ID == key_listi[j]){
	  key_listk[j] = vec_nodes[ind][i].c;
	  break;
	}
      }
    }

    //--- Print node communities for maximum Modularity for the consensus matrix	 	  
    consensusout = new fstream("OUT/consensusout.txt",ios_base::out);
    (*consensusout) << "key list" << endl;
    for(int i=0; i<key_listi.size(); i++){
      if(key_listk[i] == -1 && key_listj[i] != -1)
	key_listj[i] = -1;
      (*consensusout) << key_listi[i] << " " << key_listj[i] << " " << key_listk[i] << endl;
      //(*consensusout) << key_listi[i] << " = " << key_listk[i] << endl;
      (*forcytoscape) << key_listi[i] << " = " << key_listk[i] << endl;
      cout << key_listi[i] << " " << key_listj[i] << " " << key_listk[i] << endl;
    }	  
  } else {

    int com   = 1;
    int _size = 0;
    int c_max = maxCommunity();

    //--- Store node communities for maximum Modularity for the Spectral Modularity run
    communityout = new fstream("OUT/communityout.txt",ios_base::out);
    (*communityout) << "communityout" << endl;
    (*communityout) << "Max Q: " << specQ << endl;
    (*communityout) << "cputime: " << cpu_time_used << " seconds " << endl;
    (*communityout) << "Network (nodes): " << N << " (edges): " << E << endl;
    while(com<(c_max+1)){
      _size = 0;
      for(int i=0; i<n.size(); i++){
	if(n[i].c == com ){ n[i].print( communityout ); _size++; }
      }
    if(_size != 0)
      (*communityout) << "community: " << com << " size: " << _size << endl;
    com++;
    }

    for(int i=1; i<n.size(); i++){
      for(int j=0; j<key_listi.size(); j++){
	if(n[i].ID == key_listi[j]){
	  key_listk[j] = n[i].c;
	  break;
	}
      }
    }

  //--- Print node communities for maximum Modularity the consensus matrix	 	  
  consensusout = new fstream("OUT/consensusout.txt",ios_base::out);
  (*consensusout) << "key list" << endl;
  for(int i=0; i<key_listi.size(); i++){
    if(key_listk[i] == -1 && key_listj[i] != -1)
      key_listj[i] = -1;
    (*consensusout) << key_listi[i] << " " << key_listj[i] << " " << key_listk[i] << endl;
    //(*consensusout) << key_listi[i] << " = " << key_listk[i] << endl;
    (*forcytoscape) << key_listi[i] << " = " << key_listk[i] << endl;
    cout << key_listi[i] << " " << key_listj[i] << " " << key_listk[i] << endl;
  }	  


  }


  //--- Remove data structures
  communityout->close();
  forcytoscape->close();
  vec_mod.clear();
  vec_mod_err.clear();
  vec_nodes.clear();

  exit(1);

}

void printHelpMessage( const char *arg1){

  cout << arg1 << " requires 4 arguments: " << endl;
  cout << "argument 1: random number seed  " << endl;
  cout << "argument 2: the type of algorithm to run " << endl;
  cout << "          : 1 = Geodesic edge Betweenness" << endl;
  cout << "          : 2 = Random edge Betweenness"   << endl;
  cout << "          : 3 = Spectral Betweenness"      << endl;
  cout << "argument 3: specify if network file is *weighted or not: " << endl;
  cout << "          : w  = Using a weighted network file " << endl;
  cout << "          : nw = Using a non-weighted network file " << endl;    
  cout << "argument 4: the network file to run  " << endl;
  cout << "Example   : ./run 1 1 testData/Kirate.wpairs " << endl;
  cout << "*         : The structure of the network file is:" << endl;
  cout << "w         : A (interactor) \t B (interactor) \t W (weight) " << endl;
  cout << "nw        : A (interactor) \t B (interactor) " << endl;
  cout << "          : Where A and B are integers and W is a double." << endl;
  exit(1);

}

void DrawProgressBar(int len, double percent) {
  cout << "\x1B[2K"; // Erase the entire current line.
  cout << "\x1B[0E"; // Move to the beginning of the current line.
  string progress;
  for (int i = 0; i < len; ++i) {
    if (i < static_cast<int>(len * percent)) {
      progress += "=";
    } else {
      progress += " ";
    }
  }
  cout << "> [" << progress << "] " << (static_cast<int>(100 * percent)) << "%";
  flush(cout); // Required.

}

/*
 Utility method to boot-strap the current node set size. 
 Randomly select 80% of nodes from current node set.
 */
void getSubSample( vector<int> &keys ){

  vector<node> Cn;
  vector<edge> Celist;

  for(int i=1; i<Gn.size(); i++){
    Gn[i].c = -1;
    Cn.push_back(Gn[i].Clone());
  }

  for(int i=0; i<Gelist.size(); i++){
    Celist.push_back(Gelist[i].Clone());    
  }	

  vector<int> rmKeys;
  vector<int> rmEdgeKeys;

  int N           = Cn.size();
  int sample_size = 0;
  double subN     = 0.8; //set to sample 80% of the current node set size.

  //generate a sub-sample using bootstrapped method
  while( sample_size < N*subN ){	
    
    int rnd_ind = (int)floor( _rand.doub() * (N+1) );

    if(Cn[rnd_ind].c == -1){
      Cn[rnd_ind].c = 1;
      sample_size++;	    
    }
  }
  
  for(int i=0; i<Cn.size(); i++){
    if(Cn[i].c == -1){
      keys[i] = -1;
      rmKeys.push_back(Cn[i].k);
    }	  
  }

  for(int i=0; i<rmKeys.size(); i++){
    
    for(int k=0; k<Celist.size(); k++){
      if( Celist[k].so == rmKeys[i]){
	Celist[k].so = -1; Celist[k].si = -1;
      }
    }
    
    for(int k=0; k<Celist.size(); k++){
      if( Celist[k].si == rmKeys[i]){
	Celist[k].so = -1; Celist[k].si = -1;
      }
    }

  }

  reader.enumerateNodeList(Cn, Celist, n, elist);

  Cn.swap(emptyn);
  Celist.swap(emptye);

}

/*
 Find the maximum stored Modularity value.
 */
int findMax(vector<double> m){

  double val = m[0];
  int    ind = 0;

  for(int i=0; i<m.size(); i++){
    if(m[i] > val){
      val = m[i];
      ind = i;
    }
  }

  return ind;

}

/*
 The change in Modularity used for the Spectral method.
 deltaQ = Sum_k { Sum_ij { si_ki * Bgi_ij * si_jk } }
*/
void deltaModularity( double &mod ){

  mod         = 0;
  
  int N       = si.size();
  double ele  = 0.0;
  double sum  = 0.0;

  MatDoub deltaQ(2,2);
  MatDoub SIt(N,2);

  for(int i=0; i<N; i++){
    SIt[i][0] = 0;
    SIt[i][1] = 0;
    for(int j=0; j<N; j++){
      SIt[i][0] += Bgi[i][j] * SI[j][0];
      SIt[i][1] += Bgi[i][j] * SI[j][1];
    }
  }

  for(int i=0; i<2; i++){
    double sum1 = 0;
    double sum2 = 0;
    for(int j=0; j<N; j++){
      sum1 += SI[j][0] * SIt[j][0];
      sum2 += SI[j][1] * SIt[j][1];
    }
    deltaQ[i][0] = sum1; 
    deltaQ[i][1] = sum2; 
  }

  for(int k=0; k<2; k++)
    sum += deltaQ[k][k];

  mod = _norm * sum;

}

/*
 The change in Modularity used during the fine-tuning
 method; where node si_i is moved from one community to 
 the other: if si^old_i = +-1 => si^new_i = -+1
 deltaQ = deltaQ^new - deltaQ^old
        = Sum_ij { Big_ij * si^new_i * si^new_j }  
        - Sum_ij { Big_ij * si^old_i * si^old_j }
        = Sum_(i!=k,j!=k) { Bgi_ij * si^new_i * si^new_j
	                    + Sum_(j!=k) Big_kj * si^new_k * si^new_j
                            + Sum_(i!=k) Big_ik * si^new_i * si^new_k 
                            + Big_kk } 
        - Sum_(i!=k,j!=k) { Big_ij si^old_i * si^old_j
                            - Sum_(j!=k) Big_kj * si^old_k * si^old_j
	                    - Sum_(i!=k) Big_ik * si^old_i * si^old_k 
			    - Big_kk }
        = Sum_(j!=k) { Big_kj * ( si^new_k - si^old_k ) * si^old_j }
	+ Sum_(i!=k) { Big_ik * si^old_i * ( si^new_k - si^old_k ) }
	=  2 * ( si^new_k - si^old_k ) * Sum_(i!=k) { Big_ik * si^old_i }
	= -4 * si^old_k * Sum_(i!=k) { Big_ik * si^old_i }
*/
void deltaModularityMax( int k, double &mod ){

  mod          = 0;
  
  int N        = si.size();
  double sumi  = 0.0;

  for(int i=0; i<N; i++){
    
    if( i!=k )
      sumi += Bgi[i][k] * si[i];
    
  }

  mod = -4.0 * si[k] * sumi;


}


/*
 The modularity (Q):

 Q = Sum_i { e_ii - a^2_i }
   = Tr e - |e^2|

 */
void Modularity( double &R, double &R_SD ){

  int eTot   = totallist.size()-1;
  int comtot = 1;
  for(int i=1; i<n.size(); i++)
    if(n[i].c > comtot) comtot = n[i].c;
  
  com_max = comtot;
  comtot += 1;
  
  double eii[comtot];
  double eij[comtot];

  for(int i=1; i<comtot; i++){ 
    eii[i]=0.0;    
    eij[i]=0.0;
  }

  for(int i=1; i<totallist.size(); i++){

    edge current = totallist[i];

    int Ci       = n[current.so].c;
    int Cj       = n[current.si].c;
    
    if(Ci == Cj) eii[Ci] += 2 * current.Globalwe;

    eij[Ci] += 1 * current.Globalwe;
    eij[Cj] += 1 * current.Globalwe;

  }
  
  double Qij  = 0;
  double Q    = 0;

  double rij3 = 0;
  double rij  = 0;

  for(int i=1; i<comtot; i++){
    
    Qij   = eij[i]/2/eTot;
    Q    += eii[i]/2/eTot;

    Q    -= Qij*Qij;

  }

  //The Modularity (R)
  R = Q;

  //Error on the Modularity (R_SD)
  //Obtained via the jack-knife method
  if( comtot > 2 )
    {
      for(int i=1; i<comtot; i++){
      
	rij  = eij[i]/2/eTot;
	rij3 = 0;
	
	for(int j=1; j<comtot; j++)
	  if( j !=i ) rij3 += eij[j]/2/eTot;
      
	R_SD += (rij - rij3/(comtot-1)) * (rij - rij3/(comtot-1));
	
      }

      double fac = (double)(comtot - 1)/(double)comtot;
      R_SD = sqrt( fac/eTot * R_SD );
    
    }


}

/*
 Utility method to store the current values of the nodes.
 */
vector<node> storeNodes(){

  vector<node> store;

  for(int i=1; i<n.size(); i++)
    store.push_back(n[i].Clone());
  
  return store;

}

/*
 Utility method used by the RandomWalk algorithm to 
 check the community structure of the network after 
 removing highest betweenness scoring edges.
 */
void visitedNodes(int rootnode){

  queue<int> Q;

  Q.push( rootnode );

  while( !Q.empty() ){

    int v = Q.front();
    Q.pop();

    vector<edge> edges = n[v].getEdges();
    for(int i=0; i<edges.size(); i++){

      int ind_j    = edges[i].si;
      if( edges[i].si == v )
	ind_j      = edges[i].so;

      if( n[ind_j].v == 0 ){
	n[ind_j].v = 1;      
	node_dist.push( n[ind_j].k );
	Q.push( ind_j );
      }
    }

  }

  return ;

}

/*
 Utility method used by Geodesic algorithm to 
 assign weights and distances to each node in the 
 connected component using a modified breadth-first search algorithm, 
 in accordance with [1] (see method declarations above). 
 */
void assignNodeWeights(int rootnode){
// Breadth-first search algorithm:

  queue<int> Q;

  Q.push( rootnode );

  while( !Q.empty() ){

    int v = Q.front();
    Q.pop();

    vector<edge> edges = n[v].getEdges();
    for(int i=0; i<edges.size(); i++){

      int ind_j    = edges[i].si;
      if( edges[i].si == v )
	ind_j      = edges[i].so;
      
      if( n[ind_j].w != 0 ){

	if( n[ind_j].d == n[v].d + 1 )
	  n[ind_j].w += n[v].w;
      } else {	  
	n[ind_j].w += n[v].w;
	n[ind_j].d  = n[v].d + 1; 
	node_dist.push( n[ind_j].k );
	Q.push(ind_j);
      }
    
    }
  }

  return ;

}

//-------------------------------------------------------------------
// Method to calculate the Geodesic edge Betweenness of the input graph
//-------------------------------------------------------------------
void calculateEdgeBetweennessGeodesic(){

  int nCommunity = 0;
 
  queue<int> tops ;
  temp_score [n.size()];

  //--- Reset node properties after removing the edge. 
  for(int i=1; i < n.size(); i++){

    n[i].c = 0;
    tops.push(n[i].k);

    }

  
  //--- Loop over every node in turn, treating as the 
  //--- root node.
  while ( !tops.empty() ){

    for(int i=0; i<elist.size(); i++)
      elist[i].we   = 0;

    int top = tops.front();
    tops.pop();
    node_dist.push( top );
    
    for(int i=1; i<n.size(); i++){     
      n[i].w        = 0.0;
      n[i].d        = 0.0;
      temp_score[i] = 0.0;
    }        

    n[top].w = 1.0;
    n[top].d = 0.0;
  
    //-- Assign Node weights & distances
    
    assignNodeWeights(top);
    
    //-- Communities in Network
    bool newCommunity = false;
    if( n[top].c == 0 ){
      nCommunity  += 1;
      newCommunity = true;
    } 
    
    //-- Assign Edges Weights
    while( !node_dist.empty() ){
      
      int _key = node_dist.top(); 
      node_dist.pop();

      if(newCommunity)
	n[_key].c = nCommunity;
      
      if( n[_key].d < 1 ) continue;

	vector<edge> edges = n[_key].getEdges();
	for( int i=0; i<edges.size(); i++ ){
	
	    int ind_j = edges[i].si;
	    if( edges[i].si == _key )
	      ind_j   = edges[i].so;

	    if( (n[ind_j].d == n[_key].d-1) && n[ind_j].w > 0 ){
	    
	      temp_score[ind_j]          += (temp_score[_key]+1) * (n[ind_j].w / n[_key].w);

	      elist[edges[i].key-1].we   += elist[edges[i].key-1].Globalwe * ( (temp_score[_key]+1) * (n[ind_j].w / n[_key].w) );
		
	      totallist[edges[i].key].we += elist[edges[i].key-1].we;

	    }
	}
    }	
    
  }
  

}


//-------------------------------------------------------------------
//Method to calculate the Random Walk edge Betweenness of the input graph
//-------------------------------------------------------------------
void calculateEdgeBetweennessRandom(){
  
  //--- Community detection...
  int nCommunity    = 0;
  
  for(int i=1; i<n.size(); i++){
    n[i].c = 0;
    n[i].v = 0;
    n[i].w = 0;
  }

  for(int i=1; i<n.size(); i++){
    
    for(int j=1; j<n.size(); j++)
      n[j].v = 0;
    
    node startNode  = n[i];
    node_dist.push( startNode.k );
    
    visitedNodes(startNode.k);
      
    //--- Communities in Network
    bool newCommunity = false;
    if( n[startNode.k].c == 0){
      nCommunity  += 1;
      newCommunity = true;
    }    

    while( !node_dist.empty() ){
      
      int _key = node_dist.top();
      node_dist.pop();
            
      if(newCommunity) 
	n[_key].c = nCommunity;      

    }

  }
 
  int com_max_old = com_max;
  com_max         = 0;
  for(int i=1; i<n.size(); i++){
     if( n[i].c > com_max ) 
       com_max = n[i].c;          
  }

  //--- Update the matrices
  upDateMatrices();

  vector<node> startNodes;
  
  for(int c=1; c<com_max+1; c++){
    
     //--- Setup the sub-matrices...    
     startNodes.clear();
     
     //--- Get the sub-matrices
     getSubMatrix(c, startNodes);    
     
     //--- Take a random walk within the network,
     //--- and calculate the edge betweenness scores...
     calculateRandomWalk(c, startNodes);

   }
  

}

/*
 Utility method used by the RandomWalk algorithm to 
 invert the Graph Laplacian and accummulate the random-path
 contributions from each source-sink node pair, in 
 accordance with [2] (see method declarations above).
 */
void calculateRandomWalk(int c, vector<node> Nodes){

   queue<node> termNodes;
   for(int i=0; i<Nodes.size(); i++)
     termNodes.push(Nodes[i]);

   int N = Nodes.size();
   S.resize(N,1); 
   V.resize(N,1);
   T.resize(N,N);
   Rc.resize((N-1),(N-1));
   Vi.resize(C.size(),1);

   //--- Remove arbitrary termination ('sink') state '0'. 
   removeMatrixRow(0,Rc);

   //--- Invert Matrix.
   LUdcmp lu( Rc );

   Ti.resize(Rc.nrows(),Rc.nrows());

   lu.inverse( Ti );
        
   for(int i=0; i< T.nrows(); i++){
     for(int j=0; j< T.nrows(); j++){
       T[i][j] = 0;
     }
   }

   //--- Add back arbitrary termination ('sink') state '0'.
   addMatrixRow(Ti,0,T);
  
     while ( !termNodes.empty() ){
       
       node termNode  = termNodes.front();
       termNodes.pop();
       
       for(int t=0; t< Nodes.size(); t++){	    
	 
	 //--- Take the next start ('source') state.
	 node startNode = Nodes[t];
	 
	 if( startNode.k != termNode.k ){
	 
	   for(int i=0; i<S.nrows(); i++){
	     S[i][0]  = 0;
	     V[i][0]  = 0;
	     Vi[i][0] = 0;
	   }

	   S[startNode.k][0] =  1;
	   S[termNode.k][0]  = -1;
  
	   //--- V = T * S
	   for(int i=0; i<T.nrows(); i++){
	     double sum = 0.0;
	     for(int j=0; j<T.nrows(); j++){
	       sum += T[i][j] * S[j][0];
	     }
	     V[i][0] = sum;
	   }

	   addMatrixRows(V, c, Vi);
	   
	   //--- Edge Betweenness, i.e.
	   //--- the currents (potential differences) alone each edge!
	   for(int i=0; i<elist.size(); i++){
	     
	     if( !elist[i].removed ){
	       int Ni = elist[i].so-1;
	       int Nj = elist[i].si-1;
	       
	       totallist[i+1].we += fabs(Vi[Ni][0] - Vi[Nj][0]);	    
	     
	     }
	   }//elist
	 }//Nodes   
       }//startNodes
       
     }

}

/*
 Utility method used by Geodesic and RandomWalk algorithms
 to set-up the Modularity and Laplacian matrices.
 */
void setupMatrices(){
  
  //--- Matrix size
  int N = R.nrows();

  //--- 2*m
  two_m = elist.size();

  //--- _norm
  _norm = 1.0/(2.0*two_m);

  for(int i=0; i<N; i++){
    C[i] = 0;    
    for(int j=0; j<N; j++){
      R[i][j] = 0.0;
      A[i][j] = 0.0;
    }
  }

  //--- Setup Matrices
  //--- Store the current community each vertex belongs too
  for(int i=0; i<N; i++){
    R[i][i] = n[i+1].getDegree();
    C[i]    = n[i+1].c;
  }

  //--- The Adjacency matrix, A
  for(int i=0; i<elist.size(); i++){
    int ind_i = elist[i].so -1;
    int ind_j = elist[i].si -1;	 

    if( ind_i == ind_j || ind_j == ind_i )
      A[ind_i][ind_j] = 1.0 * elist[i].Globalwe;
    else {
      A[ind_i][ind_j] = 1.0 * elist[i].Globalwe;
      A[ind_j][ind_i] = 1.0 * elist[i].Globalwe;
    }

  }

  for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
      R[i][j] = R[i][j] - A[i][j]; 
    }
  }

  //--- The Modularity matrix, Bgi
  for(int i=1; i<n.size(); i++){
    for(int j=1; j<n.size(); j++){
      Bgi[i-1][j-1] = A[i-1][j-1] - (n[i].getDegree() * n[j].getDegree() * _norm);
    }
  }
  
}

/*
 Utility method use by the RandomWalk algorithm to 
 update the Adjacency and Laplacian matrices.  
 */
void upDateMatrices(){

  for(int i=0; i<R.nrows(); i++){
    C[i] = 0;
    for(int j=0; j<R.nrows(); j++){
      R[i][j] = 0;
    }
  }

  //--- Setup Matrices
  //--- Store the current community each vertex belongs too
  for(int i=0; i<R.nrows(); i++){
    R[i][i] = n[i+1].getDegree();
    C[i]    = n[i+1].c;
  }

  //--- Update the Adjacency matrix, A
  for(int i=0; i<elist.size(); i++){
    
    if( elist[i].removed ){

      int ind_i = elist[i].so -1;
      int ind_j = elist[i].si -1;	 
 
      //if edge has been removed, remove this entry
      //from A.
      A[ind_i][ind_j] = 0;
      A[ind_j][ind_i] = 0;
    }
  }

  //--- Update the Lapacian matrix, R
  for(int i=0; i<R.nrows(); i++){
    for(int j=0; j<R.nrows(); j++){
      R[i][j] = R[i][j] - A[i][j]; 
    }
  }

}

/*
 Utility method used by RandomWalk algorithm to 
 resize the Graph Laplacian for each community, com,
 within the network.
 */
void getSubMatrix(int com, vector<node> &Nodes){
  
  int dummy = -1000;
  int rows  = 0;

  Rh.resize(R.nrows(), R.nrows());
  Rh = R;

  //--- NR style
  for( int i=0; i< C.size(); i++){

    if( C[i] == com )
      Nodes.push_back(node(rows++,0.0,0.0));
    else {
      for( int k=0; k<Rh.nrows(); k++){
	Rh[i][k] = dummy;
	Rh[k][i] = dummy;
      }
    }

  }

  datain = Rh.getMatrixArray();
  data   [Rh.nrows()*Rh.nrows()];
  int ind = 0;

  for(int i=0; i < Rh.nrows()*Rh.ncols(); i++){

    double ele = datain[i];
    if(ele != dummy)
      data[ind++] = ele;
   
  }

  Ri.resize(rows,rows);
  Ri = MatDoub( rows, rows, data );

}

void removeMatrixRow( MatDoub Unr, MatDoub &outnr ){

  int dummy = -1000;
  
  datain = Unr.getMatrixArray();
  data   [outnr.nrows()*outnr.nrows()];
  int k=0;

  for(int i=0; i < Unr.nrows()*Unr.nrows(); i++){

    double ele = datain[i];
    if(ele != dummy)
      data[k++] = ele;

  }

  outnr = MatDoub( outnr.nrows(), outnr.nrows(), data );


}

void removeMatrixRow( MatDoub &out ){

  int dummy = -1000;

  datain = Ri.getMatrixArray();
  data  [Ri.nrows()*Ri.nrows()];
  int k=0;

  for(int i=0; i < Ri.nrows()*Ri.ncols(); i++){

    double ele = datain[i];
    if(ele != dummy)
      data[k++] = ele;

  }

  out = MatDoub( out.nrows(), out.nrows(), data );


}

void removeMatrixRow(int row, MatDoub &out){

  int dummy = -1000;

  Rh.resize(Ri.nrows(),Ri.nrows());
  Rh = Ri;

  for(int i=0; i<Rh.nrows(); i++){
    Rh[row][i] = dummy;
    Rh[i][row] = dummy;
  }

  datain = Rh.getMatrixArray();
  data   [Rh.nrows()*Rh.nrows()];
  int k=0;

  for(int i=0; i < Rh.nrows()*Rh.nrows(); i++){

    double ele = datain[i];
    if(ele != dummy)
      data[k++] = ele;

  }

  out = MatDoub( out.nrows(), out.nrows(), data );

  
}

void addMatrixRows(MatDoub U, int c, MatDoub &Ui){

  int j=0;
  for(int i=0; i<C.size(); i++){

    if(C[i] == c)
      Ui[i][0] = U[j++][0];
    else
      Ui[i][0] = 0;

  }

}

void addMatrixRow(MatDoub U, int row, MatDoub &out){

  int dummy = -1000;

  for(int i=0; i<out.nrows(); i++){
    out[i][row] = dummy;
    out[row][i] = dummy;
  }

  datain   = U.getMatrixArray(); 
  datainO  = out.getMatrixArray(); 
  int k            = 0;
  
  data     [out.nrows()*out.nrows()];
  
  for(int i=0; i < out.nrows()*out.ncols(); i++){
    
    if( datainO[i] == dummy )
      data[i] = 0;
    else
      data[i] = datain[k++];
    
  }

  out = MatDoub( out.nrows(), out.nrows(), data );

}

//-------------------------------------------------------------------
// Method to calculate the Spectral Modularity
//-------------------------------------------------------------------
void calculateSpectralModularity(){

  int N = si.size();
  MatDoub Bg(N,N);
  Bg.resize(N,N);
  Bg = Bgi;

  //--- Calculate eigenvectors, and values, from Bgi...
  calculateEigenVectors();
  
  int ind = -1;
  findLeadingEigenVectors(ind);
	  
  cout << "> max EigenValue is " << betai[ind] << " with ind " << ind << endl;
  //--- set up the index vectors, si and SI, for the initial split
  maximiseIndexVectors(ind);

  double tol        = 0.00001;//the tolerance value, 10^-5; eigenvalues below this threshold are not used 
  int dummy         = -1000;
  double deltaQ_old = 0.0;
  double deltaQ_new = 0.0;
  
  //--- Calculate the Spectral Modularity
  deltaModularity(deltaQ_old);
  cout << "> Spectral Q: " << deltaQ_old << endl;

  double diff = deltaQ_old;

  //--- Fine tuning stage to maximum deltaModularity for the initial split
  while( diff > tol ){

    modifySplit( tol, N );
    
    deltaModularity( deltaQ_new );
    cout << "> Modified Q: " << deltaQ_new << endl;    
    
    diff = fabs( deltaQ_new - deltaQ_old ); 

    deltaQ_old = deltaQ_new;    

  }
  
  //--- Keep recorded of maximum fine-tuned Modularity value.
  specQ += deltaQ_old;

  cout << "> node list " << endl;
  for(int i=1; i<n.size(); i++){
    keys_p[i-1]   = 0;
    keys_n[i-1]   = 0;
    if(si[i-1] > 0){
      keys_p[i-1] = n[i].k;
      keys_n[i-1] = dummy;
      n[i].c      = 1;
    } else {
      keys_p[i-1] = dummy;
      keys_n[i-1] = n[i].k;	   
      n[i].c      = 2;
    }
  
    n[i].print();
  }		  

  //--- Recursively split the group of positive eigenvector nodes
  splitP(Bg, keys_p, dummy, tol);
  
  //--- Recursively split the group of negative eigenvector nodes
  splitN(Bg, keys_n, dummy, tol);
  
  
}

/*
 Utility method used by the Spectral method fine-tune an initial 
 given community split. 
 */
void modifySplit( double tol, int countmax ){

  double qmax  = 0;
  double qold  = 0;
  int count    = 0;
  int Ng       = si.size();

  visited.resize(Ng);

  VecDoub Gsi(Ng);
  MatDoub GSI(Ng,2);

  for(int i=0; i<Ng; i++){
    Gsi[i]    = si[i];
    GSI[i][0] = SI[i][0];
    GSI[i][1] = SI[i][1];
  }

  maxModularity( qmax );

  while( count < countmax ){

    if( qmax > qold ){
      for(int i=0; i<Ng; i++){
	Gsi[i]    = si[i];
	GSI[i][0] = SI[i][0];
	GSI[i][1] = SI[i][1];
      }
    }

    qold = qmax;
    qmax = 0.0;

    maxModularity(qmax);

    count++;    

  }

  for(int i=0; i<Ng; i++){
    si[i]    = Gsi[i];
    SI[i][0] = GSI[i][0];
    SI[i][1] = GSI[i][1];
  }

}

/*
 Utility method used by the Spectral method to find 
 which node, when moved gives the maximum change in the 
 Modularity value.
 */
void maxModularity(double &qmax){

  int N = si.size();

  VecDoub qstored(N);
  for(int i=0; i<N; i++)
    qstored[i]     = 0;
  
  
  double Q = 0.0;
     
  for(int k=0; k<N; k++){
      
    if( visited[k] < 1 ){

      Q          = 0.0;
      
      deltaModularityMax( k, Q );      
      
      qstored[k] = Q;
      	      
    }
  }

  qmax        =  0;//qstored(0);
  int ind_max =  -1;//0; 
  for(int i=0; i<N; i++){
    
    if( qstored[i] > qmax ){
      qmax    = qstored[i];
      ind_max = i; 
    }

  }
  
  
  for(int i=0; i<N; i++){
    
    if( i != ind_max )
      ;
    else {
      visited[i] = 1;
      if( si[i] == 1 ){
	si[i] = -1;
	SI[i][0] = 0; SI[i][1] = 1;
      } else {
	si[i] = 1;
	SI[i][0] = 1; SI[i][1] = 0;
      }
    }    
  }
  
  
}

/*
 Calculate the split of nodes belonging to the last group of nodes
 with positive eigenvector values.
 */
void splitP( MatDoub Bg, VecInt keys, int dummy, double tol ){

  cout << "> In splitP method... " << endl;

  int N = Bg.nrows();

  MatDoub Bgii(N,N);
  MatDoub Bgiii(N,N);
  VecInt  keysi_p (N);

  //--- Starting from the group Modularity matrix Bg,
  //--- resize matrices: Bgi, keysi_p, keysi_n, u and betai.
  Bgiii  = Bg;
  int Ng = 0;

  for(int i=0; i<keys.size(); i++){
    if(keys[i] != dummy){
      Ng++;
    }else{
      for(int k=0; k<Bgiii.nrows(); k++){
	Bgiii[k][i] = dummy;
	Bgiii[i][k] = dummy;
      }
    }
  }

  keysi_p.resize(Ng);
  VecInt keysi_n(Ng);
  
  int k=0;
  for(int i=0; i<keys.size(); i++){
    if(keys[i] != dummy)
      keysi_p[k++] = keys[i];
  }

  Bgii.resize(Ng,Ng);
  removeMatrixRow(Bgiii,Bgii);	

  Bgi.resize(Bgii.nrows(),Bgii.nrows());
  
  //--- Calculate the Modularity matrix Bgi for the new node group
  calculateB(Bgii, Bgi);

  u.resize(Ng,Ng);
  betai.resize(Ng);

  //--- Calculate eigenvectors, and values, from Bgi...
  calculateEigenVectors();
	  
  int ind = 0;
  findLeadingEigenVectors(ind);

  //--- Check that maximum eigenvalue is greater than the tolerance. 
  cout << "> max EigenValue is " << betai[ind] << " with ind " << ind << endl;
  if( betai[ind] > tol ){
	
    //--- set up the index vectors, si and SI, for the initial split
    maximiseIndexVectors(ind);

    double deltaQ_old = 0.0;
    double deltaQ_new = 0.0;

    int cp = 0;
    int cn = 0;
    
    //--- Calculate the Spectral Modularity
    deltaModularity(deltaQ_old);
    cout << "> Spectral Q: " << deltaQ_old << endl;

    double diff = fabs(deltaQ_old);
    int count   = 0;

    //--- Fine tuning stage to maximum deltaModularity for the initial split
    while( diff > tol ){

      modifySplit( tol, Ng );

      deltaModularity(deltaQ_new);
      cout << "> Modified Q: " << deltaQ_new << endl;
    
      diff = fabs( deltaQ_new - deltaQ_old ); 
    
      deltaQ_old = deltaQ_new;

    }

    //--- Keep recorded of maximum fine-tuned Modularity value.
    specQ += deltaQ_old;
    for(int i=0; i<Ng; i++){
      if(si[i] > 0) cp++;
      else          cn++;
    }
     
    if(cp < 1 || cn < 1) { cout << "> Stop splitting. " << endl; return; }

    int Ncomp = maxCommunity() + 1;
    int Ncomn = Ncomp + 1;
      
    cout << "> node list " << endl;
    for(int i=0; i<keysi_p.size(); i++){
      if( si[i] > 0 ){
	keysi_p[i] = (int)keysi_p[i];
	keysi_n[i] = dummy;
	n[(int)keysi_p[i]].c = Ncomp;
	cout << "> Node: " << keysi_p[i] << " c = " << n[(int)keysi_p[i]].c << endl;
      } else {
	keysi_n[i] = (int)keysi_p[i];
	keysi_p[i] = dummy;
	cout << "> Node: " << keysi_n[i] << " c = " << n[(int)keysi_n[i]].c << endl;
      }
    }
     
    //--- Recursively split the group of positive eigenvector nodes
    splitP(Bgii, keysi_p, dummy, tol);

    //--- Recursively split the group of negative eigenvector nodes    
    splitN(Bgii, keysi_n, dummy, tol);
      
  } else {
    cout << "> Stop splitting. " << endl; return;
  }
   
}

/*
 Calculate the split of nodes belonging to the last group of nodes
 with negative eigenvector values.
 */
void splitN(MatDoub Bg, VecInt keys, int dummy, double tol){

  cout << "> In splitN method... " << endl;

  int N = Bg.nrows();
  MatDoub Bgii(N,N);
  MatDoub Bgiii(N,N);
  VecInt  keysi_n (N);

  //--- Starting from the group Modularity matrix Bg,
  //--- resize matrices: Bgi, keysi_p, keysi_n, u and betai.
  Bgiii  = Bg;
  int Ng = 0;

  for(int i=0; i<keys.size(); i++){
    if(keys[i] != dummy){
      Ng++;
    }else{
      for(int k=0; k<Bgiii.nrows(); k++){
	Bgiii[i][k] = dummy;
	Bgiii[k][i] = dummy;
      }
    }
  }
  
  keysi_n.resize(Ng);
  VecInt keysi_p(Ng);

  int k=0;
  for(int i=0; i<keys.size(); i++){
    if(keys[i] != dummy)
      keysi_n[k++] = keys[i];
  }
  
  Bgii.resize(Ng,Ng);
  removeMatrixRow(Bgiii,Bgii);	

  Bgi.resize(Bgii.nrows(),Bgii.nrows());

  //--- Calculate the Modularity matrix Bgi for the new node group
  calculateB(Bgii, Bgi);

  u.resize(Ng,Ng);
  betai.resize(Ng);
  
  //--- Calculate eigenvectors, and values, from Bgi...
  calculateEigenVectors();
	  
  int ind = 0;
  findLeadingEigenVectors(ind);

  //--- Check that maximum eigenvalue is greater than the tolerance. 
  cout << "> max EigenValue is " << betai[ind] << " with ind " << ind << endl;
  if(betai[ind] > tol ){	

    //--- set up the index vectors, si and SI, for the initial split
    maximiseIndexVectors(ind);

    double deltaQ_old = 0.0; 
    double deltaQ_new = 0.0; 

    int cp = 0;
    int cn = 0;

    //--- Calculate the Spectral Modularity
    deltaModularity(deltaQ_old);
    cout << "> Spectral Q: " << deltaQ_old << endl;

    double diff = fabs(deltaQ_old);
    int count   = 0;

    //--- Fine tuning stage to maximum deltaModularity for the initial split
    while( diff > tol ){
    
      modifySplit( tol, Ng );
    
      deltaModularity(deltaQ_new);
      cout << "> Modified Q: " << deltaQ_new << endl;

      diff = fabs( deltaQ_new - deltaQ_old ); 
    
      deltaQ_old = deltaQ_new;

    }

    //--- Keep recorded of maximum fine-tuned Modularity value.
    specQ += deltaQ_old;
    for(int i=0; i<Ng; i++){
      si[i] = si[i];
      if(si[i] > 0) cp++;
      else          cn++;
    }

    if(cp < 1 || cn < 1){ cout << "> Stop splitting. " << endl; return; }
      
    int Ncomn = maxCommunity() + 1;
    int Ncomp = Ncomn + 1;

    cout << "> node list " << endl;
    for(int i=0; i<keysi_n.size(); i++){
      if( si[i] < 0){
	keysi_n[i] = keysi_n[i];
	keysi_p[i] = dummy;
	n[(int)keysi_n[i]].c = Ncomn;
	cout << "> Node: " << keysi_n[i] << " c = " << n[(int)keysi_n[i]].c << endl;
      } else {
	keysi_p[i] = keysi_n[i];	
	keysi_n[i] = dummy;	
	cout << "> Node: " << keysi_p[i] << " c = " << n[(int)keysi_p[i]].c << endl;
      }
    }

    //--- Recursively split the group of positive eigenvector nodes
    splitP(Bgii, keysi_p, dummy, tol);

    //--- Recursively split the group of negative eigenvector nodes        
    splitN(Bgii, keysi_n, dummy, tol);

  } else {
    cout << "> Stop splitting. " << endl; return ;
  }

   
}

/*
 Utility method find maximum community within the 
 network.
 */
int maxCommunity(){

  int Ncom = n[1].c;

  for(int i=1; i<n.size(); i++){
    if(n[i].c > Ncom) Ncom = n[i].c;
  }

  return Ncom;

}

/*
 Kronecker-delta function
 */
int delta( int i, int j){

  if ( i == j ) return 1;

  return 0;

}

/*
 Calculte the Modularity matrix when split 
 into more than two communities, see [2]
 in method declarations above.
 */
void calculateB(MatDoub B, MatDoub &Bg){

  int Ng = B.ncols();

  if( Bg.ncols() != Ng )
    Bg.resize(Ng,Ng);

  for(int i=0; i<Ng; i++){
    for(int j=0; j<Ng; j++){
      double sum = 0.0;
      for(int k=0; k<Ng; k++)
	sum += B[i][k];
      
      Bg[i][j] = B[i][j] -1.0 * delta(i,j) * sum;
    }
  }

}

/*
 Calculate the eigenvalues, betai, and eigenvectors, u, for 
 the current Modularity matrix Bgi.
 */
void calculateEigenVectors(){

  int Ng = Bgi.ncols();

  if(u.ncols() != Ng){
    u.resize(Ng,Ng);
    betai.resize(Ng);
  }

  u.resize(Ng,Ng);
  betai.resize(Ng);

  Symmeig h(Bgi, true);
  
  for(int i=0; i<Ng; i++){
    betai[i] = h.d[i];
    for(int j=0; j<Ng; j++){
      u[j][i] = h.z[j][i];
    }
  }

 
}


/*
 Find the leading eigenvector, i.e. 
 the one which corresponds to the most positive
 eigenvalue.
 */
void findLeadingEigenVectors(int &ind){

  
  int Ng = Bgi.ncols();

  int ind_max = 0;
  int ind_min = 0;

  double max  = betai[ind_max];
  double min  = betai[ind_min];

  for(int i=0; i<Ng-1; i++){
    if( betai[i] > max ){
      max     = betai[i];
      ind_max = i;
    } 
  }

  ind = ind_max;

}

/*
 Update the index vectors, si and SI, for each node in the 
 current split such that:

 si(i) =  1 if eigenvector_max(i) > 0
       = -1 if eigenvector_max(i) < 0

 SI(i,0) = 1        
 SI(i,1) = 0 if eigenvector_max(i) > 0
         = 0
         = 1 if eigenvector_max(i) < 0
 */
void maximiseIndexVectors( int ind ){

  int Ng = u.ncols();

  si.resize(Ng);
  
  SI.resize(Ng,2);

  for(int i=0; i<Ng; i++){

    if(u[i][ind] < 0){
      si[i]    = -1;
      SI[i][0] =  0;
      SI[i][1] =  1;
    } else {
      si[i]    =  1;
      SI[i][0] =  1;
      SI[i][1] =  0;
    }

  }

}
