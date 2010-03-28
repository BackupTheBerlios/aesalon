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
    
    @file storage/Timestamp.h
*/

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
    void add_ns(qint64 ns);
    QString to_string() const;
private:
    void normalize();
};

#endif
