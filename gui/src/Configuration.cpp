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
    connect(aesalon_path_file_selector, SIGNAL(fileSelected(QString)), this, SLOT(change_aesalon_path(QString)));
    
    aesalon_path_layout->addWidget(aesalon_path_select_dialog);
    
    form_layout->addRow("Path to aesalon:", aesalon_path_layout);
    
    xterm_path_layout = new QHBoxLayout();
    xterm_path = new QLineEdit();
    xterm_path->setText(settings.value("xterm-path").toString());
    xterm_path_layout->addWidget(xterm_path);
    
    xterm_path_file_selector =  new QFileDialog();
    xterm_path_file_selector->setDirectory(getenv("HOME"));
    
    xterm_path_select_dialog = new QPushButton(tr(". . ."));
    connect(xterm_path_select_dialog, SIGNAL(clicked(bool)), xterm_path_file_selector, SLOT(exec()));
    connect(xterm_path_file_selector, SIGNAL(fileSelected(QString)), this, SLOT(change_xterm_path(QString)));
    
    xterm_path_layout->addWidget(xterm_path_select_dialog);
    
    form_layout->addRow("X terminal to use:", xterm_path_layout);
    
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
    settings.setValue("xterm-path", xterm_path->text());
    QDialog::accept();
}

} // namespace GUI
} // namespace Aesalon
