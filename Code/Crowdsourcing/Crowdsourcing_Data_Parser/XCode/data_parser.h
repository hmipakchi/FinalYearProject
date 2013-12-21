#ifndef data_parser_h
#define data_parser_h

#include "task.h"
#include "worker.h"
#include "utility.h"

/* DataParser class */

class DataParser {
public:
    DataParser();
    
    ~DataParser();
    
    DataParser operator=(const DataParser& tc);
    
    void writeTaskTrueValuesToFile(const string& taskTrueValuesFilename, const string& evaluationFilename) const;
    
    void writeWorkerTaskResponsesToFile(const string& workersTaskResponsesFilename, const string& evaluationFilename, const string& labelsFilename) const;
    
    void writeWorkerTaskIdsToFile(const string& workersTaskIdsFilename, const string& labelsFilename) const;
    
private:
    vector<Task> parseTasksEvaluation(const string& evaluationFilename) const;
    
    vector<Worker> parseWorkersLabels(const string& labelsFilename) const;
    
    void convertBinaryLabel(int& trueValue) const;
};

#endif