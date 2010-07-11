#ifndef Initializer_H
#define Initializer_H

#include <string>
#include <vector>

#include "misc/Configuration.h"
#include "module/ModuleMapper.h"
#include "program/Launcher.h"
#include "program/Controller.h"
#include "program/Analyzer.h"

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
	Program::Launcher *m_launcher;
	Program::Controller *m_controller;
	Program::Analyzer *m_analyzer;
public:
	Misc::Configuration *configuration() const { return m_configuration; }
	Module::ModuleMapper *moduleMapper() const { return m_moduleMapper; }
	Program::Launcher *launcher() const { return m_launcher; }
	int run();
private:
	void usage();
};

#endif
