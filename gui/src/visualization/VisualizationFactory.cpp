#include "VisualizationFactory.h"
#include "ClickHandler.h"

VisualizationFactory::VisualizationFactory(DataThread *data_thread) : data_thread(data_thread) {

}

ClickHandler *VisualizationFactory::create_click_handler(QDialog *info_box) const {
    return new ClickHandler(info_box);
}
