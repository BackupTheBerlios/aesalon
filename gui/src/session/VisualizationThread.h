#ifndef AESALON_GUI_SESSION_VISUALIZATION_THREAD_H
#define AESALON_GUI_SESSION_VISUALIZATION_THREAD_H

#include <QThread>
#include <QImage>

#include "DataRequestQueue.h"

class VisualizationThread : public QThread { Q_OBJECT
private:
    DataRequestQueue *request_queue;
public:
    VisualizationThread(QObject *parent = 0);
    virtual ~VisualizationThread();
    
    DataRequestQueue *get_request_queue() const { return request_queue; }
protected:
    virtual void run();
signals:
    void replace_image(QImage *new_image);
};

#endif
