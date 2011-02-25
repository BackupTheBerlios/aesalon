/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/config/GlobalVault.h
*/

#ifndef AesalonConfig_GlobalVault_H
#define AesalonConfig_GlobalVault_H

#include "Vault.h"

namespace Config {

class GlobalVault : public Vault {
private:
	static GlobalVault *m_instance;
public:
	GlobalVault();
	virtual ~GlobalVault() { m_instance = NULL; }
	
	static GlobalVault *instance() { return m_instance; }
};

} // namespace Config

#endif
