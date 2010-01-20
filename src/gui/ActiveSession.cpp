#include "ActiveSession.h"
#include "ActiveSession.moc"
#include "ActiveSessionOverview.h"

namespace Aesalon {
namespace GUI {

ActiveSession::ActiveSession(Session *session, QWidget *parent) : QTabWidget(parent), session(session) {
    this->setTabPosition(West);
    this->addTab(new ActiveSessionOverview(session), "&Overview");
    
    memory = new Platform::Memory();
}

ActiveSession::~ActiveSession() {
    delete memory;
}

void ActiveSession::execute() {
    /*emit close_session(this);*/
}

} // namespace GUI
} // namespace Aesalon
