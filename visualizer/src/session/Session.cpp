#include "Session.h"

Session::Session(SessionIOWrapper *ioWrapper) : m_ioWrapper(ioWrapper) {
	m_moduleMapper = new ModuleMapper();
	m_sessionDisplay = new SessionDisplay(m_moduleMapper);
	m_reader = new SessionReader(m_ioWrapper, m_moduleMapper);
	
	m_ioWrapper->open();
	m_sessionDisplay->addColumn();
	m_sessionDisplay->addRow();
	
}

Session::~Session() {
	delete m_reader;
	delete m_moduleMapper;
}
