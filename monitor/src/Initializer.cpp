#include <iostream>

#include "Initializer.h"
#include "LogSystem.h"

Initializer::Initializer(char *argv[]) : m_argv(argv) {
	m_configuration = new Misc::Configuration(m_argv);
}

Initializer::~Initializer() {
	
}

int Initializer::run() {
	if(m_configuration->configItems()["help"]->boolValue()) {
		usage();
		return 0;
	}
	return 0;
}

void Initializer::usage() {
	std::cout << "Aesalon version " << AesalonVersion << ". Copyright (C) 2010 strange." << std::endl;
	std::cout << "usage: " << m_argv[0] << " [options] [--] filename [arguments]" << std::endl;
	std::cout << "Options:" << std::endl;
	for(Misc::Configuration::ConfigItems::iterator i = m_configuration->configItems().begin();
		i != m_configuration->configItems().end(); i ++) {
		if(i->second == NULL) continue;
		std::cout << "\t--" << i->second->name() << ": " << i->second->description() << std::endl;
	}
}
