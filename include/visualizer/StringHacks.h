/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/StringHacks.h
*/

#ifndef AesalonVisualizer_StringHacks_H
#define AesalonVisualizer_StringHacks_H

#include <QString>
#include <ostream>

std::ostream &operator<<(std::ostream &stream, const QString &string);

uint qHash(const std::string &string);

#endif
