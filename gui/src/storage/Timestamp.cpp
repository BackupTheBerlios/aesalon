#include <time.h>
#include "Timestamp.h"

Timestamp::Timestamp() {
    clock_gettime(TIME_SOURCE, &internal_time);
}

Timestamp::Timestamp(quint64 ns) {
    internal_time.tv_sec = ns / 1000000000;
    internal_time.tv_nsec = ns % 1000000000;
}

bool Timestamp::operator<(const Timestamp &other) const {
    if(internal_time.tv_sec < other.internal_time.tv_sec) return true;
    else if(internal_time.tv_sec == other.internal_time.tv_sec && internal_time.tv_nsec < other.internal_time.tv_nsec) return true;
    else return false;
}

bool Timestamp::operator<=(const Timestamp &other) const {
    if(internal_time.tv_sec < other.internal_time.tv_sec) return true;
    else if(internal_time.tv_sec == other.internal_time.tv_sec && internal_time.tv_nsec <= other.internal_time.tv_nsec) return true;
    else return false;
}

bool Timestamp::operator>(const Timestamp &other) const {
    if(internal_time.tv_sec > other.internal_time.tv_sec) return true;
    else if(internal_time.tv_sec == other.internal_time.tv_sec && internal_time.tv_nsec > other.internal_time.tv_nsec) return true;
    else return false;
}

bool Timestamp::operator>=(const Timestamp &other) const {
    if(internal_time.tv_sec > other.internal_time.tv_sec) return true;
    else if(internal_time.tv_sec == other.internal_time.tv_sec && internal_time.tv_nsec >= other.internal_time.tv_nsec) return true;
    else return false;
}

bool Timestamp::operator==(const Timestamp &other) const {
    if(internal_time.tv_sec == other.internal_time.tv_sec && internal_time.tv_nsec == other.internal_time.tv_nsec) return true;
    else return false;
}

bool Timestamp::operator!=(const Timestamp &other) const {
    if(internal_time.tv_sec != other.internal_time.tv_sec || internal_time.tv_nsec != other.internal_time.tv_nsec) return true;
    else return false;
}

Timestamp &Timestamp::operator=(const Timestamp &other) {
    internal_time = other.internal_time;
    return *this;
}

qint64 Timestamp::seconds_until(const Timestamp& other) const {
    return other.internal_time.tv_sec - internal_time.tv_sec;
}

qint64 Timestamp::ms_until(const Timestamp& other) const {
    qint64 ms = seconds_until(other) * 1000;
    ms += (other.internal_time.tv_nsec - internal_time.tv_nsec) / 1000000;
    return ms;
}

qint64 Timestamp::ns_until(const Timestamp& other) const {
    qint64 ns = seconds_until(other) * 1000000000;
    ns += (other.internal_time.tv_nsec - internal_time.tv_nsec);
    return ns;
}

void Timestamp::add_ms(qint64 ms) {
    internal_time.tv_nsec += ms * 1000000;
    while(internal_time.tv_nsec >= 1000000000) internal_time.tv_nsec -= 1000000000, internal_time.tv_sec ++;
    /*internal_time = internal_time.addMSecs(ms);*/
}

QString Timestamp::to_string() const {
    return QString().sprintf("%li:%li", internal_time.tv_sec, internal_time.tv_nsec);
}
