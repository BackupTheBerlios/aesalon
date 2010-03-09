#ifndef AESALON_GUI_STORAGE_TIMESTAMP_H
#define AESALON_GUI_STORAGE_TIMESTAMP_H

#include <QDateTime>
#include <time.h>

#ifndef TIME_SOURCE
    #define TIME_SOURCE CLOCK_REALTIME
#endif

class Timestamp {
private:
    struct timespec internal_time;
public:
    Timestamp();
    Timestamp(const Timestamp &other) : internal_time(other.internal_time) {}
    Timestamp(quint64 ns);
    
    bool operator<(const Timestamp &other) const;
    bool operator<=(const Timestamp &other) const;
    bool operator>(const Timestamp &other) const;
    bool operator>=(const Timestamp &other) const;
    bool operator==(const Timestamp &other) const;
    bool operator!=(const Timestamp &other) const;
    Timestamp &operator=(const Timestamp &other);
    qint64 seconds_until(const Timestamp &other) const;
    qint64 ms_until(const Timestamp &other) const;
    qint64 ns_until(const Timestamp &other) const;
    void add_ms(qint64 ms);
    QString to_string() const;
};

#endif
