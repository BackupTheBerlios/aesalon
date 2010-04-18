#ifndef AESALON_GUI_VISUALIZATION_FACTORY_H
#define AESALON_GUI_VISUALIZATION_FACTORY_H

#include "Renderer.h"
#include "AxisFormatter.h"
#include "ClickHandler.h"
#include "session/DataThread.h"

class VisualizationFactory {
private:
    DataThread *data_thread;
public:
    VisualizationFactory(DataThread *data_thread);

    DataThread *get_data_thread() const { return data_thread; }

    virtual Renderer *create_renderer(Canvas *canvas) const = 0;
    virtual AxisFormatter *create_formatter() const = 0;
    virtual ClickHandler *create_click_handler() const;
};

#endif
