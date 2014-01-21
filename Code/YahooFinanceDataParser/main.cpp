//
// main.cpp
// Yahoo_Finance_Data_Parser
//
// Created by Hesam Ipakchi on 20/01/2014.
// Copyright (c) 2014 Hesam Ipakchi. All rights reserved.
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

vector<string> parseStockDatesCSVFile(const string& inputDataFilename);

vector<vector<string> > parseAllStockDatesCSVFile(const vector<string>& tickers);

void writeStockDatesToFile(const string& outputDatesFilename, const vector<vector<string> >& allStockDates);

vector<string> parseReferenceDatesFromFile(const string& referenceDatesFilename);

vector<string> extractTestableDates(const vector<vector<string> >& allStockDates, const vector<string>& referenceDates);

void writeTestableDatesToFile(string outputTestableDatesFilename, const vector<string>& testableDates);

vector<double> parseStockDataCSVFile(const string& inputDataFilename, const vector<string>& testableDates);

vector<vector<double> > parseAllStockDataCSVFile(const vector<string>& tickers, const vector<string>& testableDates);

void writeStockDataToFile(const string& outputDataFilename, const vector<vector<double> >& allStockData);

int main(int argc, const char * argv[])
{
    
    string inputTickerFilname = "tickers.txt";
	string outputStockDatesFilename = "outputStockDates.txt";
	string referenceDatesFilename = "referenceDates.txt";
    string outputTestableDatesFilename = "testableDates.txt";
    string outputDataFilename = "outputStockData.txt";
    
    vector<string> tickers = parseStockDataTickers(inputTickerFilname);
    
	vector<string> referenceDates = parseReferenceDatesFromFile(referenceDatesFilename);
    
	vector<vector<string> > allStockDates = parseAllStockDatesCSVFile(tickers);
	writeStockDatesToFile(outputStockDatesFilename, allStockDates);
    
	vector<string> testableDates = extractTestableDates(allStockDates, referenceDates);
    writeTestableDatesToFile(outputTestableDatesFilename, testableDates);
    
	vector<vector<double> > allStockData = parseAllStockDataCSVFile(tickers, testableDates);
    writeStockDataToFile(outputDataFilename, allStockData);
    
	referenceDates.clear();
	allStockDates.clear();
	testableDates.clear();
	allStockData.clear();
    
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

vector<string> parseStockDatesCSVFile(const string& inputDataFilename) {
    vector<string> stockDates;
    ifstream fInp;
    fInp.open(inputDataFilename.c_str());
    if (fInp.is_open()) {
        string text;
        string dateString;
        // determine position of close price within csv file
        int noUnusedDataBefore = 1;
        while (!fInp.eof()) {
            // remove unused information
            for (int i = 0; i < noUnusedDataBefore; i++) {
                getline ( fInp, text, '\n' );
            }
            
            // extract date of stock price
            getline ( fInp, dateString, ',' );
            
            // add stock date to dates vector
            if (!dateString.empty()) {
                stockDates.push_back(dateString);
            }
        }
        return stockDates;
    }
    else {
        string message = "parseStockDatesCSVFile: error opening file: " + inputDataFilename;
        throw message.c_str();
    }
}

vector<vector<string> > parseAllStockDatesCSVFile(const vector<string>& tickers) {
    vector<vector<string> > allStockDates;
    for (int i = 0; i < tickers.size(); i++) {
        string ticker = tickers.at(i);
        vector<string> stockDates = parseStockDatesCSVFile(ticker + ".csv");
        int noTimeSteps = (int) stockDates.size();
        cout << "##### noDates for " << ticker << ": " << noTimeSteps << endl;
        allStockDates.push_back(stockDates);
        stockDates.clear();
    }
    return allStockDates;
}

void writeStockDatesToFile(const string& outputDatesFilename, const vector<vector<string> >& allStockDates) {
    ofstream fOut;
    fOut.open(outputDatesFilename.c_str(), fstream::out);
    if (fOut.is_open()) {
        int numberStocks = (int) allStockDates.size();
        for (int i = 0; i < numberStocks; i++) {
            vector<string> stockDates = allStockDates.at(i);
            int numberTimeSteps = (int) stockDates.size();
            for (int t = 0; t < numberTimeSteps; t++) {
                fOut << stockDates.at(t) << " ";
            }
            if (i != (numberStocks-1)) {
				fOut << endl;
			}
            stockDates.clear();
        }
    }
    else {
        string message = "writeStockDatesToFile: error opening file: " + outputDatesFilename;
        throw message.c_str();
    }
}

vector<string> parseReferenceDatesFromFile(const string& referenceDatesFilename) {
    vector<string> referenceDates;
	ifstream fInp;
    fInp.open(referenceDatesFilename.c_str());
    if (fInp.is_open()) {
		string referenceDateString;
		while (!fInp.eof()) {
            
            // extract reference date
            getline (fInp, referenceDateString);
            
            // add reference date vector
            if (!referenceDateString.empty()) {
                referenceDates.push_back(referenceDateString);
            }
        }
		return referenceDates;
	}
	else {
        string message = "parseReferenceDatesCSVFile: error opening file: " + referenceDatesFilename;
        throw message.c_str();
    }
}

vector<string> extractTestableDates(const vector<vector<string> >& allStockDates, const vector<string>& referenceDates) {
	vector<string> testableDates;
	int noReferenceDates = (int) referenceDates.size();
	int noStocks = (int) allStockDates.size();
	cout << "#####     noReferenceDates = " << noReferenceDates << ", noStocks = " << noStocks << endl;
	for (int i = 0; i < noReferenceDates; i++) {
		string referenceDate = referenceDates.at(i);
        
        cout << "***** i = " << i << ", " << referenceDate << endl;
        
		bool isDateTestableForStock = true;
		for (int j = 0; j < noStocks; j++) {
			if (isDateTestableForStock) {
				vector<string> stockDates = allStockDates.at(j);
				// date not testable for this stock
				if (find(stockDates.begin(), stockDates.end(), referenceDate) == stockDates.end()) {
					isDateTestableForStock = false;
				}
                stockDates.clear();
			}
			else {
				break;
			}
		}
		// date is testable for ALL stocks
		if (isDateTestableForStock) {
			testableDates.push_back(referenceDate);
		}
	}
	int noTestableDates = (int) testableDates.size();
	cout << "#####     noTestableDates = " << noTestableDates << endl;
	return testableDates;
}

void writeTestableDatesToFile(string outputTestableDatesFilename, const vector<string>& testableDates) {
    ofstream fOut;
    fOut.open(outputTestableDatesFilename.c_str(), fstream::out);
    if (fOut.is_open()) {
        int numberTestableDates = (int) testableDates.size();
        for (int i = 0; i < numberTestableDates; i++) {
            fOut << testableDates.at(i);
			if (i != (numberTestableDates-1)) {
				fOut << endl;
			}
        }
    }
    else {
        string message = "writeStockDatesToFile: error opening file: " + outputTestableDatesFilename;
        throw message.c_str();
    }
}

vector<double> parseStockDataCSVFile(const string& inputDataFilename, const vector<string>& testableDates) {
    vector<double> stockData;
    ifstream fInp;
    fInp.open(inputDataFilename.c_str());
    if (fInp.is_open()) {
        string text;
		string dateString;
        string closePriceString;
        double closePrice;
        // determine position of date and close price within csv file
		int noUnusedDataBeforeDate = 1;
        int noUnusedDataBeforeClosePrice = 3;
        int noUnusedDataAfterClosePrice = 2;
        while (!fInp.eof()) {
			// remove unused information
            for (int i = 0; i < noUnusedDataBeforeDate; i++) {
                getline ( fInp, text, '\n' );
            }
            
            // extract date of stock price
            getline ( fInp, dateString, ',' );

			if (!dateString.empty()) {
				if(find(testableDates.begin(), testableDates.end(), dateString) != testableDates.end()) {
					// remove unused information
					for (int i = 0; i < noUnusedDataBeforeClosePrice; i++) {
						getline ( fInp, text, ',' );
					}
            
					// extract close price of stock
					getline ( fInp, closePriceString, ',' );
					closePrice = atof(closePriceString.c_str());
            
					// add close price of stock to data vector
					stockData.push_back(closePrice);
				}
			}
        }
        return stockData;
    }
    else {
        string message = "parseStockDataCSVFile: error opening file: " + inputDataFilename;
        throw message.c_str();
    }
}

vector<vector<double> > parseAllStockDataCSVFile(const vector<string>& tickers,  const vector<string>& testableDates) {
    vector<vector<double> > allStockData;
    for (int i = 0; i < tickers.size(); i++) {
        string ticker = tickers.at(i);
        vector<double> stockData = parseStockDataCSVFile(ticker + ".csv", testableDates);
        int noTimeSteps = (int) stockData.size();
        cout << "##### noTimeSteps for " << ticker << ": " << noTimeSteps << endl;
        allStockData.push_back(stockData);
        stockData.clear();
    }
    return allStockData;
}

void writeStockDataToFile(const string& outputDataFilename, const vector<vector<double> >& allStockData) {
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
			if (i != (numberStocks-1)) {
				fOut << endl;
			}
            stockData.clear();
        }
    }
    else {
        string message = "writeStockDataToFile: error opening file: " + outputDataFilename;
        throw message.c_str();
    }
}
