#ifndef AESALON_INTERFACE_GDB_PROCESSOR_H
#define AESALON_INTERFACE_GDB_PROCESSOR_H

#include "Parser.h"
#include "StreamHandler.h"
#include "platform/BidirectionalPipe.h"
#include "misc/EventQueue.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

/** GDB Processor class. Handles ParseData instances passed on from Parser.
*/
class Processor {
public:
    /** Enum to describe the current state of GDB. */
    enum gdb_state_e {
        GDB_SETUP, /**< GDB is in the "setup" stage -- the program is loaded into memory, but not being executed. */
        GDB_STOPPED, /**< GDB is currently stopped, no program is loaded into memory. */
        GDB_PAUSED, /**< GDB is paused, but a program is loaded into memory. */
        GDB_RUNNING,  /**< GDB is currently running a program that is loaded into memory. */
        GDB_STATES /**< The maximum amount of GDB states. */
    };
private:
    /** The current state of GDB. */
    gdb_state_e gdb_state;

    /** BidirectionalPipe used to communicate with GDB. */
    Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe;
    /** An instance of Parser, used to change the raw output of GDB into ParseData instances. */
    Misc::SmartPointer<Parser> parser;
    /** An EventQueue to push events onto as they occur. */
    Misc::SmartPointer<Misc::EventQueue> event_queue;
    
    Misc::SmartPointer<StreamHandler> stream_handler;
    
    /** The current line to pass onto GDB; stored for reference purposes. */
    std::string line;
    
    /** Handle an AsyncOuput instance, extract any needed data from it.
        @param output The AsyncOutput instance to handle.
    */
    void handle_async(Misc::SmartPointer<AsyncOutput> output);
    /** Handle a ResultRecord, and push events onto @a event_queue as required.
        @param record The ResultRecord instance to handle.
    */
    void handle_result(Misc::SmartPointer<ResultRecord> record);
    
    /** Checks if the current line begins with a specific string.
        @param beginning The beginning to check for.
        @return True if the current line begins with the given string.
    */
    bool begins_with(std::string beginning) {
        return line.substr(0, beginning.length()) == beginning;
    }
public:
    /** General constructor for Processor.
        @param bi_pipe A BidirectionalPipe connected to GDB.
        @param event_queue the EventQueue to push events onto.
    */
    Processor(Misc::SmartPointer<Platform::BidirectionalPipe> bi_pipe, Misc::SmartPointer<Misc::EventQueue> event_queue);
    /** Generic destructor for Processor. */
    virtual ~Processor() {}
    
    /** Process a line from GDB.
        @param line The line from GDB to process.
    */
    void process(std::string line);
    
    /** Get the current state of GDB.
        @return The current state of GDB.
    */
    gdb_state_e get_gdb_state() const { return gdb_state; }
    /** Set the GDB state.
        @param new_state The new state to set the current state to.
    */
    void set_gdb_state(gdb_state_e new_state) { gdb_state = new_state; }
    
    Misc::SmartPointer<StreamHandler> get_stream_handler() const { return stream_handler; }
    void set_stream_handler(Misc::SmartPointer<StreamHandler> new_stream_handler) { stream_handler = new_stream_handler; }
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
