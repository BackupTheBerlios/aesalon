/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/util/MessageSystem.h
*/

#ifndef AesalonUtil_MessageSystem_H
#define AesalonUtil_MessageSystem_H

#include <string>

#include "util/StreamAsString.h"

namespace Util {

class MessageSystem {
public:
	enum MessageLevel {
		Debug,
		Log,
		Warning,
		Fatal
	};
public:
	static void writeMessage(MessageLevel level, const std::string &message);
};

} // namespace Util

/* Usage something like:
	Message(Debug, "The foo has passed bar #" << fooBar << " at baz #" << baz);
*/

#define Message(level, message) \
	Util::MessageSystem::writeMessage(level, Util::StreamAsString() << message)

#define Debug Util::MessageSystem::Debug
#define Log Util::MessageSystem::Log
#define Warning Util::MessageSystem::Warning
#define Fatal Util::MessageSystem::Fatal

#define LevelString(level) \
	(level == Debug?("DEBUG"):( \
	level == Log?("LOG"):( \
	level == Warning?("WARN"):"FATAL")))

#endif
