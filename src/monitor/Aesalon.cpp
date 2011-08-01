/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Aesalon.cpp
*/
#include <stdlib.h>
#include <iostream>

#include "monitor/Coordinator.h"
#include "config/GlobalVault.h"
#include "storage/Mempool.h"

int main(int argc, char *argv[]) {
	Storage::Mempool::create();
	Config::GlobalVault gv;
	Monitor::Coordinator coordinator(argv);
	coordinator.run();
	
	Storage::Mempool::destroy();
	return coordinator.returnValue();
}
