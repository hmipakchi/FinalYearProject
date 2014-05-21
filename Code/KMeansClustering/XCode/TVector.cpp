//
//  TVector.cpp
//  k-means
//
//  Created by callen on 21/10/13.
//  Copyright (c) 2013 callen. All rights reserved.
//

#include "TVector.h"


TVector::TVector(){
    
}

TVector::TVector(const int& n) {
    for (int i = 0; i < n; i++) {
        data.push_back(0);
    }
}

TVector::TVector(const vector<double>& data){
    this->data = data;
}

TVector::TVector(const TVector& tv){
    data = tv.data;
}

TVector::~TVector(){
    data.clear();
}


TVector& TVector::operator=(const TVector& tv){
    if(this == &tv)
        return *this;
    else
        data = tv.data;
    return *this;
}

void TVector::setData(const vector<double>& data) {
    this -> data = data;
}

void TVector::setIndicator(int pos, double b) {
    data[pos] = b;
}

double TVector::getIndicator(int pos) {
    return data[pos];
}
TVector TVector::operator+(const TVector& tv) const {
    return add(tv);
}

TVector TVector::operator-(const TVector& tv) const {
    return sub(tv);
}

TVector TVector::operator*(const double& tv) const {
    return multiply(tv);
}

TVector TVector::add(const TVector& tv) const {
    vector<double> sum;
    for (int i = 0; i < tv.getDimension(); i++) {
        sum.push_back(data[i]+tv.data[i]);
    }
    return TVector(sum);
}

TVector TVector::sub(const TVector& tv) const {
    vector<double> diff;
    for (int i = 0; i < tv.getDimension(); i++) {
        diff.push_back(data[i]-tv.data[i]);
    }
    return TVector(diff);
}

TVector TVector::multiply(const double& tv) const {
    vector<double> product;
    for (int i = 0; i < this->getDimension(); i++) {
        product.push_back(data[i]*tv);
    }
    return TVector(product);
}

double TVector::norm() const {
    double norm2 = 0;
    for(vector<double>::const_iterator i = data.begin(); i != data.end(); i++) {
        norm2 += (*i) * (*i);
    }
    return sqrt(norm2);
}

double TVector::innerProduct(const TVector& tv) const {
    double innerProduct = 0;
    for (int i = 0; i < tv.getDimension(); i++) {
        innerProduct += data[i] * tv.data[i];
    }
    return innerProduct;
}

void TVector::normalise() {
    double norm2 = norm();
    for(vector<double>::iterator i = data.begin(); i != data.end(); i++) {
        *i = *i / norm2;
    }
}

int TVector::getDimension() const {
    return (int) data.size();
}

double TVector::getMin() const {
    double min = (double) INT_MAX;
    for(vector<double>::const_iterator i = data.begin(); i != data.end(); i++) {
        if ((*i) < min) {
            min = *i;
        }
    }
    return min;
}

double TVector::getMax() const {
    double max = (double) INT_MIN;
    for(vector<double>::const_iterator i = data.begin(); i != data.end(); i++) {
        if ((*i) > max) {
            max = *i;
        }
    }
    return max;
}

double TVector::getMean() const {
    double mean = 0;
    for(vector<double>::const_iterator i = data.begin(); i != data.end(); i++) {
        mean += (*i);
    }
    return mean / ((double) this->getDimension());
}


string TVector::toString() const {
    string s = "[";
    for(vector<double>::const_iterator i = data.begin(); i != data.end(); i++) {
        stringstream data_ss;
        data_ss << *i;
        if(i != data.end()-1)
            s += data_ss.str() + ", ";
        else
            s += data_ss.str();
    }
    s += "]";
    return s;
}

ostream& operator<<(ostream& o, const TVector& tv) {
    o << tv.toString();
    return o;
}