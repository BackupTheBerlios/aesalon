#include "VisualizationController.h"
#include "VisualizationController.moc"

VisualizationController::VisualizationController(DataThread *data_thread, VisualizationRenderer *renderer,
    QObject *parent) : QThread(parent), data_thread(data_thread), renderer(renderer) {
    
}

VisualizationController::~VisualizationController() {

}

void VisualizationController::run() {
    exec();
}

void VisualizationController::update_region(const VisualizationDataRange &range) {
    /* TODO: implement fancy caching etc. in here */
    /*VisualizationRenderer *renderer = display->get_renderer();*/
    SnapshotList *snapshot_list = data_thread->get_snapshot_list();
    Snapshot *snapshot = snapshot_list->get_snapshot_for(range.get_lower_time());
    renderer->begin_update(snapshot);
    snapshot_list->iterate_through(range.get_lower_time(), range.get_upper_time(), *renderer);
    renderer->end_update();
}
