#ifndef AESALON_GUI_VISUALIZATION_CANVAS_GENERATOR_H
#define AESALON_GUI_VISUALIZATION_CANVAS_GENERATOR_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

#include "Canvas.h"
#include "Renderer.h"
#include "session/DataThread.h"
#include "CanvasGeneratorThread.h"

class CanvasGenerator : public QDialog { Q_OBJECT
private:
    DataThread *data_thread;
    Renderer *renderer;
    
    QVBoxLayout *main_layout;
    QTimer *update_timer;
    QProgressBar *progress_bar;
    CanvasGeneratorThread *gen_thread;
public:
    CanvasGenerator(DataThread *data_thread, Renderer *renderer);
    virtual ~CanvasGenerator();
private slots:
    void update_progress_bar();
};

#endif
