#include "ActiveSession.h"
#include "ActiveSession.moc"

namespace Aesalon {
namespace GUI {

ActiveSession::ActiveSession(Session *session, QWidget *parent) : session(session) {
    
}

void ActiveSession::execute() {
    /*emit close_session(this);*/
}

} // namespace GUI
} // namespace Aesalon
