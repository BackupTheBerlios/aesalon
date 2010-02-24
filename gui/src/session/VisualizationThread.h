#ifndef AESALON_GUI_SESSION_VISUALIZATION_THREAD_H
#define AESALON_GUI_SESSION_VISUALIZATION_THREAD_H

#include <QList>
#include <QThread>
#include <QImage>

#include "VisualizationRequest.h"
#include "DataRequestQueue.h"
#include "storage/Timestamp.h"

class VisualizationThread : public QThread { Q_OBJECT
private:
    DataThread *data_thread;
    DataRequestQueue *request_queue;
    QList<VisualizationData *> v_data;
    VisualizationRequest *current_request;
    QImage *current_image;
public:
    VisualizationThread(DataThread *data_thread, QObject *parent = 0);
    virtual ~VisualizationThread();
    
    DataRequestQueue *get_request_queue() const { return request_queue; }
protected:
    virtual void run();
    void send_request(DataRequest *request);
    virtual void generate_requests(VisualizationRequest *current_request) = 0;
    DataThread *get_data_thread() const { return data_thread; }
public slots:
    void update_request(VisualizationRequest *new_request);
signals:
    void replace_image(QImage *new_image);
};

#endif
