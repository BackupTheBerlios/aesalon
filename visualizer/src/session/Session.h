#ifndef Session_H
#define Session_H

#include <QIODevice>

#include "SessionDisplay.h"
#include "SessionIOWrapper.h"
#include "module/ModuleMapper.h"
#include "SessionReader.h"

class Session {
public:
	Session(SessionIOWrapper *ioWrapper);
	virtual ~Session();
private:
	SessionIOWrapper *m_ioWrapper;
	SessionDisplay *m_sessionDisplay;
	SessionReader *m_reader;
	ModuleMapper *m_moduleMapper;
public:
	SessionIOWrapper *ioWrapper() const { return m_ioWrapper; }
	SessionDisplay *sessionDisplay() const { return m_sessionDisplay; }
	ModuleMapper *moduleMapper() const { return m_moduleMapper; }
};

#endif