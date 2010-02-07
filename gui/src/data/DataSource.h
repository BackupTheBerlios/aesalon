#ifndef AESALON_GUI_DATA_SOURCE_H
#define AESALON_GUI_DATA_SOURCE_H

#include <QString>

#include "DataReceiver.h"

class DataSource {
private:
    QString name;
public:
    DataSource(QString name) : name(name) {}
    virtual ~DataSource();
    
    const QString &get_name() const { return name; }
    void set_name(const QString &new_name) { name = new_name; }
    
    virtual DataReceiver *new_receiver() = 0;
};

#endif
