#ifndef AESALON_GUI_SESSION_VISUALIZATION_THREAD_H
#define AESALON_GUI_SESSION_VISUALIZATION_THREAD_H

#include <QList>
#include <QThread>
#include <QPixmap>
#include <QTimer>

#include "VisualizationRequest.h"
#include "VisualizationRenderer.h"
#include "DataRequestQueue.h"
#include "storage/Timestamp.h"

class VisualizationThread : public QThread { Q_OBJECT
private:
    DataThread *data_thread;
    DataRequestQueue *request_queue;
    QList<VisualizationData *> v_data;
    VisualizationRequest *current_request;
    QPixmap *current_image;
    QTimer *queue_timer;
public:
    VisualizationThread(DataThread *data_thread, QObject *parent = 0);
    virtual ~VisualizationThread();
    
    DataRequestQueue *get_request_queue() const { return request_queue; }
protected:
    virtual void run();
    void send_request(DataRequest *request);
    virtual void generate_requests(VisualizationRequest *current_request) = 0;
    DataThread *get_data_thread() const { return data_thread; }
    virtual bool is_splittable() const = 0;
public slots:
    void update_request(VisualizationRequest *new_request);
    void render_request();
private slots:
    void process_queue();
signals:
    void replace_image(QPixmap *new_image);
    void image_updated();
};

#endif
