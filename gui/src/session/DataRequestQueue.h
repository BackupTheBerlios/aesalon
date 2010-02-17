#ifndef AESALON_GUI_SESSION_DATA_REQUEST_QUEUE_H
#define AESALON_GUI_SESSION_DATA_REQUEST_QUEUE_H

#include <QQueue>
#include <QMutex>
#include "DataRequest.h"

class DataRequestQueue {
private:
    static const int request_list_size = 1000;
    QQueue<DataRequest *> request_list;
    QMutex list_mutex;
    int last_slot;
public:
    DataRequestQueue();
    virtual ~DataRequestQueue();
    
    void push_request(DataRequest *request);
    DataRequest *pop_request();
    int current_requests();
};

#endif
