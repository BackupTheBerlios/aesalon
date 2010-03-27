#include "Visualization.h"
#include "Visualization.moc"

Visualization::Visualization() {
    setLayout(new QVBoxLayout());
    
    viewport = new Viewport(this);
    layout()->addWidget(viewport);
}

Visualization::~Visualization() {
    
}
