#ifndef AESALON_GUI_SESSION_DATA_REQUEST_H
#define AESALON_GUI_SESSION_DATA_REQUEST_H

#include <QObject>

class Snapshot;

class DataRequest {
private:
    QObject *object;
public:
    DataRequest(QObject *by_object) : object(by_object) {}
    virtual ~DataRequest() {}
    
    QObject *get_object() const { return object; }
    
    /* NOTE: this function's definition will change . . . */
    virtual void get_response(Snapshot *snapshot) = NULL;
};

#endif
