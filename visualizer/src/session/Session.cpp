#include "Session.h"

Session::Session(SessionIOWrapper *ioWrapper) : m_ioWrapper(ioWrapper) {
	m_heartbeat = new SessionHeartbeat();
	m_moduleMapper = new ModuleMapper();
	m_sessionDisplay = new SessionDisplay(this);
	m_reader = new SessionReader(m_ioWrapper, m_moduleMapper);
	
	m_ioWrapper->open();
	if(m_ioWrapper->hasHeartbeat()) {
		QObject::connect(m_ioWrapper, SIGNAL(connected()), m_heartbeat, SLOT(enable()));
	}
	QObject::connect(m_ioWrapper, SIGNAL(disconnected()), m_heartbeat, SLOT(disable()));
	
	/* NOTE: debugging code. */
	m_sessionDisplay->addColumn();
	m_sessionDisplay->addRow();
}

Session::~Session() {
	delete m_reader;
	delete m_moduleMapper;
}
