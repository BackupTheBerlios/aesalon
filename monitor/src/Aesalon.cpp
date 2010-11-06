#include <iostream>
#include "common/Exception.h"
#include "config/Store.h"
#include "config/ArgumentParser.h"

int main(int argc, char *argv[]) {
	try {
		Monitor::Config::Store s;
		Monitor::Config::ArgumentParser ap(&s, argv);
		ap.parse();
	}
	catch(Common::Exception exception) {
		std::cout << exception.message() << std::endl;
	}
	return 0;
}
