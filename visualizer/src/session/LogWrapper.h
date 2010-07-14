#ifndef LogWrapper_H
#define LogWrapper_H

#include <QFile>

#include "SessionIOWrapper.h"

class LogWrapper : public SessionIOWrapper {
public:
	LogWrapper(QString logFilename);
	virtual ~LogWrapper();
private:
	QFile m_logfile;
public:
	virtual void open();
};

#endif
