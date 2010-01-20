#include <iostream>

#include "ActiveSessionOverview.h"
#include "ActiveSessionOverview.moc"

namespace Aesalon {
namespace GUI {

ActiveSessionOverview::ActiveSessionOverview(Session *session, QWidget *parent) : session(session) {
    info_form = new QFormLayout();
    
    info_form->addRow(tr("Session name:"), new QLabel(session->get_session_name()));
    info_form->addRow(tr("Executable path:"), new QLabel(session->get_executable_path()));
    info_form->addRow(tr("Arguments:"), new QLabel(session->get_arguments()));
    info_form->addRow(tr("Port:"), new QLabel(QString().setNum(session->get_port())));
    /* Use an empty QLabel as a separator. */
    info_form->addWidget(new QLabel());
    status = new QLabel();
    info_form->addRow(tr("Status:"), status);
    
    this->setLayout(info_form);
}

void ActiveSessionOverview::update_status(QString new_status) {
    status->setText(new_status);
}

} // namespace GUI
} // namespace Aesalon
