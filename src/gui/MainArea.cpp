#include "MainArea.h"
#include "MainArea.moc"
#include "SessionManager.h"

namespace Aesalon {
namespace GUI {

MainArea::MainArea(QWidget *parent) {
    SessionManager *manager = new SessionManager;
    this->addTab(manager, tr("&Session Manager"));
    connect(manager, SIGNAL(new_tab_request(QWidget*, QString)), this, SLOT(add_new_tab(QWidget*, QString)));
}

} // namespace GUI
} // namespace Aesalon
