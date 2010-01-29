#ifndef AESALON_GUI_SESSION_LIST_WIDGET_H
#define AESALON_GUI_SESSION_LIST_WIDGET_H

#include <QListWidget>
#include <QMenu>

#include "Session.h"




class SessionListWidget : public QListWidget { Q_OBJECT
private:
    QMenu *context_menu;
    QMenu *new_context_menu;
    QAction *delete_shortcut;
public:
    SessionListWidget(QWidget *parent = 0);
    virtual ~SessionListWidget() {}
    
    void update_list(QList<Session *> sessions);
public slots:
    void display_context_menu(const QPoint &pos);
    void context_menu_new() { emit new_request(); }
    void context_menu_edit() { emit edit_request(this->currentItem()); }
    void context_menu_delete() { emit delete_request(this->currentItem()); }
signals:
    void new_request();
    void edit_request(QListWidgetItem *item);
    void delete_request(QListWidgetItem *item);
};




#endif
