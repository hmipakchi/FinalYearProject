#if !defined(HELPER_INCLUDED)
#define HELPER_INCLUDED

class Helper {

 public:
  Helper();
  ~Helper();

  int rBinarySearch(vector<int> sortedVector, int first, int last, int key);
  int rBinarySearch(vector<node> sortedVector, int first, int last, int key);
  int rBinarySearch(vector<edge> sortedVector, int first, int last, int key);

  vector<edge> cloneEdgeList(vector<edge> evec);
  vector<node> cloneNodeList(vector<node> nvec);

};
  
Helper::Helper(){}

Helper::~Helper(){}

int Helper::rBinarySearch(vector<int> sortedVector, int first, int last, int key) {
   // function:
   //   Searches sortedVector[first]..sortedVector[last] for key.  
   // returns: index of the matching element if it finds key, 
   //         otherwise  -(index where it could be inserted)-1.
   // parameters:
   //   sortedVector in  array of sorted (ascending) values.
   //   first, last in  lower and upper subscript bounds
   //   key         in  value to search for.
   // returns:
   //   index of key, or -insertion_position -1 
   //                 if key is not in the array.
   
   if (first <= last) {
       int mid = (first + last) / 2;  // compute mid point.
       if (key == sortedVector[mid]) 
           return mid;   // found it.
       else if (key < sortedVector[mid]) 
           // Call ourself for the lower part of the array
           return rBinarySearch(sortedVector, first, mid-1, key);
       else
           // Call ourself for the upper part of the array
           return rBinarySearch(sortedVector, mid+1, last, key);
   }
   return -(first + 1);    // failed to find key
}


int Helper::rBinarySearch(vector<node> sortedVector, int first, int last, int key) {
   
   if (first <= last) {
       int mid = (first + last) / 2;  // compute mid point.
       if (key == sortedVector[mid].k) 
           return mid;   // found it.
       else if (key < sortedVector[mid].k) 
           // Call ourself for the lower part of the array
           return rBinarySearch(sortedVector, first, mid-1, key);
       else
           // Call ourself for the upper part of the array
           return rBinarySearch(sortedVector, mid+1, last, key);
   }
   return -(first + 1);    // failed to find key
}


int Helper::rBinarySearch(vector<edge> sortedVector, int first, int last, int key) {
   
   if (first <= last) {
       int mid = (first + last) / 2;  // compute mid point.
       if (key == sortedVector[mid].key) 
           return mid;   // found it.
       else if (key < sortedVector[mid].key) 
           // Call ourself for the lower part of the array
           return rBinarySearch(sortedVector, first, mid-1, key);
       else
           // Call ourself for the upper part of the array
           return rBinarySearch(sortedVector, mid+1, last, key);
   }
   return -(first + 1);    // failed to find key
}


vector<edge> Helper::cloneEdgeList(vector<edge> evec){

  vector<edge> clone;

  clone.push_back(edge(-1,-1,-1,-1));
  for(int i=0; i<evec.size(); i++)
    clone.push_back( evec[i].Clone());

  return clone;

}

vector<node> Helper::cloneNodeList(vector<node> nvec){

  vector<node> clone;

  for(int i=0; i<nvec.size(); i++){
    clone.push_back(nvec[i].Clone());
  }

  return clone;
}


#endif
