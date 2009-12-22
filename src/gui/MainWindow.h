#ifndef AESALON_GUI_MAIN_WINDOW_H
#define AESALON_GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMdiArea>

#include "AesalonConfiguration.h"
#include "GUIConfiguration.h"

#include "misc/SmartPointer.h"

namespace Aesalon {
namespace GUI {

class MainWindow : public QMainWindow { Q_OBJECT
private:
    QMenu *aesalon_menu;
    QAction *aesalon_menu_new;
    QAction *aesalon_menu_connect;
    QAction *aesalon_menu_quit;
    QMenu *tools_menu;
    QAction *tools_menu_config_aesalon;
    QAction *tools_menu_config_gui;
    QMenu *help_menu;
    QAction *help_menu_about;
    QMdiArea *mdi_area;
    
    AesalonConfiguration *aesalon_configuration;
    GUIConfiguration *gui_configuration;
    
    void create_menus();
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
private slots:
    void quit_requested();
    void new_program();
    void about_aesalon();
};

} // namespace GUI
} // namespace Aesalon

#endif
