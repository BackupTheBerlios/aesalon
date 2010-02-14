#ifndef AESALON_GUI_SESSION_DATA_REQUEST_H
#define AESALON_GUI_SESSION_DATA_REQUEST_H

#include <QObject>

class SnapshotList;
class GraphDataEngine;

class DataRequest {
private:
    GraphDataEngine *requester;
public:
    DataRequest(GraphDataEngine *data_engine) : requester(data_engine) {}
    virtual ~DataRequest() {}
    
    GraphDataEngine *get_requester() const { return requester; }
    
    /* NOTE: this method is called from within the data storage thread. */
    virtual void construct_response(SnapshotList *snapshot_list) = 0;
    /* NOTE: this method is called from the rendering thread. */
    virtual void process_response() = 0;
};

#endif
