/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Aesalon.cpp
*/

#include "monitor/Coordinator.h"

int main(int argc, char *argv[]) {
	Monitor::Coordinator coordinator(argv);
	coordinator.run();
	return coordinator.returnValue();
}