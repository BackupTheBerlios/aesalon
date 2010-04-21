#include "ClickHandler.h"

ClickHandler::ClickHandler(QDialog *info_box) {
    info_box->setEnabled(false);
}

ClickHandler::~ClickHandler() {
    
}

void ClickHandler::handle_click(Canvas *canvas, DataPoint at) {

}
