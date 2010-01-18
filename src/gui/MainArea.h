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
public slots:
    void add_new_tab(QWidget *new_tab, QString name) {
        addTab(new_tab, name);
    }
    void remove_tab(QWidget *tab) {
        remove_tab(tab);
    }
};

} // namespace GUI
} // namespace Aesalon

#endif
