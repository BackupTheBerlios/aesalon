#ifndef Session_H
#define Session_H

#include <QIODevice>

#include "SessionDisplay.h"
#include "SessionIOWrapper.h"

class Session {
public:
	Session(SessionIOWrapper *ioWrapper);
	virtual ~Session();
private:
	SessionIOWrapper *m_ioWrapper;
	SessionDisplay *m_sessionDisplay;
public:
	SessionIOWrapper *ioWrapper() const { return m_ioWrapper; }
	SessionDisplay *sessionDisplay() const { return m_sessionDisplay; }
};

#endif
