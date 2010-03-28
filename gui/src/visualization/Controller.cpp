#include "Controller.h"

Controller::Controller(Renderer *renderer, DataThread *data_thread): QThread(data_thread), data_thread(data_thread), renderer(renderer) {

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

void Controller::render_region(const DataRange &range) {
    SnapshotList *snapshot_list = data_thread->get_snapshot_list();
    Snapshot *snapshot = snapshot_list->get_snapshot_for(range.get_begin().get_time_element());
    
    renderer->begin_rendering(range, snapshot);
    snapshot_list->iterate_through(range.get_begin().get_time_element(), range.get_end().get_time_element(), *renderer);
    renderer->end_rendering();
}
