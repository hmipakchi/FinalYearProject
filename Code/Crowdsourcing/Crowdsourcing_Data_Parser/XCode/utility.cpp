#include "utility.h"

/* Utility methods */

string convertIntToString(const int& x) {
    stringstream x_ss;
    x_ss << x;
    return x_ss.str();
}