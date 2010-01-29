#ifndef AESALON_GUI_MAIN_WINDOW_H
#define AESALON_GUI_MAIN_WINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QMenu>
#include <QMenuBar>




class MainWindow : public QMainWindow { Q_OBJECT
private:
    void setup_menus();
    
    QMenu *aesalon_menu;
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow() {}
public slots:
    void open_configuration();
};




#endif
