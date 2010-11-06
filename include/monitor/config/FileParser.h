/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/config/FileParser.h

*/

#include <string>

#ifndef AesalonMonitor_Config_FileParser_H
#define AesalonMonitor_Config_FileParser_H

#include "Store.h"

namespace Monitor {
namespace Config {

class FileParser {
private:
	Store *m_store;
public:
	FileParser(Store *store);
	~FileParser();
	
	void parse(const std::string &filename);
};

} // namespace Config
} // namespace Monitor

#endif
