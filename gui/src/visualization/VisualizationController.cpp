#include "VisualizationController.h"
#include "VisualizationController.moc"

VisualizationController::VisualizationController(DataThread *data_thread, VisualizationRenderer *renderer,
    QObject *parent) : QThread(parent), data_thread(data_thread), renderer(renderer), updater(NULL) {
    
}

VisualizationController::~VisualizationController() {

}

void VisualizationController::run() {
    exec();
}

void VisualizationController::change_updater(VisualizationUpdater *new_updater) {
    if(updater) {
        disconnect(this, SIGNAL(move_display(VisualizationDataRange)));
        disconnect(updater, SIGNAL(render_region(VisualizationDataRange)), this, SLOT(update_region(VisualizationDataRange)));
        updater->deleteLater();
    }
    updater = new_updater;
    connect(updater, SIGNAL(move_display(VisualizationDataRange)), SIGNAL(move_display(VisualizationDataRange)));
    connect(updater, SIGNAL(move_display(Timestamp,Timestamp)), SLOT(move_display(Timestamp,Timestamp)));
    connect(updater, SIGNAL(render_region(VisualizationDataRange)), SLOT(update_region(VisualizationDataRange)));
    connect(updater, SIGNAL(render_region(Timestamp,Timestamp)), SLOT(update_region(Timestamp,Timestamp)));
}

void VisualizationController::move_display(const Timestamp &start, const Timestamp &end) {
    VisualizationDataRange range = renderer->get_range();
    range.set_lower_time(start);
    range.set_upper_time(end);
    emit move_display(range);
}

void VisualizationController::update_region(const VisualizationDataRange &region) {
    /* TODO: implement fancy caching etc. in here */
    SnapshotList *snapshot_list = data_thread->get_snapshot_list();
    Snapshot *snapshot = snapshot_list->get_snapshot_for(region.get_lower_time());
    renderer->begin_update(snapshot);
    snapshot_list->iterate_through(region.get_lower_time(), region.get_upper_time(), *renderer);
    renderer->end_update();
}

void VisualizationController::update_region(const Timestamp &start, const Timestamp &end) {
    VisualizationDataRange range = renderer->get_range();
    range.set_lower_time(start);
    range.set_upper_time(end);
    emit update_region(range);
}
