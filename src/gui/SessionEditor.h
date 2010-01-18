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

#include "Session.h"

namespace Aesalon {
namespace GUI {

class SessionEditor : public QDialog { Q_OBJECT
private:
    Session *session;
    
    QVBoxLayout *layout;
    QFormLayout *form_layout;
    QLineEdit *session_name;
    QLineEdit *executable_path;

    QDialogButtonBox *button_box;

    void create_widgets();
public:
    SessionEditor(QWidget *parent = 0, Session *session = 0);
    virtual ~SessionEditor() {}
    
    Session *get_session() const { return session; }
public slots:
    void accept();
};

} // namespace GUI
} // namespace Aesalon

#endif
