#ifndef AESALON_GUI_STORAGE_SCOPE_H
#define AESALON_GUI_STORAGE_SCOPE_H

#include <QString>

class Scope {
private:
    QString name;
public:
    Scope(const QString &name = "");
    
    const QString &get_name() const { return name; }
    void set_name(const QString &new_name) { name = new_name; }
};

#endif