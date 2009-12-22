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
    general_layout->addRow(tr("Path to aesalon monitor"), general_aesalon_path);
    general_tab->setLayout(general_layout);
    
    tab_widget->addTab(general_tab, tr("&General"));
    
    main_layout->addWidget(tab_widget, 0, Qt::AlignTop | Qt::AlignLeft);
    
    dialog_buttons = new QHBoxLayout();
    accept_button = new QPushButton(tr("&Accept"));
    connect(accept_button, SIGNAL(clicked()), this, SLOT(accept()));
    dialog_buttons->addWidget(accept_button, 0, Qt::AlignRight);
    cancel_button = new QPushButton(tr("&Cancel"));
    connect(cancel_button, SIGNAL(clicked()), this, SLOT(reject()));
    dialog_buttons->addWidget(cancel_button, 0, Qt::AlignRight);
    
    main_layout->addLayout(dialog_buttons);
    setLayout(main_layout);
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
