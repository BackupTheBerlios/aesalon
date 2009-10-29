/** Aesalon, a program to visualize dynamic memory allocations in real-time.
    Copyright (C) 2009 

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
*/

#include <iostream>
#include "Initializer.h"
#include "misc/Exception.h"

int main(int argc, char *argv[]) {
    try {
        new Aesalon::Interface::Initializer(argv);
    }
    catch(Aesalon::Misc::Exception e) {
        std::cout << e.get_message() << std::endl;
        return 1;
    }
    if(Aesalon::Interface::Initializer::get_instance()) delete Aesalon::Interface::Initializer::get_instance();
    return 0;
}
