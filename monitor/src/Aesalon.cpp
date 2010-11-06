#include <iostream>
#include "common/Exception.h"
#include "config/Store.h"
#include "config/ArgumentParser.h"
#include "config/FileParser.h"

int main(int argc, char *argv[]) {
	try {
		Monitor::Config::Store s;
		Monitor::Config::FileParser fp(&s);
		fp.parse(".aesalon.conf");
		Monitor::Config::ArgumentParser ap(&s, argv);
		ap.parse();
		std::cout << s.item("followForks")->value() << std::endl;
	}
	catch(Common::Exception exception) {
		std::cout << exception.message() << std::endl;
	}
	return 0;
}
