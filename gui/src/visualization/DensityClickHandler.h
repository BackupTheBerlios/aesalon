#ifndef AESALON_GUI_VISUALIZATION_DENSITY_CLICK_HANDLER_H
#define AESALON_GUI_VISUALIZATION_DENSITY_CLICK_HANDLER_H

#include <QLabel>
#include <QFormLayout>

#include "ClickHandler.h"

class DensityClickHandler : public ClickHandler {
private:
    DataThread *data_thread;
    QLabel *address;
    QLabel *size;
    QLabel *alloc_time;
    QLabel *alloc_scope;
    QLabel *release_time;
    QLabel *release_scope;
public:
    DensityClickHandler(DataThread *data_thread, QWidget *info_widget);
    virtual ~DensityClickHandler();
    
    virtual void handle_click(Canvas *canvas, DataPoint at);
};

#endif
