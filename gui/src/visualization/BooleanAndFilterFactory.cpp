#include <QFormLayout>
#include <QLabel>

#include "BooleanAndFilterFactory.h"
#include "BooleanAndFilter.h"

BooleanAndFilterFactory::BooleanAndFilterFactory() {
    QFormLayout *main_layout = new QFormLayout();
    
    main_layout->addWidget(new QLabel(tr("Boolean AND operator: all children must be true for this to be true.")));
    setLayout(main_layout);
}

BooleanAndFilterFactory::~BooleanAndFilterFactory() {
    
}

Filter *BooleanAndFilterFactory::create_filter() {
    return new BooleanAndFilter();
}

QString BooleanAndFilterFactory::get_name() const {
    return "Operator (All)";
}
