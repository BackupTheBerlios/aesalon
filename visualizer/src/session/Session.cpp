#include "Session.h"

Session::Session(SessionIOWrapper *ioWrapper) : m_ioWrapper(ioWrapper) {
	m_sessionDisplay = new SessionDisplay();
	m_moduleMapper = new ModuleMapper();
	m_reader = new SessionReader(m_ioWrapper, m_moduleMapper);
}

Session::~Session() {
	delete m_reader;
	delete m_moduleMapper;
}

