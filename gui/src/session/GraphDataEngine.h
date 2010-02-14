#ifndef AESALON_GUI_SESSION_GRAPH_DATA_ENGINE_H
#define AESALON_GUI_SESSION_GRAPH_DATA_ENGINE_H

#include <QMutex>
#include <QObject>
#include <QQueue>

#include "DataRequest.h"
#include "GraphData.h"
#include "DataThread.h"

class GraphDataEngine : public QObject { Q_OBJECT
private:
    QMutex queue_mutex;
    QQueue<DataRequest *> data_request_queue;
    
    QList<GraphData *> graph_data;
    DataThread *data_thread;
protected:
    virtual DataRequest *spawn_new_request(const Timestamp &timestamp) = 0;
public:
    GraphDataEngine(QObject *parent, DataThread *data_thread);
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
    
    void add_data(GraphData *data) {
        graph_data.append(data);
    }
    
    void get_data(const Timestamp &timestamp);
    
    void handle_requests();
};

#endif
