/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/monitor/Aesalon.cpp
*/

#include <iostream>
#include "Exception.h"

#include "monitor/Coordinator.h"
#include "util/MessageSystem.h"

int main(int argc, char *argv[]) {
	try {
		Monitor::Coordinator coordinator(argv);
		coordinator.run();
		return coordinator.returnValue();
	}
	catch(Exception exception) {
		std::cout << exception.message() << std::endl;
	}
	return 0;
}
