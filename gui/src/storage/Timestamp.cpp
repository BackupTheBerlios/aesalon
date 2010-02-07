#include "Timestamp.h"

bool Timestamp::operator<(const Timestamp &other) const {
    return time < other.time;
}

bool Timestamp::operator>(const Timestamp &other) const {
    return time > other.time;
}

bool Timestamp::operator==(const Timestamp &other) const {
    return time == other.time;
}

Timestamp &Timestamp::operator=(const Timestamp &other) {
    time = other.time;
    return *this;
}
