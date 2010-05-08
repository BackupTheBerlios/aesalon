#include <QComboBox>

#include "FilterEditor.h"
#include "FilterEditor.moc"

#include "BooleanAndFilterFactory.h"

FilterEditor::FilterEditor() {
    create_layout = new QVBoxLayout();
    
    QComboBox *type_selection = new QComboBox();
    create_layout->addWidget(type_selection);
    connect(type_selection, SIGNAL(currentIndexChanged(QString)), SLOT(set_factory(QString)));
    
    FilterFactory *factory = new FilterFactory();
    factory_map[factory->get_name()] = factory;
    create_layout->addWidget(factory);
    
    factory = new BooleanAndFilterFactory();
    factory_map[factory->get_name()] = factory;
    type_selection->addItem(factory->get_name());
    this->setMinimumSize(300, 200);
}

FilterEditor::~FilterEditor() {
    
}

Filter *FilterEditor::create_filter() {
    setLayout(create_layout);
    set_factory("NULL");
    
    exec();
    
    return NULL;
}

void FilterEditor::set_factory(QString name) {
    QWidget *widget = factory_map[name];
    if(widget == NULL) return;
    
    int index = create_layout->count() - 1;
    create_layout->removeItem(create_layout->itemAt(index));
    create_layout->addWidget(widget);
    
}
