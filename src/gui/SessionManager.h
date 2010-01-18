#ifndef AESALON_GUI_SESSION_MANAGER_H
#define AESALON_GUI_SESSION_MANAGER_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "Session.h"
#include "SessionListWidget.h"
#include "ActiveSession.h"

namespace Aesalon {
namespace GUI {

class SessionManager : public QWidget { Q_OBJECT
private:
    QList<Session *> session_list;
    
    QVBoxLayout *layout;
    SessionListWidget *session_list_view;
    QHBoxLayout *session_button_layout;
    QPushButton *new_session_button;
    QPushButton *edit_session_button;
    QPushButton *launch_session_button;
public:
    SessionManager(QWidget *parent = 0);
    virtual ~SessionManager();
    
    Session *get_session_by_name(const QString &name) const;
public slots:
    void save_sessions();
    void new_session();
    void launch_session(QListWidgetItem *session_item);
    void launch_current_session() { launch_session(session_list_view->currentItem()); }
    void edit_session(QListWidgetItem *session_item);
    void edit_current_session() { edit_session(session_list_view->currentItem()); }
    void delete_session(QListWidgetItem *session_item);
    
    void close_active_session(ActiveSession *session);
signals:
    void new_tab_request(QWidget *new_tab, QString name);
    void remove_tab_request(QWidget *tab);
};

} // namespace GUI
} // namespace Aesalon

#endif
