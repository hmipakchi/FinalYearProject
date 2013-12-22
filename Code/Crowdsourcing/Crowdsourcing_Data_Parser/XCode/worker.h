#ifndef worker_h
#define worker_h

#include "utility.h"
#include "task.h"

/* Worker class */

template <class TT, class WT>
class Worker {
public:
    Worker() : workerId(WT()){}
    
    ~Worker() {
        tasks.clear();
    }
    
    Worker(const Worker<TT, WT>& tc):workerId(tc.workerId), tasks(tc.tasks) {}
    
    Worker(const WT& workerId, const vector<Task<TT>>& tasks) {
        this->workerId = workerId;
        this->tasks = tasks;
    }
    
    Worker(const WT& workerId, const Task<TT>& task) {
        this->workerId = workerId;
        vector<Task<TT>> temp;
        temp.push_back(task);
        this->tasks = temp;
        temp.clear();
    }
    
    Worker operator=(const Worker& tc) {
        if (this == &tc) {
            return *this;
        }
        else {
            workerId = tc.workerId;
            tasks = tc.tasks;
            return *this;
        }
    }
    
    void setWorkerId(const WT& workerId) {
        this->workerId = workerId;
    }
    
    WT getWorkerId() const {
        return workerId;
    }
    
    void setAllTasks(const vector<Task<TT>>& tasks) {
        this->tasks = tasks;
    }
    
    vector<Task<TT>> getAllTasks() const {
        return tasks;
    }
    
    vector<int> getAllTaskIds() const {
        vector<int> taskIds;
        for (int i = 0; i < tasks.size(); i++) {
            taskIds.push_back(tasks.at(i).getTaskIdConverted());
        }
        return taskIds;
    }
    
    void addTask(const Task<TT>& task) {
        tasks.push_back(task);
    }
    
    string toString() const {
        string tasksForWorker = "[";
        for (int i = 0; i < tasks.size(); i++) {
            if (i != 0) {
                tasksForWorker = tasksForWorker + "," + tasks.at(i).toString();
            }
            else {
                tasksForWorker = tasksForWorker + tasks.at(i).toString();
            }
        }
        tasksForWorker = tasksForWorker + "]";
        return convertTemplateTypenameToString(workerId) + "," + tasksForWorker;
    }
    
    friend ostream& operator<<(ostream& os, const Worker<TT,WT>& obj) {
        os << obj.toString();
        return os;
    }
    
private:
    WT workerId;
    vector<Task<TT>> tasks;
};

#endif