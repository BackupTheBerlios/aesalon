#ifndef AESALON_GUI_VISUALIZATION_CONTROLLER_H
#define AESALON_GUI_VISUALIZATION_CONTROLLER_H

#include <QThread>
#include <QTimer>

#include "session/DataThread.h"
#include "DataRange.h"
#include "Renderer.h"
#include "VisualizationFactory.h"

class Controller : public QThread { Q_OBJECT
private:
    DataThread *data_thread;
    QTimer *update_timer;
    Renderer *renderer;
    qint64 last_update;
public:
    Controller(VisualizationFactory *factory);
    virtual ~Controller();
    
    virtual void run();
public slots:
    void change_update_time(int ms);
    void begin_rt();
    void end_rt();
    void render_full();
private slots:
    void rt_update();
    Canvas *render_region(const DataRange &range);
signals:
    void clear_canvas();
    void canvas_update(Canvas *canvas);
    void change_range(const DataRange &range);
    void shift_range_to(Timestamp timestamp);
    void force_repaint();
};

#endif
