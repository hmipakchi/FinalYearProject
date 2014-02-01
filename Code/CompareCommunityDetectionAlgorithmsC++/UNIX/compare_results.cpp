//
//  main.cpp
//  XCode
//
//  Created by Hesam Ipakchi on 01/02/2014.
//  Copyright (c) 2014 Hesam Ipakchi. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "utility.h"

using namespace std;

vector<vector<int> > readGroundTruthAssignments(const string& groundTruthAssignmentsFilename);

vector<vector<int> > readAssignmentsFromLouvainTree(const string& louvainTreeFilename, const int& noNodes, const int& noCommunities);

int main(int argc, const char * argv[])
{
    string groundTruthAssignmentsFilename = "test-network-sbm-assignments.txt";
    string louvainTreeFilename = "test-network-sbm-louvain.tree";
    int noNodes, noGTCommunities, noLouCommunities;
    noNodes = 150;
    noLouCommunities = 4;
    
    vector<vector<int> > groundTruthAssignments = readGroundTruthAssignments(groundTruthAssignmentsFilename);
    cout << "ground truth assignments:" << endl << groundTruthAssignments << endl;
    
    vector<vector<int> > louvainAssignments = readAssignmentsFromLouvainTree(louvainTreeFilename, noNodes, noLouCommunities);
    cout << "louvain assignments:" << endl << louvainAssignments << endl;
    
    groundTruthAssignments.clear();
    louvainAssignments.clear();
    
    return 0;
}

vector<vector<int> > readGroundTruthAssignments(const string& groundTruthAssignmentsFilename) {
    ifstream fInp;
    fInp.open(groundTruthAssignmentsFilename.c_str());
    if (fInp.is_open()) {
        vector<vector<int> > groundTruthAssignments;
        int noNodes, noCommunities;
        fInp >> noNodes >> noCommunities;
        for (int i = 0; i < noNodes; i++) {
            int nodeId, communityId;
            fInp >> nodeId >> communityId;
            if (communityId >= noCommunities) {
                string message = "readGroundTruthAssignments: considering nodeId: " + convertIntToString(nodeId) + ", the communityId = " + convertIntToString(communityId) + ", which is above the noCommunities(" + convertIntToString(noCommunities) + ")!!!";
                throw message.c_str();
            }
            vector<int> nodes;
            if ((int) groundTruthAssignments.size() <= communityId) {
                nodes.push_back(nodeId);
                groundTruthAssignments.push_back(nodes);
            }
            else {
                nodes = groundTruthAssignments.at(communityId);
                nodes.push_back(nodeId);
                groundTruthAssignments.at(communityId) = nodes;
            }
        }
        fInp.close();
        return groundTruthAssignments;
    }
    else {
        string message = "readGroundTruthAssignments: error opening file: " + groundTruthAssignmentsFilename;
        throw message.c_str();
    }
}

vector<vector<int> > readAssignmentsFromLouvainTree(const string& louvainTreeFilename, const int& noNodes, const int& noCommunities) {
    ifstream fInp;
    fInp.open(louvainTreeFilename.c_str());
    if (fInp.is_open()) {
        vector<vector<int> > louvainAssignments;
        for (int i = 0; i < noNodes; i++) {
            int nodeId, communityId;
            fInp >> nodeId >> communityId;
            if (communityId >= noCommunities) {
                string message = "readAssignmentsFromLouvainTree: considering nodeId: " + convertIntToString(nodeId) + ", the communityId = " + convertIntToString(communityId) + ", which is above the noCommunities(" + convertIntToString(noCommunities) + ")!!!";
                throw message.c_str();
            }
            vector<int> nodes;
            if ((int) louvainAssignments.size() <= communityId) {
                nodes.push_back(nodeId);
                louvainAssignments.push_back(nodes);
            }
            else {
                nodes = louvainAssignments.at(communityId);
                nodes.push_back(nodeId);
                louvainAssignments.at(communityId) = nodes;
            }
        }
        fInp.close();
        return louvainAssignments;
    }
    else {
        string message = "readAssignmentsFromLouvainTree: error opening file: " + louvainTreeFilename;
        throw message.c_str();
    }
}