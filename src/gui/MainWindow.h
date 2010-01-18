#ifndef AESALON_GUI_MAIN_WINDOW_H
#define AESALON_GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMenu>
#include <QMenuBar>

namespace Aesalon {
namespace GUI {

class MainWindow : public QMainWindow { Q_OBJECT
private:
    void setup_menus();
    
    QMenu *aesalon_menu;
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow() {}
};

} // namespace GUI
} // namespace Aesalon

#endif
