//
//  main.cpp
//  Crowdsourcing_Data_Parser
//
//  Created by Hesam Ipakchi on 20/12/2013.
//  Copyright (c) 2013 Hesam Ipakchi. All rights reserved.
//

#include "data_parser.h"
#include "utility.h"

int main(int argc, const char * argv[])
{
    DataParser dataParser;
    
    string evaluationFilename = "BarzanMozafari_Evaluation.txt";
    string labelsFilename = "BarzanMozafari_Labels.txt";
    string taskTrueValuesFilename = "BarzanMozafari_TaskTrueValues.txt";
    string workersTaskResponsesFilename = "BarzanMozafari_WorkersTaskResponses.txt";
    string workersTaskIdsFilename = "BarzanMozafari_WorkersTaskIds.txt";
    
    try {
        dataParser.writeTaskTrueValuesToFile(taskTrueValuesFilename, evaluationFilename);
    } catch (const char* msg) {
        cerr << msg << endl;
    }
    
    try {
        dataParser.writeWorkerTaskResponsesToFile(workersTaskResponsesFilename, evaluationFilename, labelsFilename);
    } catch (const char* msg) {
        cerr << msg << endl;
    }
    
    try {
        dataParser.writeWorkerTaskIdsToFile(workersTaskIdsFilename, labelsFilename);
    } catch (const char* msg) {
        cerr << msg << endl;
    }
    
//    vector<Task> tasks;
//    vector<Worker> workers;
//    
//    try {
//        tasks = dataParser.parseTasksEvaluation(evaluationFilename);
//    } catch (const char* msg) {
//        cerr << msg << endl;
//    }
//    
//    try {
//        workers = dataParser.parseWorkersLabels(labelsFilename);
//    } catch (const char* msg) {
//        cerr << msg << endl;
//    }
//    
//    cout << "Results for testing evaluation data parser with filename: " << evaluationFilename << "..." << endl << tasks << endl;
//    
//    cout << "Results for testing evaluation data parser with filename: " << labelsFilename << "..." << endl << workers << endl;
//    
//    tasks.clear();
//    workers.clear();
    
    return 0;
}

