#include <QFile>

#include "LogWrapper.h"

LogWrapper::LogWrapper(QString logFilename) : SessionIOWrapper() {
	m_logfile.setFileName(logFilename);
	setIODevice(&m_logfile);
}

LogWrapper::~LogWrapper() {
	m_logfile.close();
}

void LogWrapper::open() {
	if(m_logfile.exists()) {
		m_logfile.open(QIODevice::ReadOnly);
		emit connected();
		emit readyRead();
		emit disconnected();
	}
	else {
		qWarning("Log file does not exist: \"%s\"", qPrintable(m_logfile.fileName()));
	}
}
