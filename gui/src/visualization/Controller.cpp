#include "Controller.h"

Controller::Controller(DataThread *data_thread): QThread(data_thread), data_thread(data_thread) {

}

Controller::~Controller() {

}

void Controller::run() {
    update_timer = new QTimer();
    connect(update_timer, SIGNAL(timeout()), SLOT(update()));
    /* NOTE: this shouldn't be hardcoded . . . */
    update_timer->start(1000);
    exec();
    delete update_timer;
}

void Controller::change_update_time(int ms) {
    update_timer->setInterval(ms);
}

void Controller::update() {
    
}
