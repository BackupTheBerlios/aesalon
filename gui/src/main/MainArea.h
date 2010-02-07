#ifndef AESALON_GUI_MAIN_AREA_H
#define AESALON_GUI_MAIN_AREA_H

#include <QWidget>
#include <QTabWidget>

class MainArea : public QTabWidget { Q_OBJECT
public:
    MainArea(QWidget *parent = 0);
    virtual ~MainArea() {}
private slots:
    void tab_close_requested(int index);
public slots:
    void add_tab(QWidget *new_tab, QString name);
    void remove_tab(QWidget *tab);
};

#endif
