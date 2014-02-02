#if !defined(READINPUTFILE_INCLUDED)
#define READINPUTFILE_INCLUDED

void removeDuplicates(std::vector<int>& vec)
{
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

class readInputFile {

 private: 
  // temporary variables for this function
  int s,f,t;
  double w;
  string Sso, Ssi, St;

  node *newnode;
  edge current;	    

  int newNodeIndexso;
  int newNodeIndexsi;
  int index;
    
  vector<node> ni;
  vector<node> selfInterNodesi;
  vector<edge> elisti;

  vector<node> temp;

  vector<int> old_s;
  vector<int> old_f;
  vector<double> old_we;  

  vector<string> old_Ss;
  vector<string> old_Sf;

  vector<string> node_names;
  vector<int>    node_ids;

  vector<node> emptyn;
  vector<edge> emptye;
  vector<int>  emptyi;
  vector<string> emptys;

  Helper ihelper;

 public:
  readInputFile();
  ~readInputFile();

  bool areDigits   ( string _str );
  int  print_header( char *message, int Ncols );
  void readFile    ( const char *file_network, int Ncols );
 
  void readNodelist( const char *nodelist );
  void readEdgelist( const char *edgelist );
 
  void enumerateNodeList( vector<node> nj, vector<edge> ej, vector<node> &Rnj, vector<edge> &Rej );
 
  vector<node> getNodeSet();
  vector<edge> getEdgeSet();

};

readInputFile::readInputFile(){}


/*
  Utility method returns true if a string contains only 
  integers from 0 to 9.
 */
bool readInputFile::areDigits( string _str ){

  for (int i=0; i<_str.length(); i++)
    {
      if( _str[i]>='0' && _str[i]<='9'){;}
      else
	return false;
          
    return true;
    }
}


/*
 Utility method to read the files head line, and the number 
 of data columns in the file.
*/
int readInputFile::print_header( char *message, int Ncols){

  char *tokens = strtok(message, "\t\r"); 
  vector<string> columns;     
  while(tokens != NULL){
    columns.push_back( string(tokens) );       
    tokens = strtok(NULL, "\t\r");     
  }

  if( columns.size() == 2 ){

    if( areDigits(columns[0]) && areDigits(columns[1]) ){
      old_s.push_back((int)atoi(columns[0].c_str()));
      old_f.push_back((int)atoi(columns[1].c_str()));
      old_we.push_back(1);
      return 2;
    }

  }

  if( columns.size() == 3 ){

    if( areDigits(columns[0]) && areDigits(columns[1]) ){
      old_s.push_back((int)atoi(columns[0].c_str()));
      old_f.push_back((int)atoi(columns[1].c_str()));
      if( Ncols == 3 )
	old_we.push_back((double)atof(columns[2].c_str()));
      else
	old_we.push_back(1);
      return 3;
    }

  }

  cout << "> Header: " << message << endl;
  return -1;

}

/*
 Utility method to read create a node list given the
 attributes: key, degree and inputted ID.
*/
void readInputFile::readNodelist(const char *nodelist){

  char comments[256];
  ifstream fscan(nodelist, ios::in);
  fscan.getline(comments, 256);

  int deg, id, ID;

  while (fscan >> deg >> id >> ID) {		

    node *n = new node(id, ID, deg);
    ni.push_back(*n);
    
  }
  
  fscan.close();

}

/*
 Utility method to read create a edge list given the
 attributes: origin node (so), termination node(si), 
 unique key, inputted origin ID (idso) and inputted 
 termination ID (idsi).
 */
void readInputFile::readEdgelist(const char *edgelist){

  char comments[256];
  ifstream fscan(edgelist, ios::in);
  fscan.getline(comments, 256);

  int so, idso, si, idsi, key;

  while (fscan >> so >> idso >> si >> idsi >> key){

    edge *e = new edge(so, si, 1.0, key, idso, idsi);
    elisti.push_back(*e);
    
  }
  
  fscan.close();    

}

/*
 Read in network file which can be weighted or unweighted; 
 determined by the parameter Ncols
 Ncols = 2 => unweighted
 Ncols = 3 => weighted 
 File input file can have one header line at the begining,
 which is detected, or not.
 */
void readInputFile::readFile(const char *file_network, int Ncols ){

  // First scan through the input file to discover the largest node id. We need to
  // do this so that we can allocate a properly sized array for the sparse matrix
  // representation.

  int foundCols = -1;
  char comments[256];
  ifstream fscan(file_network, ios::in);
   
  if( fscan.is_open() != true ){
    cout << "> Error opening " << file_network << endl;
    exit(1);
  }

  cout << "> Scanning file_network:" << file_network << endl;
  
  // The network file can have a Header line, only one placed at the begining of the file. 
  fscan.getline(comments, 256);
  foundCols = print_header(comments, Ncols);
  if( foundCols == -1 ){
    fscan.getline(comments, 256);
    foundCols = print_header(comments, Ncols);
  } else {
    cout << "> Header: None " << endl;
  }

  int count = 1;
  
  if(Ncols == 2){
 
    if(foundCols == 2){

      while (fscan >> s >> f ) {	        // read friendship pair (s,f)
	if (f < s) { t = s; s = f; f = t; }	// guarantee s < f
	
	old_s.push_back(s);
	old_f.push_back(f);
	old_we.push_back(1);
      }
    } else {

      while (fscan >> s >> f >> w ) {	        // read friendship pair (s,f)
	if (f < s) { t = s; s = f; f = t; }	// guarantee s < f
      
	old_s.push_back(s);
	old_f.push_back(f);
	old_we.push_back(1);
      }	 
    }
     
    fscan.close();

    for(int i=0; i < old_s.size(); ++i){
	
      for(int j=i+1; j < old_s.size(); ++j){
	
	if( old_s[j] == old_s[i] && old_f[j] == old_f[i] )
	  {old_s[j] = -1; old_f[j] = -1; old_we[j] = -1;}
      }
       
      if(old_s[i] != -1 && old_f[i] != -1){
	if(old_we[i] == 0) old_we[i] = 1;
	elisti.push_back(edge(old_s[i], old_f[i], old_we[i], count++));
	node_ids.push_back( old_s[i] );
	node_ids.push_back( old_f[i] );
      } 
    }
  }
    
  if(Ncols == 3){
       
    if(foundCols == 2 ){

      while (fscan >> s >> f ) {		// read friendship pair (s,f)
	if (f < s) { t = s; s = f; f = t; }	// guarantee s < f
	
	old_s.push_back(s);
	old_f.push_back(f);
	old_we.push_back(1);
      }
      
    } else {
      
      while (fscan >> s >> f >> w ) {	        // read friendship pair (s,f)
	if (f < s) { t = s; s = f; f = t; }	// guarantee s < f
      
	old_s.push_back(s);
	old_f.push_back(f);
	old_we.push_back(w);
      }	 
      
    }

    fscan.close();
    
 
    for(int i=0; i < old_s.size(); ++i){
      
      for(int j=i+1; j < old_s.size(); ++j){
	
	if( old_s[j] == old_s[i] && old_f[j] == old_f[i] )
	  {old_s[j] = -1; old_f[j] = -1; old_we[j] = -1;}
      }
      
      if(old_s[i] != -1 && old_f[i] != -1){
	if(old_we[i] == 0) old_we[i] = 1;
	elisti.push_back(edge(old_s[i], old_f[i], old_we[i], count++));
	node_ids.push_back( old_s[i] );
	node_ids.push_back( old_f[i] );
      }
      
    }
  }
   
   count = 1;
   removeDuplicates(node_ids);
   vector<int>  tally;
   for(int i=0; i<node_ids.size(); i++){
     tally.push_back( count );
     ni.push_back( node( count, 0.0, 0.0, node_ids[i]) );
     count++;
   }
     
   ni.insert(ni.begin(), node(-1, 0.0, 0.0, -1) );	      
   for(int i=0; i<elisti.size(); i++){
     
     int index_so = -1;
     int index_si = -1;
     
     if( elisti[i].IDso == 0 ){
       index_so       = ihelper.rBinarySearch( node_ids, 0, node_ids.size(), elisti[i].so );
       int temp_so    = elisti[i].so;
       elisti[i].IDso = temp_so;
       elisti[i].so   = tally[index_so];
     }
	
     if( elisti[i].IDsi == 0 ){
       index_si       = ihelper.rBinarySearch( node_ids, 0, node_ids.size(), elisti[i].si );
       int temp_si    = elisti[i].si;
       elisti[i].IDsi = temp_si;
       elisti[i].si   = tally[index_si];
     }

   }
     

   for(int i=0; i<ni.size(); i++){

     for(int j=0; j<elisti.size(); j++){
       
       if( elisti[j].IDso == ni[i].ID || elisti[j].IDsi == ni[i].ID )
	 ni[i].addEdge( elisti[j].so, elisti[j].si, elisti[j].we, elisti[j].key, elisti[j].IDso, elisti[j].IDsi );
       
       }

   }

   old_s.clear();
   old_f.clear();
   old_we.clear();

   return;

}

/*
 Renumerates the node and edge list after randomly removing nodes from the boot-strap method. 
 */
void readInputFile::enumerateNodeList(vector<node> nj, vector<edge> ej, vector<node> &Rnj, vector<edge> &Rej){

  
  ni.clear();
  elisti.clear();

  Rnj.clear();
  Rej.clear();

  node_ids.clear();

  int count = 1;
  vector<int> tally;
  tally.clear();
  for(int i=0; i<nj.size(); i++){  
    if( nj[i].c != -1 ){
      node_ids.push_back( nj[i].ID );
      tally.push_back( count );
      ni.push_back( node( count, 0.0, 0.0, nj[i].ID ) );          
      count++;
    }
  }

  count = 1;
  ni.insert(ni.begin(), node(-1, 0.0, 0.0, -1) );	      
  for(int i=0; i<ej.size(); i++){

    edge current = ej[i];
    
    if( current.so != -1 && current.si != -1 ){
      
      int index_so   = -1;
      int index_si   = -1;
      
      index_so       = ihelper.rBinarySearch( node_ids, 0, node_ids.size(), current.IDso );
      current.so     = tally[index_so];
            
      index_si       = ihelper.rBinarySearch( node_ids, 0, node_ids.size(), current.IDsi );
      current.si     = tally[index_si];

      if( index_so != -1 && index_si != -1 )
	elisti.push_back( edge(current.so, current.si, current.Globalwe, count++, current.IDso, current.IDsi) );

    }
  }

  for(int i=0; i<ni.size(); i++){
    
    for(int j=0; j<elisti.size(); j++){
      
    if( elisti[j].IDso == ni[i].ID || elisti[j].IDsi == ni[i].ID )
      ni[i].addEdge( elisti[j].so, elisti[j].si, elisti[j].Globalwe, elisti[j].key, elisti[j].IDso, elisti[j].IDsi );
      
    }

  }

  Rnj = ni;
  Rej = elisti;
  
  ni.clear();
  elisti.clear();
  


}

readInputFile::~readInputFile(){}

//return the new node list after reading the input file
vector<node> readInputFile::getNodeSet(){ return ni; }

//return the new edge list after reading the input file
vector<edge> readInputFile::getEdgeSet(){ return elisti; }


#endif
