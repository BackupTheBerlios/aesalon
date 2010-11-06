#include <iostream>
#include "common/Exception.h"
#include "Coordinator.h"

int main(int argc, char *argv[]) {
	try {
		Monitor::Coordinator coordinator(argv);
		coordinator.run();
	}
	catch(Common::Exception exception) {
		std::cout << exception.message() << std::endl;
	}
	return 0;
}
