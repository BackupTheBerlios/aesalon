/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/Coordinator.h
*/

#ifndef AesalonMonitor_Coordinator_H
#define AesalonMonitor_Coordinator_H

#include "config/Vault.h"

namespace Monitor {

class Coordinator {
private:
	static Coordinator *m_instance;
	char **m_argv;
	int m_argcOffset;
	Config::Vault *m_vault;
	int m_returnValue;
public:
	static Coordinator *instance() { return m_instance; }
	Coordinator(char **argv);
	~Coordinator();
	
	char **argv() const { return m_argv; }
	Config::Vault *vault() const { return m_vault; }
	int argcOffset() const { return m_argcOffset; }
	int returnValue() const { return m_returnValue; }
	void setReturnValue(int newValue) { m_returnValue = newValue; }
	
	void run();
private:
	void parseConfigs();
	void usage(bool displayHelp);
};

} // namespace Monitor

#endif

