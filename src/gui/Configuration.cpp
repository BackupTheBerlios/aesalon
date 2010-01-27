#include <QSettings>
#include "Configuration.h"
#include "Configuration.moc"

namespace Aesalon {
namespace GUI {

Configuration::Configuration(QWidget *parent) : QDialog(parent) {
    QSizePolicy expanding;
    expanding.setHorizontalPolicy(QSizePolicy::Expanding);
    expanding.setVerticalPolicy(QSizePolicy::Expanding);
    this->setSizePolicy(expanding);
    this->setMinimumSize(600, 100);
    this->setWindowTitle(tr("Configuration"));

    QSettings settings;
    main_layout = new QVBoxLayout();
    form_layout = new QFormLayout();
    
    aesalon_path_layout = new QHBoxLayout();
    aesalon_path = new QLineEdit();
    aesalon_path->setText(settings.value("aesalon-path").toString());
    aesalon_path_layout->addWidget(aesalon_path);
    
    aesalon_path_file_selector =  new QFileDialog();
    aesalon_path_file_selector->setDirectory(getenv("HOME"));
    
    aesalon_path_select_dialog = new QPushButton(tr(". . ."));
    connect(aesalon_path_select_dialog, SIGNAL(clicked(bool)), aesalon_path_file_selector, SLOT(exec()));
    
    aesalon_path_layout->addWidget(aesalon_path_select_dialog);
    
    form_layout->addRow("Path to aesalon", aesalon_path_layout);
    main_layout->addLayout(form_layout);
    
    button_box = new QDialogButtonBox();
    button_box->addButton(QDialogButtonBox::Save);
    button_box->addButton(QDialogButtonBox::Cancel);
    button_box->setCenterButtons(false);
    connect(button_box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), this, SLOT(reject()));
    
    main_layout->addWidget(button_box);
    
    setLayout(main_layout);
}

void Configuration::accept() {
    QSettings settings;
    settings.setValue("aesalon-path", aesalon_path->text());
    QDialog::accept();
}

} // namespace GUI
} // namespace Aesalon
