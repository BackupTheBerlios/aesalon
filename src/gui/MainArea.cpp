#include "MainArea.h"
#include "MainArea.moc"
#include "SessionManager.h"

namespace Aesalon {
namespace GUI {

MainArea::MainArea(QWidget *parent) {
    addTab(new SessionManager(), tr("&Session Manager"));
}

} // namespace GUI
} // namespace Aesalon
