#include "VisualizationCanvas.h"
#include "VisualizationCanvas.moc"

VisualizationCanvas::VisualizationCanvas(QObject *parent): QGraphicsScene(parent) {
    setFont(QFont("DejaVu Sans", 8));
}

VisualizationCanvas::~VisualizationCanvas() {
    
}
