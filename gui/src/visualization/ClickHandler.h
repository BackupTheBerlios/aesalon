#ifndef AESALON_GUI_VISUALIZATION_CLICK_HANDLER_H
#define AESALON_GUI_VISUALIZATION_CLICK_HANDLER_H

#include <QDialog>

#include "DataPoint.h"
#include "Canvas.h"
#include "session/DataThread.h"

class ClickHandler {
private:
    QDialog *info_box;
public:
    ClickHandler(QDialog *info_box);
    virtual ~ClickHandler();
    
    QDialog *get_info_box() const { return info_box; }
    
    virtual void handle_click(Canvas *canvas, DataPoint at);
};

#endif
