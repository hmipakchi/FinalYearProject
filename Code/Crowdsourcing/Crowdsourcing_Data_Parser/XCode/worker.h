#ifndef worker_h
#define worker_h

#include "utility.h"
#include "task.h"

/* Worker class */

class Worker {
public:
    Worker();
    
    ~Worker();
    
    Worker(const Worker& tc);
    
    Worker(const string& workerId, const vector<Task>& tasks);
    
    Worker(const string& workerId, const Task& task);
    
    Worker operator=(const Worker& tc);
    
    void setWorkerId(const string& workerId);
    
    string getWorkerId() const;
    
    void setAllTasks(const vector<Task>& tasks);
    
    vector<Task> getAllTasks() const;
    
    vector<int> getAllTaskIds() const;
    
    int getResponse(const int& taskId) const;
    
    void addTask(const Task& task);
    
    string toString() const;
    
    friend ostream& operator<<(ostream& os, const Worker& obj);
    
private:
    string workerId;
    vector<Task> tasks;
};

#endif