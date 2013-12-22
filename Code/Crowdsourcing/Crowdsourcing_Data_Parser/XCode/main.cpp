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
    // first template type: TaskId type
    // second template type: WorkerId type
    DataParser<int, string> dataParser;

    string evaluationFilename = "BarzanMozafari/BarzanMozafari_Evaluation.txt";
    string labelsFilename = "BarzanMozafari/BarzanMozafari_Labels.txt";
    string taskTrueValuesFilename = "BarzanMozafari/BarzanMozafari_TaskTrueValues.txt";
    string workersTaskResponsesFilename = "BarzanMozafari/BarzanMozafari_WorkersTaskResponses.txt";
    string workersTaskIdsFilename = "BarzanMozafari/BarzanMozafari_WorkersTaskIds.txt";
    
//    string evaluationFilename = "HITspam_Crowdflower/HITspam_Crowdflower_Evaluation.txt";
//    string labelsFilename = "HITspam_Crowdflower/HITspam_Crowdflower_Labels.txt";
//    string taskTrueValuesFilename = "HITspam_Crowdflower/HITspam_Crowdflower_TaskTrueValues.txt";
//    string workersTaskResponsesFilename = "HITspam_Crowdflower/HITspam_Crowdflower_WorkersTaskResponses.txt";
//    string workersTaskIdsFilename = "HITspam_Crowdflower/HITspam_Crowdflower_WorkersTaskIds.txt";
    
//    string evaluationFilename = "HITspam_MTurk/HITspam_MTurk_Evaluation.txt";
//    string labelsFilename = "HITspam_MTurk/HITspam_MTurk_Labels.txt";
//    string taskTrueValuesFilename = "HITspam_MTurk/HITspam_MTurk_TaskTrueValues.txt";
//    string workersTaskResponsesFilename = "HITspam_MTurk/HITspam_MTurk_WorkersTaskResponses.txt";
//    string workersTaskIdsFilename = "HITspam_MTurk/HITspam_MTurk_WorkersTaskIds.txt";


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

