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
    normalize();
}

void Timestamp::add_ns(qint64 ns) {
    internal_time.tv_nsec += ns;
    normalize();
}

QString Timestamp::to_string() const {
    return QString().sprintf("%c%02li:%02li.%03li.%03li", internal_time.tv_sec < 0?'-':' ', qAbs(internal_time.tv_sec / 60),
        qAbs(internal_time.tv_sec % 60), qAbs(internal_time.tv_nsec / 1000000), qAbs((internal_time.tv_nsec / 1000) % 1000));
}

void Timestamp::normalize() {
    while(internal_time.tv_nsec >= 1000000000) internal_time.tv_nsec -= 1000000000, internal_time.tv_sec ++;
    while(internal_time.tv_nsec < 0) internal_time.tv_nsec += 1000000000, internal_time.tv_sec --;
}
