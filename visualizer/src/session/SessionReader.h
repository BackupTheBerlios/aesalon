#ifndef SessionReader_H
#define SessionReader_H

#include <QObject>

#include "SessionIOWrapper.h"

class SessionReader : public QObject { Q_OBJECT
public:
	SessionReader(SessionIOWrapper *ioWrapper);
	virtual ~SessionReader();
private:
	QIODevice *m_ioDevice;
	QByteArray m_unprocessed;
public:
	QIODevice *ioDevice() const { return m_ioDevice; }
private slots:
	void readData();
};

#endif
