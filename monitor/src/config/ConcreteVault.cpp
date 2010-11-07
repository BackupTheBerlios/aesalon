/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/config/ConcreteVault.cpp

*/

#include "config/ConcreteVault.h"

namespace Monitor {
namespace Config {

void ConcreteVault::set(const std::string &key, const std::string &value) {
	m_data[key] = value;
}

std::string ConcreteVault::get(const std::string &key) {
	return m_data[key];
}

} // namespace Config
} // namespace Monitor
