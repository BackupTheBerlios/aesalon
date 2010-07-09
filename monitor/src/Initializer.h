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
	static Initializer *singleton() { return m_singleton; }
private:
	static Initializer *m_singleton;
	char **m_argv;
	std::vector<std::string> m_tools;
	Misc::Configuration *m_configuration;
	Module::ModuleMapper *m_moduleMapper;
public:
	Misc::Configuration *configuration() const { return m_configuration; }
	Module::ModuleMapper *moduleMapper() const { return m_moduleMapper; }
	int run();
private:
	void usage();
};

#endif
