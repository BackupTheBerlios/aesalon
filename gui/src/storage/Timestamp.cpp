#include "Timestamp.h"

bool Timestamp::operator<(const Timestamp &other) const {
    return internal_time < other.internal_time;
}

bool Timestamp::operator>(const Timestamp &other) const {
    return internal_time > other.internal_time;
}

bool Timestamp::operator==(const Timestamp &other) const {
    return internal_time == other.internal_time;
}

Timestamp &Timestamp::operator=(const Timestamp &other) {
    internal_time = other.internal_time;
    return *this;
}

qint64 Timestamp::seconds_until(const Timestamp& other) const {
    return internal_time.secsTo(other.internal_time);
}

qint64 Timestamp::ms_until(const Timestamp& other) const {
    qint64 seconds = seconds_until(other);
    QTime t = internal_time.time();
    QTime t2 = other.internal_time.time();
    
    if(*this < other) {
        if(t < t2) return t.msecsTo(t2);
        else qCritical("NOTE: comparision between different-day QTimes NYI!");
    }
    else {
        if(t > t2) return t.msecsTo(t2);
        else qCritical("NOTE: comparision between different-day QTimes NYI!");
    }
    return -1;
}
