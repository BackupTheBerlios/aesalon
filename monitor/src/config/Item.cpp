/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/config/Item.cpp

*/

#include <sstream>

#include "config/Item.h"
#include "common/ParsingException.h"
#include "common/StreamAsString.h"

namespace Monitor {
namespace Config {

Item::Item(const std::string &name) : m_name(name) {

}

Item::~Item() {

}

void Item::setValue(const std::string &value) {
	m_value = value;
}

void Item::setValue(long longValue) {
	std::ostringstream ss;
	ss << longValue;
	m_value = ss.str();
}

void Item::setValue(bool boolValue) {
	if(boolValue) m_value = "true";
	else m_value = "false";
}

long Item::longValue() const {
	std::istringstream ss(m_value);
	
	long value;
	if((ss >> value) == false) {
		throw Common::ParsingException(Common::StreamAsString()
			<< "Cannot convert config item " << m_name
			<< " (value: \"" << m_value << "\") to integer.");
	}
	
	return value;
}

bool Item::boolValue() const {
	if(m_value == "true" || m_value == "True") return true;
	if(m_value == "false" || m_value == "false") return false;
	
	throw Common::ParsingException(Common::StreamAsString()
		<< "Cannot convert config item " << m_name
		<< " (value: \"" << m_value << "\") to boolean.");
}

} // namespace Config
} // namespace Monitor
