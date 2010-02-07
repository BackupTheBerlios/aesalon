#ifndef AESALON_GUI_STORAGE_TIMESTAMP_H
#define AESALON_GUI_STORAGE_TIMESTAMP_H

#include <QDateTime>

class Timestamp {
private:
    QDateTime time;
public:
    Timestamp() : time(QDateTime::currentDateTime()) {}
    Timestamp(const Timestamp &other) : time(other.time) {}
    bool operator<(const Timestamp &other) const;
    bool operator>(const Timestamp &other) const;
    bool operator==(const Timestamp &other) const;
    Timestamp &operator=(const Timestamp &other);
};

#endif
