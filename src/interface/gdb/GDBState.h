#ifndef AESALON_INTERFACE_GDB_GDBSTATE_H
#define AESALON_INTERFACE_GDB_GDBSTATE_H

namespace Aesalon {
namespace Interface {
namespace GDB {

enum GDBState {
    GDB_SETUP, /**< GDB is currently being set up, initialization is in progress. */
    GDB_RUNNING, /**< GDB is currently running. No commands can be executed during this state. */
    GDB_PAUSED, /**< GDB is paused, but has been running previously, and setup is complete. */
    GDB_FINISHED /**< GDB has finished executing the program. */
};
} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
