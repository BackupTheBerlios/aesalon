#include "VisualizationSettings.h"
#include "VisualizationSettings.moc"
#include "SingleUpdater.h"

VisualizationSettings::VisualizationSettings(QWidget *parent) : QDialog(parent) {
    main_layout = new QVBoxLayout(this);
    tab_widget = new QTabWidget();
    
    normal_panel = new QWidget(this);
    tab_widget->addTab(normal_panel, tr("&Normal updating"));
    realtime_panel = new QWidget(this);
    tab_widget->addTab(realtime_panel, tr("&Real-time updating"));
    
    lower_buttons = new QDialogButtonBox(this);
    lower_buttons->addButton(QDialogButtonBox::Ok);
    connect(lower_buttons, SIGNAL(accepted()), SLOT(accept()));
    connect(lower_buttons, SIGNAL(rejected()), SLOT(reject()));
    
    main_layout->addWidget(tab_widget);
    main_layout->addWidget(lower_buttons);
    setLayout(main_layout);
}

VisualizationSettings::~VisualizationSettings() {
    
}

void VisualizationSettings::accept() {
    if(tab_widget->currentWidget() == normal_panel) {
        /*emit change_updater(new SingleUpdater());*/
    }
    else {
        emit change_updater(new VisualizationUpdater());
    }
    QDialog::accept();
}
