#include "data_parser.h"

/* DataParser class methods */

DataParser::DataParser() {}

DataParser::~DataParser() {}

DataParser DataParser::operator=(const DataParser& tc) {
    if (this == &tc) {
        return *this;
    }
    else {
        return *this;
    }
}

vector<Task> DataParser::parseTasksEvaluation(const string& evaluationFilename) const {
    ifstream fInp;
    vector<Task> tasks;
    fInp.open(evaluationFilename.c_str());
    if (fInp.is_open()) {
        int taskId, trueValue, response;
        while (!fInp.eof()) {
            fInp >> taskId >> trueValue;
            convertBinaryLabel(trueValue);
            response = 0;
            tasks.push_back(Task(taskId,trueValue,response));
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

vector<Worker> DataParser::parseWorkersLabels(const string& labelsFilename) const {
    ifstream fInp;
    vector<Worker> workers;
    vector<string> foundWorkers;
    fInp.open(labelsFilename.c_str());
    if (fInp.is_open()) {
        string workerId;
        int taskId, response, trueValue;
        int queryCount = 0;
        while (!fInp.eof()) {
            fInp >> workerId >> taskId >> response;
            trueValue = 0;
            convertBinaryLabel(response);
            vector<string>::iterator indexFoundWorkerId = find(foundWorkers.begin(), foundWorkers.end(), workerId);
            if (indexFoundWorkerId != foundWorkers.end()) {
                int indexFoundWorker = indexFoundWorkerId - foundWorkers.begin();
                workers.at(indexFoundWorker).addTask(Task(taskId, trueValue, response));
            }
            else {
                workers.push_back(Worker(workerId, Task(taskId, trueValue, response)));
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

void DataParser::writeTaskTrueValuesToFile(const string& taskTrueValuesFilename, const string& evaluationFilename) const {
    ofstream fOut;
    fOut.open(taskTrueValuesFilename.c_str(), fstream::out);
    if (fOut.is_open()) {
        vector<Task> tasks = parseTasksEvaluation(evaluationFilename);
        for (int i = 0; i < tasks.size(); i++) {
            Task task = tasks.at(i);
            fOut << task.getTaskId() << " " << task.getTrueValue() << endl;
        }
        tasks.clear();
        fOut.close();
    }
    else {
        string message = "DataParser::writeTaskTrueValuesToFile: error opening file: " + taskTrueValuesFilename;
        throw message.c_str();
    }
}


void DataParser::writeWorkerTaskResponsesToFile(const string& workersTaskResponsesFilename, const string& evaluationFilename, const string& labelsFilename) const {
    ofstream fOut;
    fOut.open(workersTaskResponsesFilename.c_str(), fstream::out);
    if (fOut.is_open()) {
        vector<Task> tasks = parseTasksEvaluation(evaluationFilename);
        vector<Worker> workers = parseWorkersLabels(labelsFilename);
        for (int i = 0; i < workers.size(); i++) {
            Worker worker = workers.at(i);
            vector<Task> workerTasks = worker.getAllTasks();
            vector<int> workerTaskIds = worker.getAllTaskIds();
            for (int j = 0; j < tasks.size(); j++)
            {
                int taskId = tasks.at(j).getTaskId();
                vector<int>::iterator indexFoundWorkerTaskId = find(workerTaskIds.begin(), workerTaskIds.end(), taskId);
                if (indexFoundWorkerTaskId != workerTaskIds.end()) {
                    int indexFoundWorkerTask = indexFoundWorkerTaskId - workerTaskIds.begin();
                    fOut << workerTasks.at(indexFoundWorkerTask).getResponse() << " ";
                }
                else {
                    fOut << "0 ";
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

void DataParser::writeWorkerTaskIdsToFile(const string& workersTaskIdsFilename, const string& labelsFilename) const {
    ofstream fOut;
    fOut.open(workersTaskIdsFilename.c_str(), fstream::out);
    if (fOut.is_open()) {
        vector<Worker> workers = parseWorkersLabels(labelsFilename);
        int maxNoTasksCompletedForOneWorker = 0;
        for (int i = 0; i < workers.size(); i++) {
            int noTasksByWorker = workers.at(i).getAllTaskIds().size();
            if (noTasksByWorker > maxNoTasksCompletedForOneWorker) {
                maxNoTasksCompletedForOneWorker = noTasksByWorker;
            }
        }
        fOut << workers.size() << " " << maxNoTasksCompletedForOneWorker << endl;
        for (int i = 0; i < workers.size(); i++) {
            Worker worker = workers.at(i);
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

void DataParser::convertBinaryLabel(int& value) const {
    if (value == 0) {
        value = -1;
    }
}