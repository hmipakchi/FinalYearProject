#ifndef utility_h
#define utility_h

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <time.h>
#include <cctype>
#include <algorithm>

using namespace std;


string convertIntToString(const int& x);

string convertFloatToString(const float& x);

string convertDoubleToString(const double& x);

template <typename T>
ostream& operator<<(ostream& os, vector<T> const& data) {
    typename vector<T>::const_iterator it = data.begin();
    int i = 0;
    for (; it != data.end(); it++) {
        os << "[" << i << "] -> " << *it << endl;
        i++;
    }
    return os;
}
        
#endif