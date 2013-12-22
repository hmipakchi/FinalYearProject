#ifndef task_h
#define task_h

#include "utility.h"

/* Task class */

template <class T>
class Task {
public:
    Task() : taskId(T()), taskIdConverted(0), trueValue(0), response(0) {}
    
    ~Task() {}
    
    Task(const Task<T>& tc) : taskId(tc.taskId), taskIdConverted(tc.taskIdConverted), trueValue(tc.trueValue), response(tc.response) {}
    
    Task(const T& taskId, const int& trueValue, const int& response) {
        this->taskId = taskId;
        this->trueValue = trueValue;
        this->response = response;
    }
    
    Task(const T& taskId, const int& taskIdConverted, const int& trueValue, const int& response) {
        this->taskId = taskId;
        this->taskIdConverted = taskIdConverted;
        this->trueValue = trueValue;
        this->response = response;
    }
    
    Task<T> operator=(const Task<T>& tc) {
        if (this == &tc) {
            return *this;
        }
        else {
            taskId = tc.taskId;
            taskIdConverted = tc.taskIdConverted;
            trueValue = tc.trueValue;
            response = tc.response;
            return *this;
        }
    }
    
    void setTaskId(const T& taskId) {
        this->taskId = taskId;
    }
    
    T getTaskId() const {
        return taskId;
    }
    
    void setTaskIdConverted (const int& taskIdConverted) {
        this->taskIdConverted = taskIdConverted;
    }
    
    int getTaskIdConverted() const {
        return taskIdConverted;
    }
    
    void setTrueValue(const int& trueValue) {
        this->trueValue = trueValue;
    }
    
    int getTrueValue() const {
        return trueValue;
    }
    
    void setResponse(const int& response) {
        this->response = response;
    }
    
    int getResponse() const {
        return response;
    }
    
    string toString() const {
        return convertTemplateTypenameToString(taskId) + "," + convertTemplateTypenameToString(trueValue) + "," + convertTemplateTypenameToString(response);
    }
    
    friend ostream& operator<<(ostream& os, const Task<T>& obj) {
        os << obj.toString();
        return os;
    }
    
private:
    T taskId;
    int taskIdConverted;
    int trueValue;
    int response;
};

#endif