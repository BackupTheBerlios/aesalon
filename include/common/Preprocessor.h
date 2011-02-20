/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/Preprocessor.h

*/

#ifndef AesalonCommon_Preprocessor_H
#define AesalonCommon_Preprocessor_H

#include "monitor/config/Vault.h"

namespace Common {

typedef void (*Preprocessor)(Monitor::Config::Vault *vault);

} // namespace Common

#endif
