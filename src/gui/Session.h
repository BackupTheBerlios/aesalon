#ifndef AESALON_GUI_SESSION_H
#define AESALON_GUI_SESSION_H

#include <QString>

namespace Aesalon {
namespace GUI {

class Session {
private:
    QString session_name;
public:
    Session(QString session_name = "") : session_name(session_name) {}
    virtual ~Session() {}
    QString get_session_name() const { return session_name; }
    void set_session_name(const QString &new_name) { session_name = new_name; }
};

} // namespace GUI
} // namespace Aesalon

#endif
