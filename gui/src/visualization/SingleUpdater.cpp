#include "SingleUpdater.h"
#include "SingleUpdater.moc"

SingleUpdater::SingleUpdater(const VisualizationDataRange &range): VisualizationUpdater(), range(range) {
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
