#ifndef AESALON_GUI_APPLICATION_H
#define AESALON_GUI_APPLICATION_H

#include <QApplication>

namespace Aesalon {
namespace GUI {

class Application : public QApplication {
public:
    Application(int argc, char **argv);
    virtual ~Application() {}
    
    virtual bool notify(QObject *receiver, QEvent *event);
};

} // namespace GUI
} // namespace Aesalon

#endif
