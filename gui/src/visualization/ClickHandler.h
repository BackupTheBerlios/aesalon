#ifndef AESALON_GUI_VISUALIZATION_CLICK_HANDLER_H
#define AESALON_GUI_VISUALIZATION_CLICK_HANDLER_H

#include "DataPoint.h"
#include "Canvas.h"
#include "session/DataThread.h"

class ClickHandler {
public:
    virtual void handle_click(Canvas *canvas, DataPoint at);
};

#endif
