#ifndef AESALON_GUI_VISUALIZATION_CANVAS_GENERATOR_THREAD_H
#define AESALON_GUI_VISUALIZATION_CANVAS_GENERATOR_THREAD_H

#include <QThread>

#include "session/DataThread.h"
#include "Renderer.h"

class CanvasGeneratorThread : public QThread { Q_OBJECT
private:
    DataThread *data_thread;
    Renderer *renderer;
public:
    CanvasGeneratorThread(DataThread *data_thread, Renderer *renderer);
    virtual ~CanvasGeneratorThread();
    
    virtual void run();
};

#endif
