#include "Session.h"

Session::Session(SessionIOWrapper *ioWrapper) : m_ioWrapper(ioWrapper) {
	m_sessionDisplay = new SessionDisplay();
	
}

Session::~Session() {

}
