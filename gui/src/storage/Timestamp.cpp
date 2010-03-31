/** Aesalon, a project to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009-2010 strange <kawk256@gmail.com>

    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
    Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    @file storage/Timestamp.cpp
*/

#include <time.h>
#include "Timestamp.h"

Timestamp::Timestamp() {
    struct timespec internal_time;
    clock_gettime(TIME_SOURCE, &internal_time);
    ns = internal_time.tv_nsec;
    ns += internal_time.tv_sec * NS_PER_SEC;
}

Timestamp::Timestamp(quint64 ns) : ns(ns) {

}

bool Timestamp::operator<(const Timestamp &other) const {
    if(ns < other.ns) return true;
    else return false;
}

bool Timestamp::operator<=(const Timestamp &other) const {
    if(ns <= other.ns) return true;
    else return false;
}

bool Timestamp::operator>(const Timestamp &other) const {
    if(ns > other.ns) return true;
    else return false;
}

bool Timestamp::operator>=(const Timestamp &other) const {
    if(ns >= other.ns) return true;
    else return false;
}

bool Timestamp::operator==(const Timestamp &other) const {
    if(ns == other.ns) return true;
    else return false;
}

bool Timestamp::operator!=(const Timestamp &other) const {
    if(ns != other.ns) return true;
    else return false;
}

Timestamp &Timestamp::operator=(const Timestamp &other) {
    ns = other.ns;
    return *this;
}

qint64 Timestamp::seconds_until(const Timestamp &other) const {
    return (other.ns - ns) / NS_PER_SEC;
}

qint64 Timestamp::ms_until(const Timestamp &other) const {
    return (other.ns - ns) / NS_PER_MS;
}

qint64 Timestamp::ns_until(const Timestamp &other) const {
    return other.ns - ns;
}

void Timestamp::add_ms(qint64 ms) {
    ns += ms * NS_PER_MS;
}

void Timestamp::add_ns(qint64 ns) {
    this->ns += ns;
}

qint64 Timestamp::to_ns() const {
    return ns;
}

QString Timestamp::to_string() const {
    qint64 seconds = ns / NS_PER_SEC;
    return QString().sprintf("%s%02lli:%02lli.%03lli.%03lli", ns < 0?"-":"", qAbs(seconds / 60),
        qAbs(seconds % 60), qAbs(ns / NS_PER_MS) % 1000, qAbs((ns / 1000) % 1000));
}
