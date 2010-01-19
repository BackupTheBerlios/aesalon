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

#include "Session.h"

namespace Aesalon {
namespace GUI {

class SessionEditor : public QDialog { Q_OBJECT
private:
    Session *session;
    
    QVBoxLayout *layout;
    QFormLayout *form_layout;
    QLineEdit *session_name;
    QHBoxLayout *executable_path_layout;
    QLineEdit *executable_path;
    QPushButton *file_select_dialog;
    QFileDialog *file_dialog;
    QLineEdit *arguments;

    QDialogButtonBox *button_box;

    void create_widgets();
public:
    SessionEditor(QWidget *parent = 0, Session *session = 0);
    virtual ~SessionEditor() {}
    
    Session *get_session() const { return session; }
public slots:
    void accept();
    void show_file_select_dialog();
    void change_selected_file(QString new_filename);
};

} // namespace GUI
} // namespace Aesalon

#endif
