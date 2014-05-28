#include "utility.h"

string convertIntToString(const int& x) {
    stringstream x_ss;
    x_ss << x;
    return x_ss.str();
}

string convertFloatToString(const float& x) {
    stringstream x_ss;
    x_ss << x;
    return x_ss.str();
}

string convertDoubleToString(const double& x) {
    stringstream x_ss;
    x_ss << x;
    return x_ss.str();
}