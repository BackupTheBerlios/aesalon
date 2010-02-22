#ifndef AESALON_GUI_STORAGE_TIMESTAMP_H
#define AESALON_GUI_STORAGE_TIMESTAMP_H

#include <QDateTime>

class Timestamp {
private:
    QDateTime internal_time;
public:
    Timestamp() : internal_time(QDateTime::currentDateTime()) {}
    Timestamp(const Timestamp &other) : internal_time(other.internal_time) {}
    bool operator<(const Timestamp &other) const;
    bool operator>(const Timestamp &other) const;
    bool operator==(const Timestamp &other) const;
    Timestamp &operator=(const Timestamp &other);
    qint64 seconds_until(const Timestamp &other) const;
    qint64 ms_until(const Timestamp &other) const;
    void add_ms(qint64 ms);
};

#endif
