/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/config/GlobalVault.cpp
*/

#include "Config.h"

#include "config/GlobalVault.h"
#include "config/Parser.h"
#include "util/PathSanitizer.h"

namespace Config {

GlobalVault *GlobalVault::m_instance = NULL;

GlobalVault::GlobalVault() {
	m_instance = this;
	Config::Parser parser;
	
	parser.parse(this, Util::PathSanitizer::sanitize(AesalonGlobalConfig));
	parser.parse(this, Util::PathSanitizer::sanitize(AesalonUserConfig));
	parser.parse(this, Util::PathSanitizer::sanitize(AesalonLocalConfig));
}

} // namespace Config
