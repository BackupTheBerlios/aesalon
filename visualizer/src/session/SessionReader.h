#ifndef SessionReader_H
#define SessionReader_H

#include <QObject>

#include "SessionIOWrapper.h"
#include "module/ModuleMapper.h"

class SessionReader : public QObject { Q_OBJECT
public:
	SessionReader(SessionIOWrapper *ioWrapper, ModuleMapper *moduleMapper);
	virtual ~SessionReader();
private:
	QIODevice *m_ioDevice;
	QByteArray m_unprocessed;
	ModuleMapper *m_moduleMapper;
public:
	QIODevice *ioDevice() const { return m_ioDevice; }
private slots:
	void readData();
};

#endif
