//
//  cluster.cpp
//  k-means
//
//  Created by callen on 21/10/13.
//  Copyright (c) 2013 callen. All rights reserved.
//

#include <stdio.h>
#include "cluster.h"

using namespace std;

Cluster::Cluster() {
    
}

Cluster::Cluster(const int& k){
    this->k = k;
    
}

Cluster::~Cluster() {
    mu.clear();
    indicator.clear();
}

double Cluster::fRand(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

TVector Cluster::getSampleMean(vector<TVector> x) {
//    int noDataPoints = (int) x.size();
//    int noSamples = floor(noDataPoints / this->k);
//    vector<TVector> sampledData;
//    TVector meanVector = TVector(this->k);
//    for (int i = 0; i < noSamples; i++) {
//        int sample = rand() % noDataPoints;
//        sampledData.push_back(x[sample]);
//    }
//    for (int s = 0; s < noSamples; s++) {
//        meanVector = meanVector + sampledData[s];
//    }
//    meanVector = meanVector * (1/ (double) noSamples);
//    return meanVector;
    
    int noDataPoints = (int) x.size();
    int sample = rand() % noDataPoints;
    TVector sampledDataPoint = x[sample];
    return TVector(sampledDataPoint);
}

void Cluster::runKMeansClustering(vector<TVector> x, double bound) {
    for(int i = 0; i < this->k; i++) {
        mu.push_back(getSampleMean(x));
//        cout << "i = " << i << ", mu = " << mu[i] << endl;
    }
    
    for(int n = 0; n < x.size(); n++) {
        indicator.push_back(TVector(this->k));
    }
    
    for (int iter = 0; iter < noIterations; iter++) {
        // reset indicators to zero
        for (int nk = 0; nk < x.size(); nk++) {
            for (int j = 0; j < this->k; j++) {
                indicator[nk].setIndicator(j, 0);
            }
        }
        
        // keep Mu fixed and calucate new indicator's
        for (int n = 0; n < x.size(); n++){
//            cout << "n = " << n << ", x[n] = " << x[n] <<endl ;
            double norm = (double) INT_MAX;
            int index = -1;
            for (int j = 0; j < this->k; j++) {
                if(((x[n] - mu[j]).norm()) < norm)
                {
                    norm = (x[n] - mu[j]).norm();
                    index = j;
                }
            }
            indicator[n].setIndicator(index, 1);

//            cout << "###   n: " << n << "     indicator[n] << endl;
        }
        
        // keep indicators fixed and calucate new Mu's
        for (int K = 0; K < this->k ; K++) {
            TVector topV((int) x.size());
            double bot = 0;
            for(int n = 0; n < x.size(); n++) {
                topV = topV + (x[n] * indicator[n].getIndicator(K));
                bot += indicator[n].getIndicator(K);
            }
            if (bot != 0) {
                mu[K] = topV * (1/bot);
            }
            else {
                mu[K] = getSampleMean(x);
                if (iter == noIterations-1) {
                    cout << "*****     in iteration: (" << iter << "), for cluster: " << K << ", bot = 0!!!" << endl;
                }
            }
//            cout << "i = " << K << ", mu = " << mu[K] << endl;
        }
    }
    
    // calculate error function value
    double errorValue = 0.0;
    for (int n = 0; n < x.size(); n++) {
        for (int K = 0; K < this->k ; K++) {
            errorValue += (indicator[n].getIndicator(K))*pow((x[n] - mu[K]).norm(),2);
        }
    }
    this->error = errorValue;
    cout << "noClusters = " << this->k << ", error function value, J = " << this->error << endl;
}

double Cluster::getError() const {
    return error;
}

vector<TVector> Cluster::getIndicator() const {
    return indicator;
}

vector<TVector> Cluster::getMu() const {
    return mu;
}

void Cluster::printIndicator(vector<TVector> exampleIndicator, int noClusters) {
    for (int n = 0; n < exampleIndicator.size(); n++) {
        for (int cluster = 0; cluster < noClusters; cluster++) {
            if(exampleIndicator[n].getIndicator(cluster) == 1)
                cout << "n = " << n << ", cluster ID: " << cluster << endl;
        }
    }
}

void Cluster::writeIndicatorToFile(string filename, vector<TVector> exampleIndicator, int noClusters) {
    ofstream fOut;
    fOut.open(filename.c_str(), fstream::out);
    if (fOut.is_open()) {
        for (int n = 0; n < exampleIndicator.size(); n++) {
            for (int cluster = 0; cluster < noClusters; cluster++) {
                if(exampleIndicator[n].getIndicator(cluster) == 1)
                    fOut << (cluster+1) << endl;
            }
        }
        fOut.close();
    }
    else {
        cout << "ERROR: writing data to file: " << filename << " has failed..." << endl;
    }
}