#include "VisualizationSettings.h"
#include "VisualizationSettings.moc"
#include "SingleUpdater.h"
#include "RealtimeUpdater.h"

VisualizationSettings::VisualizationSettings(VisualizationDataRange *display_range, QWidget *parent)
    : QDialog(parent), display_range(display_range) {
    setMinimumSize(640, 300);
    setWindowTitle(tr("Visualization settings"));
    
    main_layout = new QVBoxLayout(this);
    tab_widget = new QTabWidget();
    
    normal_panel = new QWidget(this);
    
    normal_layout = new QFormLayout();
    normal_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    
    normal_panel->setLayout(normal_layout);
    
    tab_widget->addTab(normal_panel, tr("&Normal updating"));
    
    realtime_panel = new QWidget(this);
    realtime_layout = new QFormLayout();
    realtime_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    
    realtime_history_length = new QSpinBox();
    realtime_history_length->setAlignment(Qt::AlignRight);
    /* Minimum history length of 100 ms */
    realtime_history_length->setMinimum(100);
    /* Maximum history length of 1000000 ms, or 1000s, or 16.666 minutes. */
    realtime_history_length->setMaximum(1000000);
    realtime_history_length->setSuffix(tr("ms"));
    
    realtime_cycle_time = new QSpinBox();
    realtime_cycle_time->setAlignment(Qt::AlignRight);
    /* Set a maximum of 10 updates per second */
    realtime_cycle_time->setMinimum(100);
    /* Minimum of once per 10 seconds? (NOTE: might want to change this) */
    realtime_cycle_time->setMaximum(10000);
    realtime_cycle_time->setSuffix(tr("ms"));
    
    realtime_layout->addRow(tr("History length:"), realtime_history_length);
    realtime_layout->addRow(tr("Update cycle time:"), realtime_cycle_time);
    realtime_panel->setLayout(realtime_layout);
    
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
        emit change_updater(new RealtimeUpdater(display_range, realtime_history_length->value(), realtime_cycle_time->value()));
    }
    QDialog::accept();
}
