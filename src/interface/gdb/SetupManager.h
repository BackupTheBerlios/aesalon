#ifndef AESALON_INTERFACE_GDB_SETUP_MANAGER_H
#define AESALON_INTERFACE_GDB_SETUP_MANAGER_H

#include "StringManager.h"

namespace Aesalon {
namespace Interface {
namespace GDB {

class SetupManager : public StringManager {
public:
    SetupManager();
    virtual ~SetupManager();
};

} // namespace GDB
} // namespace Interface
} // namespace Aesalon

#endif
