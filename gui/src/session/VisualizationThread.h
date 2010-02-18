#ifndef AESALON_GUI_SESSION_VISUALIZATION_THREAD_H
#define AESALON_GUI_SESSION_VISUALIZATION_THREAD_H

#include <QList>
#include <QThread>
#include <QImage>

#include "DataRequestQueue.h"
#include "storage/Timestamp.h"

class VisualizationThread : public QThread { Q_OBJECT
private:
    DataThread *data_thread;
    DataRequestQueue *request_queue;
    QList<VisualizationData *> v_data;
public:
    VisualizationThread(DataThread *data_thread, QObject *parent = 0);
    virtual ~VisualizationThread();
    
    DataRequestQueue *get_request_queue() const { return request_queue; }
protected:
    virtual void run();
    void send_request(DataRequest *request);
public slots:
    void render(const Timestamp &from, const Timestamp &to);
signals:
    void replace_image(QImage *new_image);
};

#endif
