#include "Initializer.h"
#include "LogSystem.h"

Initializer::Initializer(char *argv[]) : m_argv(argv) {
	m_configuration = new Misc::Configuration(m_argv);
}

Initializer::~Initializer() {
	
}

int Initializer::run() {
	
	return 0;
}
