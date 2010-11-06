/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/Coordinator.h

*/

#ifndef AesalonMonitor_Coordinator_H
#define AesalonMonitor_Coordinator_H

#include "config/Store.h"

namespace Monitor {

class Coordinator {
private:
	static Coordinator *m_instance;
	char **m_argv;
	Config::Store *m_store;
	int m_argumentEndpoint;
public:
	static Coordinator *instance() { return m_instance; }
	Coordinator(char **argv);
	~Coordinator();
	
	char **argv() const { return m_argv; }
	int argumentEndpoint() const { return m_argumentEndpoint; }
	Config::Store *store() const { return m_store; }
	
	void run();
};

} // namespace Monitor

#endif
