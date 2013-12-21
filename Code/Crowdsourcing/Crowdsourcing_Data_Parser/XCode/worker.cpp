#include "worker.h"

/* Worker class methods */

Worker::Worker() {}

Worker::~Worker() {
    tasks.clear();
}

Worker::Worker(const Worker& tc):workerId(tc.workerId), tasks(tc.tasks) {}

Worker::Worker(const string& workerId, const vector<Task>& tasks) {
    this->workerId = workerId;
    this->tasks = tasks;
}

Worker::Worker(const string& workerId, const Task& task) {
    this->workerId = workerId;
    vector<Task> temp;
    temp.push_back(task);
    this->tasks = temp;
    temp.clear();
}

Worker Worker::operator=(const Worker& tc) {
    if (this == &tc) {
        return *this;
    }
    else {
        workerId = tc.workerId;
        tasks = tc.tasks;
        return *this;
    }
}

void Worker::setWorkerId(const string& workerId) {
    this->workerId = workerId;
}

string Worker::getWorkerId() const {
    return workerId;
}

void Worker::setAllTasks(const vector<Task>& tasks) {
    this->tasks = tasks;
}

vector<Task> Worker::getAllTasks() const {
    return tasks;
}

vector<int> Worker::getAllTaskIds() const {
    vector<int> taskIds;
    for (int i = 0; i < tasks.size(); i++) {
        taskIds.push_back(tasks.at(i).getTaskId());
    }
    return taskIds;
}

void Worker::addTask(const Task& task) {
    tasks.push_back(task);
}

string Worker::toString() const {
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
    return workerId + "," + tasksForWorker;
}

ostream& operator<<(ostream& os, const Worker& obj) {
    os << obj.toString();
    return os;
}