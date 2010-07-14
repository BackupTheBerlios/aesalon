#ifndef SessionIOWrapper_H
#define SessionIOWrapper_H

#include <QIODevice>

class SessionIOWrapper : public QObject { Q_OBJECT
public:
	SessionIOWrapper();
	virtual ~SessionIOWrapper();
private:
	QIODevice *m_ioDevice;
public:
	QIODevice *ioDevice() const { return m_ioDevice; }
	
	virtual void open() = 0;
	
protected:
	void setIODevice(QIODevice *ioDevice) { m_ioDevice = ioDevice; }
signals:
	void connected();
	void disconnected();
};

#endif
