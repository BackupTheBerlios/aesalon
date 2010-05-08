#include <QComboBox>

#include "FilterEditor.h"
#include "FilterEditor.moc"

FilterEditor::FilterEditor() {
    create_layout = new QFormLayout();
    
}

FilterEditor::~FilterEditor() {
    
}

Filter *FilterEditor::create_filter() {
    this->setLayout(create_layout);
    
    
    
    return NULL;
}
