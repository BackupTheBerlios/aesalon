#include "DataRequestQueue.h"

DataRequestQueue::DataRequestQueue() {
}

DataRequestQueue::~DataRequestQueue() {
    
}

void DataRequestQueue::push_request(DataRequest *request) {
    list_mutex.lock();
    request_list.enqueue(request);
    list_mutex.unlock();
}

DataRequest *DataRequestQueue::pop_request() {
    /* Do this out of order so that the mutex is locked for the smallest length of time possible. */
    /* NOTE: dubious speed gains are offset by an obvious complexity increase . . . */
    DataRequest *request = NULL;
    list_mutex.lock();
    request = request_list.dequeue();
    list_mutex.unlock();
    return request;
}

int DataRequestQueue::current_requests() {
    int count;
    list_mutex.lock();
    count = request_list.count();
    list_mutex.unlock();
    return count;
}
