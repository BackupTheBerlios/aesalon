#include "Timestamp.h"

bool Timestamp::operator<(const Timestamp &other) const {
    return internal_time < other.internal_time;
}

bool Timestamp::operator<=(const Timestamp &other) const {
    return internal_time <= other.internal_time;
}

bool Timestamp::operator>(const Timestamp &other) const {
    return internal_time > other.internal_time;
}

bool Timestamp::operator>=(const Timestamp &other) const {
    return internal_time >= other.internal_time;
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
    /*qint64 seconds = seconds_until(other);*/
    QTime this_time = internal_time.time();
    QTime other_time = other.internal_time.time();
    
    if(*this < other) {
        if(this_time <= other_time) return this_time.msecsTo(other_time);
        else qCritical("NOTE: comparision between different-day QTimes NYI! (1)");
    }
    else if(*this >= other) {
        if(this_time >= other_time) return -other_time.msecsTo(this_time);
        else qCritical("NOTE: comparision between different-day QTimes NYI! (2)");
    }
    return -1;
}

void Timestamp::add_ms(qint64 ms) {
    internal_time = internal_time.addMSecs(ms);
}

QString Timestamp::to_string() const {
    return internal_time.toString();
}
