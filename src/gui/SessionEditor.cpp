#include "SessionEditor.h"
#include "SessionEditor.moc"

namespace Aesalon {
namespace GUI {

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
    executable_path = new QLineEdit(session->get_executable_path());
    form_layout->addRow(tr("Executable path:"), executable_path);
    
    layout->addLayout(form_layout);
    
    button_box = new QDialogButtonBox();
    button_box->addButton(QDialogButtonBox::Save);
    button_box->addButton(QDialogButtonBox::Cancel);
    
    connect(button_box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), this, SLOT(reject()));
    
    button_box->setCenterButtons(false);
    
    layout->addWidget(button_box);
    
    this->setLayout(layout);
}

void SessionEditor::accept() {
    session->set_session_name(session_name->text());
    session->set_executable_path(executable_path->text());
    QDialog::accept();
}

} // namespace GUI
} // namespace Aesalon
