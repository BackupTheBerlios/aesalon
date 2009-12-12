#include <iostream>
#include <QMessageBox>
#include "Application.h"
#include "platform/PlatformException.h"

namespace Aesalon {
namespace GUI {

Application::Application(int argc, char *argv[]) : QApplication(argc, argv) {
    QCoreApplication::setApplicationName("Aesalon GUI");
    QCoreApplication::setOrganizationName("Aesalon development team");
    QCoreApplication::setOrganizationDomain("");
}

bool Application::notify(QObject *object, QEvent *event) {
    try {
        return QApplication::notify(object, event);
    }
    catch(Platform::PlatformException pe) {
        QMessageBox::critical(NULL, tr("Platform exception"), QString(pe.get_message().c_str()), 0, 0);
        return false;
    }
    catch(Misc::Exception e) {
        QMessageBox::critical(NULL, tr("Exception"), QString(e.get_message().c_str()), 0, 0);
        return false;
    }
}

} // namespace GUI
} // namespace Aesalon
