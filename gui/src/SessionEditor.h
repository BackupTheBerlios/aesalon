#ifndef AESALON_GUI_SESSION_EDITOR_H
#define AESALON_GUI_SESSION_EDITOR_H

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QSpinBox>
#include <QButtonGroup>
#include <QRadioButton>

#include "Session.h"

namespace Aesalon {
namespace GUI {

class SessionEditor : public QDialog { Q_OBJECT
private:
    Session *session;
    
    QVBoxLayout *layout;
    QFormLayout *form_layout;
    QLineEdit *session_name;
    
    QHBoxLayout *session_type_layout;
    QButtonGroup *session_type_group;
    QRadioButton *launch_session_type;
    QRadioButton *connect_session_type;
    QHBoxLayout *executable_path_layout;
    QLabel *executable_path_label;
    QLineEdit *executable_path;
    QPushButton *file_select_dialog;
    QFileDialog *file_dialog;
    QLabel *arguments_label;
    QLineEdit *arguments;
    QSpinBox *port;

    QDialogButtonBox *button_box;
    
    static QString last_directory;

    void create_widgets();
public:
    SessionEditor(QWidget *parent = 0, Session *session = 0);
    virtual ~SessionEditor() {}
    
    Session *get_session() const { return session; }
public slots:
    void accept();
    void show_file_select_dialog();
    void change_selected_file(QString new_filename);
    void change_session_type(Session::session_type_e new_type);
    void session_type_changed(QAbstractButton *new_type) {
        if(new_type == launch_session_type) change_session_type(Session::LAUNCH_SESSION);
        else change_session_type(Session::CONNECT_SESSION);
    }
};

} // namespace GUI
} // namespace Aesalon

#endif
