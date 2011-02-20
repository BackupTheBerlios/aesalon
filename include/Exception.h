/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/Exception.h
*/

#ifndef AesalonException_H
#define AesalonException_H

#include <string>

class Exception {
private:
	std::string m_message;
public:
	Exception(std::string message) : m_message(message) {}
	virtual ~Exception() {}
	
	const std::string &message() const { return m_message; }
};


#endif

