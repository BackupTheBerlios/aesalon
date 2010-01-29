#ifndef AESALON_ACITVE_SESSION_MEMORY_H
#define AESALON_ACITVE_SESSION_MEMORY_H

#include <QObject>

class ActiveSessionMemory : public QObject { Q_OBJECT
private:
    QByteArray unprocessed;
public:
    ActiveSessionMemory(QObject *parent = NULL);
    virtual ~ActiveSessionMemory() {}
public slots:
    void process_data(QByteArray data);
};

#endif
