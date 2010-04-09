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
    
    @file Initializer.h
    This file contains the defintion of the Initializer class.
*/

#ifndef AESALON_MONITOR_INITIALIZER_H
#define AESALON_MONITOR_INITIALIZER_H

#include "misc/Singleton.h"
#include "tcp/ServerSocket.h"
#include "event/Queue.h"
#include "ProgramManager.h"
#include "misc/ArgumentParser.h"
#include "misc/ConfigParser.h"
#include "analyzer/Interface.h"

/** Initializer class. Basically, handles initialization of the Monitor. */
class Initializer : public Misc::Singleton<Initializer> {
private:
    /** A copy of argv, used for initializing the ArgumentParser. */
    char **argv;
    /** The socket used to communicate with the GUI. */
    TCP::ServerSocket *server_socket;
    /** The EventQueue instance; used to keep the local copy of the program's memory updated. */
    Event::Queue *event_queue;
    
    ProgramManager *program_manager;
    
    Misc::ArgumentParser *argument_parser;
    
    Misc::ConfigParser *config_parser;
    
    Analyzer::Interface *analyzer_interface;
    
    /** Initialize the aesalon monitor. */
    void initialize();
    /** Deinitialize the aesalon monitor. */
    void deinitialize();
    
    /** Print version information .*/
    void version();
    /** Print usage information about aesalon, including a list of all flags. */
    void usage();
    
    /** Main execution loop. */
    void run();
    
    int return_value;
public:
    /** Constructor for the Initializer class. This is where the magic begins.
        @param argv argv, as passed into main().
    */
    Initializer(char **argv);
    /** Destructor for the Initializer class. Here is where the magic ends. */
    virtual ~Initializer();
    
    /** Returns a SmartPointer to the named pipe; mostly used internally but
        public for other classes to use when required.
        @return The named pipe created by this aesalon gdb instance.
    */
    TCP::ServerSocket *get_socket() const { return server_socket; }
    
    ProgramManager *get_program_manager() const { return program_manager; }
    Event::Queue *get_event_queue() const { return event_queue; }
    Misc::ArgumentParser *get_argument_parser() const { return argument_parser; }
    Analyzer::Interface *get_analyzer_interface() const { return analyzer_interface; }
    
    int get_return_value() const { return return_value; }
    void set_return_value(int new_return_value) { return_value = new_return_value; }
};

#endif
