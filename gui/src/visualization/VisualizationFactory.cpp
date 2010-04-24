#include "VisualizationFactory.h"
#include "ClickHandler.h"

VisualizationFactory::VisualizationFactory(DataThread *data_thread) : data_thread(data_thread) {

}

ClickHandler *VisualizationFactory::create_click_handler(QWidget *info_widget) const {
    return new ClickHandler(info_widget);
}
