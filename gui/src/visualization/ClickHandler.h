#ifndef AESALON_GUI_VISUALIZATION_CLICK_HANDLER_H
#define AESALON_GUI_VISUALIZATION_CLICK_HANDLER_H

#include <QDialog>

#include "DataPoint.h"
#include "Canvas.h"
#include "session/DataThread.h"

class ClickHandler {
private:
    QWidget *info_widget;
public:
    ClickHandler(QWidget *info_widget);
    virtual ~ClickHandler();
    
    QWidget *get_info_widget() const { return info_widget; }
    
    virtual void handle_click(Canvas *canvas, DataPoint at);
};

#endif
