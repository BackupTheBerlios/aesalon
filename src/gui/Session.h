#ifndef AESALON_GUI_SESSION_H
#define AESALON_GUI_SESSION_H

#include <QString>

namespace Aesalon {
namespace GUI {

class Session {
private:
    QString session_name;
    QString executable_path;
public:
    Session() : session_name(""), executable_path("") {}
    virtual ~Session() {}
    QString get_session_name() const { return session_name; }
    void set_session_name(const QString &new_name) { session_name = new_name; }
    
    QString get_executable_path() const { return executable_path; }
    void set_executable_path(const QString &new_path) { executable_path = new_path; }
};

} // namespace GUI
} // namespace Aesalon

#endif
