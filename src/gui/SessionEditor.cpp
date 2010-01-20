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
    this->setMinimumSize(400, 100);
    this->setWindowTitle(tr("Editing session"));
}

void SessionEditor::create_widgets() {
    layout = new QVBoxLayout();
    
    form_layout = new QFormLayout();
    
    session_name = new QLineEdit(session->get_session_name());
    form_layout->addRow(tr("Session name:"), session_name);
    
    executable_path_layout = new QHBoxLayout();
    executable_path = new QLineEdit(session->get_executable_path());
    executable_path_layout->addWidget(executable_path);
    file_select_dialog = new QPushButton(tr(". . ."));
    connect(file_select_dialog, SIGNAL(pressed()), this, SLOT(show_file_select_dialog()));
    executable_path_layout->addWidget(file_select_dialog);
    form_layout->addRow(tr("Executable path:"), executable_path_layout);
    
    arguments = new QLineEdit(session->get_arguments());
    form_layout->addRow(tr("Arguments:"), arguments);
    
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
}

void SessionEditor::accept() {
    if(session_name->text().length() == 0) return;
    session->set_session_name(session_name->text());
    session->set_executable_path(executable_path->text());
    session->set_arguments(arguments->text());
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

} // namespace GUI
} // namespace Aesalon
