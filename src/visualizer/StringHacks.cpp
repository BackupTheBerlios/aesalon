/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/StringHacks.cpp
*/

#include "visualizer/StringHacks.h"

std::ostream &operator<<(std::ostream &stream, const QString &string) {
	stream << string.toStdString();
	return stream;
}

uint qHash(const std::string &string) {
	/* Hashing algorithm: djb2. */
	uint hash = 0;
	int c = 0;
	const char *p = string.c_str();
	while((c = (*p++))) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}
