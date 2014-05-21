//
//  main.cpp
//  k-means
//
//  Created by callen on 21/10/13.
//  Copyright (c) 2013 callen. All rights reserved.
//

#include "cluster.h"

#define bound 0.0001

#define dimension 80

using namespace std;

void readData(string filename, vector<TVector> &Xn, int noClusters);

void writeIndicatorsToFile(string outputIndicatorsFilename, vector<TVector> indicators);

template <typename T>
string convertTemplateTypenameToString(const T& x);

int main(int argc, const char * argv[])
{
    srand (time(NULL));
    
    int minNoClusters = 2;
    int maxNoClusters = 10;
    
    vector<TVector> bestIndicator;
    vector<TVector> bestClusterCenters;
    double bestError = (double) INT_MAX;
    double bestNoClusters = 0;
    
    // real-world data: modularity matrix
    string dataTextFilename = "/Users/hesamipakchi/Documents/MATLAB/FinalYearProject/data_files/financialNetworks/financialSpectralClustering_ModularityMatrix_Eigenvectors_top_" + convertTemplateTypenameToString(dimension) + ".dat";
    
    string outputIndicatorsFilename = "/Users/hesamipakchi/Documents/MATLAB/FinalYearProject/data_files/financialNetworks/financialSpectralClusteringOutputCommunities.dat";
    
    // real-world data: weighted adjacency matrix
//    string dataTextFilename = "/Users/hesamipakchi/Documents/MATLAB/FinalYearProject/data_files/financialNetworks/financialSpectralClustering_ModularityMatrix2_Eigenvectors_top_" + convertTemplateTypenameToString(dimension) + ".dat";
    
    // synthetic data
//    string dataTextFilename = "/Users/hesamipakchi/Documents/MATLAB/FinalYearProject/data_files/financialNetworks/syntheticCorrelationMatrices_ModularityMatrix_Eigenvectors_top_100.dat";
    
    for (int i = minNoClusters; i <= maxNoClusters; i++) {
        vector<TVector> Xn;
        Cluster kMean(i);
        readData(dataTextFilename,Xn,i);
        kMean.runKMeansClustering(Xn,bound);
        
        if (kMean.getError() < bestError) {
            bestError = kMean.getError();
            bestIndicator = kMean.getIndicator();
            bestClusterCenters = kMean.getMu();
            bestNoClusters = i;
        }
    }
    
    cout << endl << "Printing best found clusters..." << endl;
    Cluster::printIndicator(bestIndicator,bestNoClusters);
    
    Cluster::writeIndicatorToFile(outputIndicatorsFilename,bestIndicator,bestNoClusters);
    
    return 0;
}

void readData(string filename, vector<TVector> &Xn, int noClusters) {
    ifstream fInp;
    vector<double> tempV;
    TVector tempTV;
    fInp.open(filename);
    float tempData;
    float emptyData;
    string line, testline;
    bool eof = false;
    if (fInp.is_open() == true)
    {
        while (fInp.good() && !eof)
        {
            for (int i = 0; i < noClusters; i++) {
                fInp >> tempData;
                tempV.push_back(tempData);
            }
            for (int i = noClusters; i < dimension; i++) {
                fInp >> emptyData;
            }
            
            tempTV.setData(tempV);
            Xn.push_back(tempTV);
            tempV.clear();
        }
        fInp.close();
    }
    else {
        cout << "ERROR: reading data from file: " << filename << " has failed..." << endl;
    }
}

template <typename T>
string convertTemplateTypenameToString(const T& x) {
    stringstream x_ss;
    x_ss << x;
    return x_ss.str();
}