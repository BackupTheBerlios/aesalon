/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/util/MessageSystem.cpp
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "util/MessageSystem.h"

namespace Util {

void MessageSystem::writeMessage(MessageSystem::MessageLevel level, const std::string &message) {
	std::time_t t = std::time(NULL);
	
	struct tm *lt = std::localtime(&t);
	
	std::string time = Util::StreamAsString() << lt->tm_hour << ":" << lt->tm_min << ":" << lt->tm_sec;
	
	std::printf("[%02i:%02i:%02i %5s] ", lt->tm_hour, lt->tm_min, lt->tm_sec, LevelString(level));
	std::puts(message.c_str());
	
	if(level == Fatal) abort();
}

} // namespace Util
