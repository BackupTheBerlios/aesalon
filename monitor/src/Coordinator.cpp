/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file monitor/src/Coordinator.cpp

*/

#include "Coordinator.h"
#include "config/ArgumentParser.h"
#include "common/Config.h"

namespace Monitor {

Coordinator::Coordinator(char **argv) : m_argv(argv) {
	
}

Coordinator::~Coordinator() {
	
}

void Coordinator::run() {
	m_store = new Config::Store();
	Config::ArgumentParser ap(m_store, m_argv);
	m_argumentEndpoint = ap.parse();
	
	
}

} // namespace Monitor
