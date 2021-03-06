/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/Coordinator.h
*/

#ifndef AesalonMonitor_Coordinator_H
#define AesalonMonitor_Coordinator_H

#include <vector>

#include "config/Vault.h"
#include "monitor/DataOutputController.h"
#include "MarshalList.h"
#include "SymbolResolver.h"
#include "storage/Mempool.h"
#include "Launcher.h"

namespace Monitor {

class Coordinator {
private:
	static Coordinator *m_instance;
	char **m_argv;
	int m_argcOffset;
	int m_returnValue;
	DataOutputController *m_dataOutputController;
	MarshalList *m_marshalList;
	SymbolResolver *m_resolver;
	Launcher *m_launcher;
public:
	static Coordinator *instance() { return m_instance; }
	Coordinator(char **argv);
	~Coordinator();
	
	char **argv() const { return m_argv; }
	int argcOffset() const { return m_argcOffset; }
	int returnValue() const { return m_returnValue; }
	void setReturnValue(int newValue) { m_returnValue = newValue; }
	DataOutputController *dataOutputController() const { return m_dataOutputController; }
	MarshalList *marshalList() const { return m_marshalList; }
	SymbolResolver *resolver() const { return m_resolver; }
	Launcher *launcher() const { return m_launcher; }
	
	void run();
private:
	void setupModuleIDs();
	void usage(bool displayHelp);
};

} // namespace Monitor

#endif
