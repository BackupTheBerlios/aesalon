#include "SingleUpdater.h"
#include "SingleUpdater.moc"

SingleUpdater::SingleUpdater(VisualizationDataRange *display_range, const VisualizationDataRange &range)
    : VisualizationUpdater(display_range), range(range) {
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), SLOT(send_update()));
    timer->start(0);
}

SingleUpdater::~SingleUpdater() {

}

void SingleUpdater::send_update() {
    emit move_display(range);
    emit render_region(range);
}
