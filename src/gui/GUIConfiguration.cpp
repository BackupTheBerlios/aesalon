#include "GUIConfiguration.h"
#include "GUIConfiguration.moc"

namespace Aesalon {
namespace GUI {

GUIConfiguration::GUIConfiguration(QWidget *parent) : QDialog(parent) {
    main_layout = new QVBoxLayout();
    tab_widget = new QTabWidget();
    
    general_tab = new QWidget();
    general_layout = new QFormLayout();
    general_aesalon_path = new QLineEdit();
    general_layout->addRow(tr("Path to aesalon monitor:"), general_aesalon_path);
    general_tab->setLayout(general_layout);
    general_layout->setSpacing(10);
    
    tab_widget->addTab(general_tab, tr("&General"));
    
    main_layout->addWidget(tab_widget);
    
    dialog_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(dialog_buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(dialog_buttons, SIGNAL(rejected()), this, SLOT(reject()));
    main_layout->addWidget(dialog_buttons);
    main_layout->addStretch(1);
    
    setLayout(main_layout);
    
    setWindowTitle(tr("GUI configuration"));
}

GUIConfiguration::~GUIConfiguration() {
    
}

int GUIConfiguration::exec() {
    general_aesalon_path->setText(settings.value("Core/aesalon-path").toString());
    
    return QDialog::exec();
}

void GUIConfiguration::accept() {
    settings.setValue("Core/aesalon-path", general_aesalon_path->text());
    QDialog::accept();
}

} // namespace GUI
} // namespace Aesalon
