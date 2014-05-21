//
//  cluster.h
//  k-means
//
//  Created by callen on 21/10/13.
//  Copyright (c) 2013 callen. All rights reserved.
//

#ifndef k_means_cluster_h
#define k_means_cluster_h


#include "TVector.h"
#include <fstream>
#include <time.h>

#define noIterations 20

class Cluster {
private:
    int k;
    vector<TVector> mu;
    vector<TVector> indicator;
    double error;
public:
    Cluster();
    Cluster(const int& k);
    ~Cluster();
    double fRand(double fMin, double fMax);
    TVector getSampleMean(vector<TVector> x);
    void runKMeansClustering(vector<TVector> x, double bound);
    double getError() const;
    vector<TVector> getIndicator() const;
    vector<TVector> getMu() const;
    static void printIndicator(vector<TVector> exampleIndicator, int noClusters);
    static void writeIndicatorToFile(string filename, vector<TVector> exampleIndicator, int noClusters);
};

#endif
