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

vector<vector<int> > readAssignmentsFromLouvainTree(const string& louvainAssignmentsFilename, const int& noNodes, const int& noCommunities);

vector<vector<int> > readAssignmentsFromNewman(const string& newmanAssignmentsFilename, const int& noNodes, const int& noCommunities);

int main(int argc, const char * argv[])
{
    string groundTruthAssignmentsFilename = "testData/test-network-sbm-assignments.txt";
    string louvainAssignmentsFilename = "testData/test-network-sbm-louvain.tree";
    string newmanAssignmentsFilename = "testData/communities_newman.txt";
    
    int noNodes = 150;
    int noLouvainCommunities = 4;
    int noNewmanCommunities = 5;
    
    vector<vector<int> > groundTruthAssignments = readGroundTruthAssignments(groundTruthAssignmentsFilename);
    cout << "ground truth assignments:" << endl << groundTruthAssignments << endl;
    
    vector<vector<int> > louvainAssignments = readAssignmentsFromLouvainTree(louvainAssignmentsFilename, noNodes, noLouvainCommunities);
    cout << "louvain assignments:" << endl << louvainAssignments << endl;
    
    vector<vector<int> > newmanAssignments = readAssignmentsFromNewman(newmanAssignmentsFilename, noNodes, noNewmanCommunities);
    cout << "newman assignments:" << endl << newmanAssignments << endl;
    
    groundTruthAssignments.clear();
    louvainAssignments.clear();
    newmanAssignments.clear();
    
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

vector<vector<int> > readAssignmentsFromLouvainTree(const string& louvainAssignmentsFilename, const int& noNodes, const int& noCommunities) {
    ifstream fInp;
    fInp.open(louvainAssignmentsFilename.c_str());
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
        string message = "readAssignmentsFromLouvainTree: error opening file: " + louvainAssignmentsFilename;
        throw message.c_str();
    }
}

vector<vector<int> > readAssignmentsFromNewman(const string& newmanAssignmentsFilename, const int& noNodes, const int& noCommunities) {
    ifstream fInp;
    fInp.open(newmanAssignmentsFilename.c_str());
    if (fInp.is_open()) {
        vector<vector<int> > newmanAssignments;
        string uselessString;
        // read in "communities"
        fInp >> uselessString;
        for (int i = 0; i < noNodes; i++) {
            int nodeId, communityId;
            // read in "=" inbetween nodeId and communityId
            fInp >> nodeId >> uselessString >> communityId;
            // newman communities tart from 1 not 0!
            communityId--;
            if (communityId >= noCommunities) {
                string message = "readAssignmentsFromLouvainTree: considering nodeId: " + convertIntToString(nodeId) + ", the communityId = " + convertIntToString(communityId) + ", which is above the noCommunities(" + convertIntToString(noCommunities) + ")!!!";
                throw message.c_str();
            }
            vector<int> nodes;
            if ((int) newmanAssignments.size() <= communityId) {
                nodes.push_back(nodeId);
                newmanAssignments.push_back(nodes);
            }
            else {
                nodes = newmanAssignments.at(communityId);
                nodes.push_back(nodeId);
                newmanAssignments.at(communityId) = nodes;
            }
        }
        fInp.close();
        return newmanAssignments;
    }
    else {
        string message = "readAssignmentsFromNewman: error opening file: " + newmanAssignmentsFilename;
        throw message.c_str();
    }
}