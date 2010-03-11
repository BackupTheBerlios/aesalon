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
    
    @file exception/TCPException.h
*/

#ifndef AESALON_TCP_EXCEPTION_H
#define AESALON_TCP_EXCEPTION_H

#include <string>

#include "BasicException.h"
#include "misc/StreamAsString.h"

namespace Exception {

/** Exception class, thrown when the TCP library encounters a problem. */
class TCPException : public BasicException {
public:
    TCPException(std::string message) :
        BasicException(Misc::StreamAsString() << "TCP exception: " << message) {
    }
};

}

#endif
