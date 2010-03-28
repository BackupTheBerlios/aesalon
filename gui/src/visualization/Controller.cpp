#include "Controller.h"
#include "Controller.moc"

Controller::Controller(Renderer *renderer, DataThread *data_thread) : QThread(NULL), data_thread(data_thread), renderer(renderer) {
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

void Controller::attach() {
    
}

void Controller::render_full() {
    emit clear_canvas();
    qint64 time_range = 0;
    if(data_thread->get_finish_time() == NULL) time_range = data_thread->get_start_time()->ns_until(Timestamp());
    else time_range = data_thread->get_start_time()->ns_until(*data_thread->get_finish_time());
    /* TODO: find a better way than setting the max range to a really high value . . . */
    DataRange range(Timestamp(0), 0, Timestamp(time_range), 9999999999999999999999999.0);
    Canvas *canvas = render_region(range);
    canvas->calculate_data_range();
    emit change_range(canvas->get_range());
    emit canvas_update(canvas);
}

void Controller::update() {
    
}

Canvas *Controller::render_region(const DataRange &range) {
    SnapshotList *snapshot_list = data_thread->get_snapshot_list();
    Snapshot *snapshot = snapshot_list->get_snapshot_for(range.get_begin().get_time_element());
    
    renderer->begin_rendering(range, snapshot);
    snapshot_list->iterate_through(range.get_begin().get_time_element(), range.get_end().get_time_element(), *renderer);
    return renderer->end_rendering();
}
