#include "Controller.h"
#include "Controller.moc"

Controller::Controller(Renderer *renderer, DataThread *data_thread) : QThread(NULL), data_thread(data_thread), renderer(renderer) {
}

Controller::~Controller() {

}

void Controller::run() {
    update_timer = new QTimer();
    update_timer->setInterval(1000);
    connect(update_timer, SIGNAL(timeout()), SLOT(rt_update()));
    exec();
    delete update_timer;
}

void Controller::change_update_time(int ms) {
    update_timer->setInterval(ms);
}

void Controller::begin_rt() {
    if(data_thread->get_finish_time() != NULL || data_thread->get_start_time() == NULL) return;
    qDebug("begin_rt() . . .");
    update_timer->start();
    qDebug("update timer interval is %lli", update_timer->interval());
    emit clear_canvas();
    last_update = data_thread->get_start_time()->ns_until(Timestamp());
    emit shift_range_to(Timestamp(last_update));
}

void Controller::end_rt() {
    update_timer->stop();
}

void Controller::render_full() {
    if(data_thread->get_start_time() == NULL) return;
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

void Controller::rt_update() {
    qDebug("Updating . . .");
    qint64 now = data_thread->get_start_time()->ns_until(Timestamp());
    Timestamp from(last_update), to(now);
    last_update = now;
    DataRange range(from, 0, to, 9999999999999999999999999.0);
    emit shift_range_to(to);
    emit render_region(range);
}

Canvas *Controller::render_region(const DataRange &range) {
    SnapshotList *snapshot_list = data_thread->get_snapshot_list();
    Snapshot *snapshot = snapshot_list->get_snapshot_for(range.get_begin().get_time_element());
    
    renderer->begin_rendering(range, snapshot);
    snapshot_list->iterate_through(range.get_begin().get_time_element(), range.get_end().get_time_element(), *renderer);
    return renderer->end_rendering();
}
