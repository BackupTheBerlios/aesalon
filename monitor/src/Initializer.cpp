#include "Initializer.h"
#include "LogSystem.h"

Initializer::Initializer(char *argv[]) : m_argv(argv) {
	
}

Initializer::~Initializer() {
	
}

int Initializer::run() {
	m_configuration = new Misc::Configuration(m_argv);
	
	
	return 0;
}

void Initializer::handleArguments() {
	
}
