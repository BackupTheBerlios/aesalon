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
	static void writeMessage(MessageLevel level, const std::string &subsystem, const std::string &message);
};

} // namespace Util

namespace MessageSubSystems {
	extern const char *General;
	extern const char *Config;
	extern const char *Storage;
} // namespace MessageSubSystems


/* Usage something like:
	Message(Debug, "The foo has passed bar #" << fooBar << " at baz #" << baz);
	Message2(Debug, Config, "The foo has passed bar #" << fooBar << " at baz #" << baz);
*/

#define Message(level, message) \
	Util::MessageSystem::writeMessage(Util::MessageSystem::level, MessageSubSystems::General, Util::StreamAsString() << message)
#define Message2(level, system, message) \
	Util::MessageSystem::writeMessage(Util::MessageSystem::level, MessageSubSystems::system, Util::StreamAsString() << message)

#define LevelString(level) \
	(level == Debug?("DEBUG"):( \
	level == Log?("LOG"):( \
	level == Warning?("WARN"):"FATAL")))

#endif
