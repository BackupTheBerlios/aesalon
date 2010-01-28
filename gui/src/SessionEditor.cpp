#include <stdlib.h>
#include "SessionEditor.h"
#include "SessionEditor.moc"

namespace Aesalon {
namespace GUI {

QString SessionEditor::last_directory = getenv("HOME");

SessionEditor::SessionEditor(QWidget *parent, Session *session) : QDialog(parent), session(session) {
    if(this->session == NULL) {
        this->session = new Session();
    }
    create_widgets();
    this->setModal(true);
    QSizePolicy expanding;
    expanding.setHorizontalPolicy(QSizePolicy::Expanding);
    expanding.setVerticalPolicy(QSizePolicy::Expanding);
    this->setSizePolicy(expanding);
    this->setMinimumSize(600, 100);
    this->setWindowTitle(tr("Editing session"));
}

void SessionEditor::create_widgets() {
    layout = new QVBoxLayout();
    
    form_layout = new QFormLayout();
    
    session_name = new QLineEdit(session->get_session_name());
    form_layout->addRow(tr("Session name:"), session_name);
    
    session_type_layout = new QHBoxLayout();
    session_type_group = new QButtonGroup();
    session_type_group->setExclusive(true);
    launch_session_type = new QRadioButton(tr("&Launch local program"));
    connect_session_type = new QRadioButton(tr("&Connect to remote monitor"));
    
    if(session->get_session_type() == Session::LAUNCH_SESSION) launch_session_type->setChecked(true);
    else connect_session_type->setChecked(true);
    
    session_type_group->addButton(launch_session_type);
    session_type_group->addButton(connect_session_type);
    connect(session_type_group, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(session_type_changed(QAbstractButton*)));
    
    session_type_layout->addWidget(launch_session_type);
    session_type_layout->addWidget(connect_session_type);
    form_layout->addRow(tr("Session type:"), session_type_layout);
    
    executable_path_layout = new QHBoxLayout();
    executable_path = new QLineEdit(session->get_executable_path());
    executable_path_layout->addWidget(executable_path);
    file_select_dialog = new QPushButton(tr(". . ."));
    connect(file_select_dialog, SIGNAL(pressed()), this, SLOT(show_file_select_dialog()));
    executable_path_layout->addWidget(file_select_dialog);
    executable_path_label = new QLabel(tr("Executable path:"));
    form_layout->addRow(executable_path_label, executable_path_layout);
    
    arguments = new QLineEdit(session->get_arguments());
    arguments_label = new QLabel(tr("Arguments:"));
    form_layout->addRow(arguments_label, arguments);
    
    port = new QSpinBox();
    port->setMinimum(1025);
    port->setMaximum(65535);
    if(session->get_port()) port->setValue(session->get_port());
    else port->setValue(DEFAULT_PORT);
    form_layout->addRow(tr("Port:"), port);
    
    layout->addLayout(form_layout);
    
    button_box = new QDialogButtonBox();
    button_box->addButton(QDialogButtonBox::Save);
    button_box->addButton(QDialogButtonBox::Cancel);
    
    connect(button_box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), this, SLOT(reject()));
    
    button_box->setCenterButtons(false);
    
    layout->addWidget(button_box);
    
    file_dialog = new QFileDialog();
    file_dialog->setDirectory(last_directory);
    connect(file_dialog, SIGNAL(fileSelected(QString)), this, SLOT(change_selected_file(QString)));
    
    this->setLayout(layout);
    
    this->change_session_type(session->get_session_type());
}

void SessionEditor::accept() {
    if(session_name->text().length() == 0) return;
    session->set_session_name(session_name->text());
    session->set_executable_path(executable_path->text());
    session->set_arguments(arguments->text());
    session->set_port(port->value());
    if(launch_session_type->isChecked()) session->set_session_type(Session::LAUNCH_SESSION);
    else session->set_session_type(Session::CONNECT_SESSION);
    QDialog::accept();
}

void SessionEditor::show_file_select_dialog() {
    file_dialog->exec();
}

void SessionEditor::change_selected_file(QString new_filename) {
    executable_path->setText(new_filename);
    last_directory = new_filename.left(new_filename.lastIndexOf('/'));
    file_dialog->setDirectory(last_directory);
}

void SessionEditor::change_session_type(Session::session_type_e new_type) {
    if(new_type == Session::LAUNCH_SESSION) {
        executable_path_label->setText(tr("Executable path:"));
        file_select_dialog->show();
        arguments->show();
        arguments_label->show();
    }
    else {
        executable_path_label->setText(tr("Host:"));
        file_select_dialog->hide();
        arguments->hide();
        arguments_label->hide();
    }
}

} // namespace GUI
} // namespace Aesalon
