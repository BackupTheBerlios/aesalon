#ifndef AESALON_GUI_VISUALIZATION_CONTROLLER_H
#define AESALON_GUI_VISUALIZATION_CONTROLLER_H

#include <QThread>
#include <QTimer>

#include "session/DataThread.h"
#include "DataRange.h"
#include "Renderer.h"

class Controller : public QThread { Q_OBJECT
private:
    DataThread *data_thread;
    QTimer *update_timer;
    Renderer *renderer;
public:
    Controller(Renderer *renderer, DataThread *data_thread);
    virtual ~Controller();
    
    virtual void run();
public slots:
    void change_update_time(int ms);
    void attach();
    void render_full();
private slots:
    void update();
    Canvas *render_region(const DataRange &range);
signals:
    void clear_canvas();
    void canvas_update(Canvas *canvas);
    void change_range(const DataRange &range);
};

#endif
