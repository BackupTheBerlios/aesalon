#ifndef AESALON_GUI_MAIN_AREA_H
#define AESALON_GUI_MAIN_AREA_H

#include <QWidget>
#include <QTabWidget>

namespace Aesalon {
namespace GUI {

class MainArea : public QTabWidget { Q_OBJECT
public:
    MainArea(QWidget *parent = 0);
    virtual ~MainArea() {}
};

} // namespace GUI
} // namespace Aesalon

#endif
