#ifndef AESALON_GUI_SESSION_VISUALIZATION_H
#define AESALON_GUI_SESSION_VISUALIZATION_H

#include <QWidget>
#include <QImage>

#include "VisualizationThread.h"

class Visualization : public QWidget { Q_OBJECT
private:
    VisualizationThread *v_thread;
    QImage *current_image;
protected:
    virtual VisualizationThread *create_v_thread(DataThread *data_thread)
        { data_thread = data_thread; return NULL; }
public:
    Visualization(DataThread *data_thread, QWidget *parent = 0);
    virtual ~Visualization();
protected:
    virtual void paintEvent(QPaintEvent *event);
private slots:
    void update_image(QImage *image);
};

#endif
