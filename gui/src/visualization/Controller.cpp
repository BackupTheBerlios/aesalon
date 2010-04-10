#include "Controller.h"
#include "Controller.moc"

Controller::Controller(VisualizationFactory *factory) : QThread(NULL),
    data_thread(factory->get_data_thread()), renderer(factory->create_renderer()) {
    update_timer = NULL;
}

Controller::~Controller() {

}

void Controller::run() {
    update_timer = NULL;
    exec();
    delete update_timer;
}

void Controller::change_update_time(int ms) {
    if(!update_timer) {
        update_timer = new QTimer();
        update_timer->setInterval(1000);
        connect(update_timer, SIGNAL(timeout()), this, SLOT(rt_update()));
    }
    update_timer->setInterval(ms);
}

void Controller::begin_rt() {
    if(!update_timer) {
        update_timer = new QTimer();
        update_timer->setInterval(1000);
        connect(update_timer, SIGNAL(timeout()), this, SLOT(rt_update()));
    }
    if(data_thread->get_finish_time() != NULL || data_thread->get_start_time() == NULL) return;
    update_timer->start();
    emit clear_canvas();
    last_update = data_thread->get_start_time()->ns_until(Timestamp());
    emit shift_range_to(Timestamp(last_update));
}

void Controller::end_rt() {
    if(update_timer) update_timer->stop();
}

void Controller::render_full() {
    if(data_thread->get_start_time() == NULL) return;
    qint64 time_range = 0;
    if(data_thread->get_finish_time() == NULL) time_range = data_thread->get_start_time()->ns_until(Timestamp());
    else time_range = data_thread->get_start_time()->ns_until(*data_thread->get_finish_time());
    /* TODO: find a better way than setting the max range to a really high value . . . */
    DataRange range(Timestamp(0), 0, Timestamp(time_range), 1e50);
    Canvas *canvas = render_region(range);
    canvas->calculate_data_range();
    emit change_range(canvas->get_range());
    emit clear_canvas();
    emit canvas_update(canvas);
}

void Controller::rt_update() {
    if(data_thread->get_finish_time()) {
        end_rt();
        emit shift_range_to(Timestamp(data_thread->get_start_time()->ns_until(*data_thread->get_finish_time())));
        return;
    }
    qint64 now = data_thread->get_start_time()->ns_until(Timestamp());
    Timestamp from(last_update), to(now);
    last_update = now;
    DataRange range(from, 0, to, 1e50);
    emit shift_range_to(to);
    emit canvas_update(render_region(range));
}

Canvas *Controller::render_region(const DataRange &range) {
    SnapshotList *snapshot_list = data_thread->get_snapshot_list();
    Snapshot *snapshot = snapshot_list->get_snapshot_for(range.get_begin().get_time_element());
    
    renderer->begin_rendering(range, snapshot);
    snapshot_list->iterate_through(range.get_begin().get_time_element(), range.get_end().get_time_element(), *renderer);
    return renderer->end_rendering();
}
