/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/Launcher.h
*/

#ifndef AesalonMonitor_Launcher_H
#define AesalonMonitor_Launcher_H

#include <sys/types.h>
#include "SHMReader.h"

namespace Monitor {

class Launcher {
	char **m_argv;
	pid_t m_targetPid;
	SHMReader *m_shmReader;
public:
	Launcher(char **argv);
	~Launcher();
	
	SHMReader *shmReader() const { return m_shmReader; }
	
	void launch();
	
private:
	void forkTarget();
	void waitForChild();
	void setupEnvironment();
};

} // namespace Monitor

#endif
