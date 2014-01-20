//
//  main.cpp
//  Yahoo_Finance_Data_Parser
//
//  Created by Hesam Ipakchi on 20/01/2014.
//  Copyright (c) 2014 Hesam Ipakchi. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& data) {
    typename vector<T>::const_iterator it = data.begin();
    int i = 0;
    for (; it != data.end(); it++) {
        os << "[" << i << "] -> " << *it << endl;
        i++;
    }
    return os;
}

vector<string> parseStockDataTickers(const string& inputTickerFilename);

vector<double> parseStockDataCSVFile(const string& inputDataFilename);

vector<vector<double> > parseAllStockDataCSVFile(const vector<string>& tickers);

void writeStockDataToFile(const vector<vector<double> >& allStockData, const string& outputDataFilename);

int main(int argc, const char * argv[])
{

    string inputTickerFilname = "tickers.txt";
    string outputDataFilename = "testOutputData.txt";
    
    vector<string> tickers = parseStockDataTickers(inputTickerFilname);
    
    vector<vector<double> >testData = parseAllStockDataCSVFile(tickers);
    
    writeStockDataToFile(testData, outputDataFilename);
    
    testData.clear();
    
    return 0;
}

vector<string> parseStockDataTickers(const string& inputTickerFilename) {
    vector<string> tickers;
    ifstream fInp;
    fInp.open(inputTickerFilename.c_str());
    if (fInp.is_open()) {
        string ticker;
        while (!fInp.eof()) {
            fInp >> ticker;
            tickers.push_back(ticker);
        }
        return tickers;
    }
    else {
        string message = "parseStockDataTickers: error opening file: " + inputTickerFilename;
        throw message.c_str();
    }
}

vector<double> parseStockDataCSVFile(const string& inputDataFilename) {
    vector<double> stockData;
    ifstream fInp;
    fInp.open(inputDataFilename.c_str());
    if (fInp.is_open()) {
        string text;
        string closePriceString;
        double closePrice;
        // determine position of close price within csv file
        int noColumnHeaders = 7;
        int noUnusedDataBefore = 3;
        int noUnusedDataAfter = 2;
        // remove unused information
        for (int i = 0; i < noColumnHeaders; i++) {
            getline ( fInp, text, ',' );
        }
        while (!fInp.eof()) {
            // remove unused information
            for (int i = 0; i < noUnusedDataBefore; i++) {
                getline ( fInp, text, ',' );
            }
            
            // extract close price of stock
            getline ( fInp, closePriceString, ',' );
            closePrice = atof(closePriceString.c_str());
            
            // add close price fo stock to data vector
            stockData.push_back(closePrice);
            
            // remove unused information
            for (int i = 0; i < noUnusedDataAfter; i++) {
                getline ( fInp, text, ',' );
            }
        }
        return stockData;
    }
    else {
        string message = "parseStockDataCSVFile: error opening file: " + inputDataFilename;
        throw message.c_str();
    }
}

vector<vector<double> > parseAllStockDataCSVFile(const vector<string>& tickers) {
    vector<vector<double> > allStockData;
    for (int i = 0; i < tickers.size(); i++) {
        string ticker = tickers.at(i);
        vector<double> stockData = parseStockDataCSVFile(ticker + ".csv");
        int numberTimeSteps = (int) stockData.size();
        cout << "#####     " << ticker << ":     " << numberTimeSteps << endl;
        allStockData.push_back(stockData);
        stockData.clear();
    }
    return allStockData;
}

void writeStockDataToFile(const vector<vector<double> >& allStockData, const string& outputDataFilename) {
    ofstream fOut;
    fOut.open(outputDataFilename.c_str(), fstream::out);
    if (fOut.is_open()) {
        int numberStocks = (int) allStockData.size();
        for (int i = 0; i < numberStocks; i++) {
            vector<double> stockData = allStockData.at(i);
            int numberTimeSteps = (int) stockData.size();
            for (int t = 0; t < numberTimeSteps; t++) {
                fOut << stockData.at(t) << " ";
            }
            fOut << endl;
            stockData.clear();
        }
    }
    else {
        string message = "writeStockDataToFile: error opening file: " + outputDataFilename;
        throw message.c_str();
    }
}

