/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/ArgumentParser.h
*/

#ifndef AesalonMonitor_ArgumentParser_H
#define AesalonMonitor_ArgumentParser_H

#include "config/Vault.h"

namespace Monitor {

class ArgumentParser {
public:
	static int parse(Config::Vault *vault, char **argv);
};

} // namespace Monitor

#endif

