#include <iostream>

#include "ActiveSessionOverview.h"
#include "ActiveSessionOverview.moc"

namespace Aesalon {
namespace GUI {

ActiveSessionOverview::ActiveSessionOverview(Session *session, QWidget *parent) : session(session) {
    info_form = new QFormLayout();
    
    info_form->addRow("Session name: ", new QLabel(session->get_session_name()));
    info_form->addRow("Executable path: ", new QLabel(session->get_executable_path()));
    this->setLayout(info_form);
}

} // namespace GUI
} // namespace Aesalon
