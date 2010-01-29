#ifndef AESALON_GUI_SESSION_H
#define AESALON_GUI_SESSION_H

#include <QString>




class Session {
public:
    enum session_type_e {
        LAUNCH_SESSION,
        CONNECT_SESSION
    };
private:
    QString session_name;
    QString executable_path;
    QString arguments;
    int port;
    session_type_e session_type;
public:
    Session() : session_name(""), executable_path(""), arguments(""), port(0), session_type(LAUNCH_SESSION) {}
    virtual ~Session() {}
    
    QString get_session_name() const { return session_name; }
    void set_session_name(const QString &new_name) { session_name = new_name; }
    
    QString get_executable_path() const { return executable_path; }
    void set_executable_path(const QString &new_path) { executable_path = new_path; }
    
    QString get_arguments() const { return arguments; }
    void set_arguments(const QString &new_arguments) { arguments = new_arguments; }
    
    session_type_e get_session_type() const { return session_type; }
    void set_session_type(session_type_e new_session_type) { session_type = new_session_type; }
    
    int get_port() const { return port; }
    void set_port(int new_port) { port = new_port; }
};




#endif
