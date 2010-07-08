#ifndef LogSystem_H
#define LogSystem_H

#include <string>

#include "DataTypes.h"

class LogSystem {
public:
	static void logModuleMessage(uint16_t moduleID, std::string message);
};

#endif
