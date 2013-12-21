#ifndef task_h
#define task_h

#include "utility.h"

/* Task class */

class Task {
public:
    Task();
    
    ~Task();
    
    Task(const Task& tc);
    
    Task(const int& taskId, const int& trueValue, const int& response);
    
    Task operator=(const Task& tc);
    
    void setTaskId(const int& taskId);
    
    int getTaskId() const;
    
    void setTrueValue(const int& trueValue);
    
    int getTrueValue() const;
    
    void setResponse(const int& response);
    
    int getResponse() const;
    
    string toString() const;
    
    friend ostream& operator<<(ostream& os, const Task& obj);
    
private:
    int taskId;
    int trueValue;
    int response;
};

#endif