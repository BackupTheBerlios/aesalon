#ifndef AESALON_GUI_SESSION_GRAPH_DATA_ENGINE_H
#define AESALON_GUI_SESSION_GRAPH_DATA_ENGINE_H

#include <QMutex>
#include <QObject>
#include <QQueue>

#include "DataRequest.h"

class GraphDataEngine : public QObject { Q_OBJECT
private:
    QQueue<DataRequest *> data_request_queue;
    QMutex queue_mutex;
public:
    GraphDataEngine(QObject *parent);
    virtual ~GraphDataEngine();
    
    /* NOTE: this function is called from outside the current thread. Hence the mutex. */
    void push_request(DataRequest *request) {
        QMutexLocker locker(&queue_mutex);
        data_request_queue.enqueue(request);
    }
    DataRequest *pop_request() {
        QMutexLocker locker(&queue_mutex);
        if(data_request_queue.isEmpty()) return NULL;
        return data_request_queue.dequeue();
    }
    
    void handle_requests();
};

#endif
