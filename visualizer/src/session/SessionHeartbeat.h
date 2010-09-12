#ifndef SessionHeartbeat_H
#define SessionHeartbeat_H

#include <QObject>
#include <QTimer>

class SessionHeartbeat : public QObject { Q_OBJECT
public:
	SessionHeartbeat();
	~SessionHeartbeat();
private:
	QTimer *m_timer;
public slots:
	int interval() const;
	void setInterval(int msec);
	void disable();
	void enable();
signals:
	void heartbeat();
};

#endif
