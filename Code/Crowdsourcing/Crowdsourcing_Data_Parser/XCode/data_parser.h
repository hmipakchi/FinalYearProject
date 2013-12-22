#ifndef data_parser_h
#define data_parser_h

#include "task.h"
#include "worker.h"
#include "utility.h"

/* DataParser class */

template <class TT, class WT>
class DataParser {
public:
    DataParser() {
    }
    
    ~DataParser() {
        tasksIdMap.clear();
    }
    
    DataParser(const DataParser<TT, WT>& tc) : tasksIdMap(tc.tasksIdMap) {}
    
    DataParser(const vector<TT>& tasksIdMap) {
        this->tasksIdMap = tasksIdMap;
    }
    
    DataParser<TT, WT> operator=(const DataParser<TT, WT>& tc) {
        if (this == &tc) {
            return *this;
        }
        else {
            tasksIdMap = tc.tasksIdMap;
            return *this;
        }
    }
    
    void writeTaskTrueValuesToFile(const string& taskTrueValuesFilename, const string& evaluationFilename) {
        ofstream fOut;
        fOut.open(taskTrueValuesFilename.c_str(), fstream::out);
        if (fOut.is_open()) {
            vector<Task<TT>> tasks = parseTasksEvaluation(evaluationFilename);
            for (int i = 0; i < tasks.size(); i++) {
                Task<TT> task = tasks.at(i);
                fOut << task.getTaskIdConverted() << " " << task.getTrueValue() << endl;
            }
            tasks.clear();
            fOut.close();
        }
        else {
            string message = "DataParser::writeTaskTrueValuesToFile: error opening file: " + taskTrueValuesFilename;
            throw message.c_str();
        }
    }
    
    void writeWorkerTaskResponsesToFile(const string& workersTaskResponsesFilename, const string& evaluationFilename, const string& labelsFilename) {
        ofstream fOut;
        fOut.open(workersTaskResponsesFilename.c_str(), fstream::out);
        if (fOut.is_open()) {
            vector<Task<TT>> tasks = parseTasksEvaluation(evaluationFilename);
            vector<Worker<TT, WT>> workers = parseWorkersLabels(labelsFilename);
            for (int i = 0; i < workers.size(); i++) {
                Worker<TT, WT> worker = workers.at(i);
                vector<Task<TT>> workerTasks = worker.getAllTasks();
                vector<int> workerTaskIds = worker.getAllTaskIds();
                for (int j = 0; j < tasks.size(); j++)
                {
                    int taskIdConverted = tasks.at(j).getTaskIdConverted();
                    vector<int>::iterator indexFoundWorkerTaskId = find(workerTaskIds.begin(), workerTaskIds.end(), taskIdConverted);
                    if (indexFoundWorkerTaskId != workerTaskIds.end()) {
                        int indexFoundWorkerTask = indexFoundWorkerTaskId - workerTaskIds.begin();
                        fOut << workerTasks.at(indexFoundWorkerTask).getResponse() << " ";
                    }
                    else {
                        if (j == tasks.size()-1) {
                            fOut << "0";
                        }
                        else {
                            fOut << "0 ";
                        }
                    }
                }
                workerTasks.clear();
                workerTaskIds.clear();
                fOut << endl;
            }
            tasks.clear();
            workers.clear();
            fOut.close();
        }
        else {
            string message = "DataParser::writeWorkerTaskResponsesToFile: error opening file: " + workersTaskResponsesFilename;
            throw message.c_str();
        }
    }
    
    void writeWorkerTaskIdsToFile(const string& workersTaskIdsFilename, const string& labelsFilename) const {
        ofstream fOut;
        fOut.open(workersTaskIdsFilename.c_str(), fstream::out);
        if (fOut.is_open()) {
            vector<Worker<TT, WT>> workers = parseWorkersLabels(labelsFilename);
            int maxNoTasksCompletedForOneWorker = 0;
            for (int i = 0; i < workers.size(); i++) {
                int noTasksByWorker = workers.at(i).getAllTaskIds().size();
                if (noTasksByWorker > maxNoTasksCompletedForOneWorker) {
                    maxNoTasksCompletedForOneWorker = noTasksByWorker;
                }
            }
            fOut << workers.size() << " " << maxNoTasksCompletedForOneWorker << endl;
            for (int i = 0; i < workers.size(); i++) {
                Worker<TT, WT> worker = workers.at(i);
                vector<int> workerTaskIds = worker.getAllTaskIds();
                for (int j = 0; j < workerTaskIds.size(); j++) {
                    fOut << workerTaskIds.at(j) << " ";
                }
                workerTaskIds.clear();
                fOut << endl;
            }
            workers.clear();
            fOut.close();
        }
        else {
            string message = "DataParser::writeWorkerTaskIdsToFile: error opening file: " + workersTaskIdsFilename;
            throw message.c_str();
        }
    }
    
private:
    vector<TT> tasksIdMap;
    
    int mapTaskId(const TT& taskId) const {
        for (int i = 0; i < tasksIdMap.size(); i++) {
            string mappedTaskIdString = convertTemplateTypenameToString(tasksIdMap.at(i));
            string taskIdString = convertTemplateTypenameToString(taskId);
            
            if (mappedTaskIdString.compare(taskIdString) == 0) {
                return i+1;
            }
        }
        string message = "DataParser::mapTaskId: error mapping mapTaskId: " + convertTemplateTypenameToString(taskId);
        throw message.c_str();
        
    }
    
    vector<Task<TT>> parseTasksEvaluation(const string& evaluationFilename) {
        bool isTaskIdMapped = (tasksIdMap.size() == 0) ? false : true;
        ifstream fInp;
        vector<Task<TT>> tasks;
        fInp.open(evaluationFilename.c_str());
        if (fInp.is_open()) {
            TT taskId = TT();
            int trueValue, response;
            string trueValueTemp, responseTemp;
            int taskIdConverted = 1;
            while (!fInp.eof()) {
                fInp >> taskId >> trueValueTemp;
                trueValue = convertBinaryLabel(trueValueTemp);
                response = 0;
                tasks.push_back(Task<TT>(taskId,taskIdConverted,trueValue,response));
                if (!isTaskIdMapped) {
                    tasksIdMap.push_back(taskId);
                }
                taskIdConverted++;
            }
            fInp.close();
            cout << "######   *****   NUMBER OF TASKS PARSED = " << tasks.size() << "   *****   ######" << endl;
            return tasks;
        }
        else {
            string message = "DataParser::parseTasksEvaluation: error opening file: " + evaluationFilename;
            throw message.c_str();
        }
    }
    
    vector<Worker<TT, WT>> parseWorkersLabels(const string& labelsFilename) const {
        ifstream fInp;
        vector<Worker<TT, WT>> workers;
        vector<WT> foundWorkers;
        fInp.open(labelsFilename.c_str());
        if (fInp.is_open()) {
            WT workerId;
            TT taskId;
            int response, trueValue;
            string responseTemp, trueValueTemp;
            int queryCount = 0;
            int taskIdConverted;
            while (!fInp.eof()) {
                fInp >> workerId >> taskId >> responseTemp;
                
                trueValue = 0;
                response = convertBinaryLabel(responseTemp);
                
                try {
                    taskIdConverted = mapTaskId(taskId);
                } catch (const char* msg) {
                    cerr << msg << endl;
                }
                
                typename vector<WT>::iterator indexFoundWorkerId = find(foundWorkers.begin(), foundWorkers.end(), workerId);
                if (indexFoundWorkerId != foundWorkers.end()) {
                    int indexFoundWorker = indexFoundWorkerId - foundWorkers.begin();
                    workers.at(indexFoundWorker).addTask(Task<TT>(taskId, taskIdConverted, trueValue, response));
                }
                else {
                    workers.push_back(Worker<TT, WT>(workerId, Task<TT>(taskId, taskIdConverted, trueValue, response)));
                    foundWorkers.push_back(workerId);
                }
                queryCount++;
            }
            fInp.close();
            cout << "######   *****   NUMBER OF WORKERS PARSED = " << workers.size() << "   *****   ######" << endl;
            cout << "######   *****   NUMBER OF QUERIES IN TOTAL PARSED = " << queryCount << "   *****   ######" << endl;
            foundWorkers.clear();
            return workers;
        }
        else {
            string message = "DataParser::parseWorkersLabels: error opening file: " + labelsFilename;
            throw message.c_str();
        }
    }
    
    int convertBinaryLabel(string value) const {
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        if (value == "0") {
            return -1;
        }
        if (value == "1") {
            return 1;
        }
        if (value == "no") {
            return -1;
        }
        if (value == "yes") {
            return 1;
        }
        return 0;
    }
};

#endif