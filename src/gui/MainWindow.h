#ifndef AESALON_GUI_MAIN_WINDOW_H
#define AESALON_GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMdiArea>

namespace Aesalon {
namespace GUI {

class MainWindow : public QMainWindow { Q_OBJECT
private:
    QMenu *aesalon_menu;
    QAction *aesalon_menu_new;
    QAction *aesalon_menu_quit;
    QMdiArea *mdi_area;
    
    void create_menus();
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow() {}
private slots:
    void quit_requested();
    void new_program();
};

} // namespace GUI
} // namespace Aesalon

#endif
