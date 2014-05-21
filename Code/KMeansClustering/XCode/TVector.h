//
//  TVector.h
//  k-means
//
//  Created by callen on 21/10/13.
//  Copyright (c) 2013 callen. All rights reserved.
//

#ifndef __k_means__TVector__
#define __k_means__TVector__

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <sstream>

using namespace std;

class TVector{
private:
    vector<double> data;
public:
    TVector();
    TVector(const int& n);
    TVector(const vector<double>& data);
    TVector(const TVector& tv);
    ~TVector();
    TVector& operator=(const TVector& tv);
    void setData(const vector<double>& data);
    void setIndicator(int pos, double b);
    double getIndicator(int pos);
    TVector operator+(const TVector& tv) const;
    TVector operator-(const TVector& tv) const;
    TVector operator*(const double& tv) const;
    TVector add(const TVector& tv) const;
    TVector sub(const TVector& tv) const;
    TVector multiply(const double& tv) const;
    double norm() const;
    double innerProduct(const TVector& tv) const;
    void normalise();
    int getDimension() const;
    double getMin() const;
    double getMax() const;
    double getMean() const;
    string toString() const;
    friend ostream& operator<<(ostream& o, const TVector& tv);
};


#endif /* defined(__k_means__TVector__) */
