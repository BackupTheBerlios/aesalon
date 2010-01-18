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
    /*this->setGeometry(300, 300, 300, 200);*/
}

void SessionEditor::create_widgets() {
    layout = new QFormLayout();
    
    session_name = new QLineEdit(session->get_session_name());
    layout->addRow(tr("Session name:"), session_name);
    
    button_box = new QDialogButtonBox();
    button_box->addButton(QDialogButtonBox::Save);
    button_box->addButton(QDialogButtonBox::Cancel);
    
    connect(button_box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(button_box, SIGNAL(rejected()), this, SLOT(reject()));
    
    layout->addWidget(button_box);
    
    this->setLayout(layout);
}

void SessionEditor::accept() {
    session->set_session_name(session_name->text());
    QDialog::accept();
}

} // namespace GUI
} // namespace Aesalon
