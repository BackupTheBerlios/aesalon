#ifndef AESALON_GUI_VISUALIZATION_DENSITY_CLICK_HANDLER_H
#define AESALON_GUI_VISUALIZATION_DENSITY_CLICK_HANDLER_H

#include "ClickHandler.h"

class DensityClickHandler : public ClickHandler {
private:
    DataThread *data_thread;
public:
    DensityClickHandler(DataThread *data_thread);
    virtual ~DensityClickHandler();
    
    virtual void handle_click(DataPoint at);
};

#endif
