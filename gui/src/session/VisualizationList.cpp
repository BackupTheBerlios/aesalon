#include "VisualizationList.h"
#include "VisualizationList.moc"
#include "visualizations/ActiveBlocksVisualization.h"

VisualizationList::VisualizationList() {
    sortItems(Qt::AscendingOrder);
    addItem(ActiveBlocksVisualization::get_static_title());
}

VisualizationList::~VisualizationList() {

}

Visualization* VisualizationList::create_from_selected(DataThread *data_thread) const {
    if(currentItem() == NULL) return NULL;
    QString title = currentItem()->text();
    /* NOTE: Find a better way! */
    if(title == ActiveBlocksVisualization::get_static_title()) return new ActiveBlocksVisualization(data_thread);
    else return NULL;
}
