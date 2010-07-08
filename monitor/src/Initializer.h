#ifndef Initializer_H
#define Initializer_H

#include <string>
#include <vector>

#include "misc/Configuration.h"
#include "module/ModuleMapper.h"

class Initializer {
public:
	Initializer(char *argv[]);
	~Initializer();
private:
	char **m_argv;
	std::vector<std::string> m_tools;
	Misc::Configuration *m_configuration;
	Module::ModuleMapper *m_moduleMapper;
public:
	int run();
};

#endif
