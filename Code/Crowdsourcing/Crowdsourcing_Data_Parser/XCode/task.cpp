#include "task.h"

/* Task class methods */

Task::Task() {
    taskId = 0;
    trueValue = 0;
    response = 0;
}

Task::~Task() {}

Task::Task(const Task& tc):taskId(tc.taskId), trueValue(tc.trueValue), response(tc.response) {}

Task::Task(const int& taskId, const int& trueValue, const int& response) {
    this->taskId = taskId;
    this->trueValue = trueValue;
    this->response = response;
}

Task Task::operator=(const Task& tc) {
    if (this == &tc) {
        return *this;
    }
    else {
        taskId = tc.taskId;
        trueValue = tc.trueValue;
        response = tc.response;
        return *this;
    }
}

void Task::setTaskId(const int& taskId) {
    this->taskId = taskId;
}

int Task::getTaskId() const {
    return taskId;
}

void Task::setTrueValue(const int& trueValue) {
    this->trueValue = trueValue;
}

int Task::getTrueValue() const {
    return trueValue;
}

void Task::setResponse(const int& response) {
    this->response = response;
}

int Task::getResponse() const {
    return response;
}

string Task::toString() const {
    return convertIntToString(taskId) + "," + convertIntToString(trueValue) + "," + convertIntToString(response);
}

ostream& operator<<(ostream& os, const Task& obj) {
    os << obj.toString();
    return os;
}