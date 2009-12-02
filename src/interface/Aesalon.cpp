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
    
    @file Aesalon.cpp
    @mainpage Aesalon doxygen documentation
    Aesalon is a program that allows a user to visualize, in real-time, the
    dynamically-allocated memory of a program.
    
    Aesalon was originally designed and written by strange <kawk256@gmail.com>.
    
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
    int return_value = Aesalon::Interface::Initializer::get_instance()->get_return_value();
    if(Aesalon::Interface::Initializer::get_instance()) delete Aesalon::Interface::Initializer::get_instance();
    return return_value;
}
