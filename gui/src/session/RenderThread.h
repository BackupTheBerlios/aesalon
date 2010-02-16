#ifndef AESALON_GUI_SESSION_RENDER_THREAD_H
#define AESALON_GUI_SESSION_RENDER_THREAD_H

#include <QThread>
#include <QSemaphore>

#include "DataRequest.h"

class RenderThread : public QThread { Q_OBJECT
private:
    QList<DataRequest *> request_list;
public:
    RenderThread(QObject *parent);
    virtual ~RenderThread();
protected:
    virtual void run();
};

#endif
