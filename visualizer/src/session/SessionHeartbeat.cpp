#include "SessionHeartbeat.h"
#include "SessionHeartbeat.moc"

SessionHeartbeat::SessionHeartbeat() {
	m_timer = new QTimer();
	connect(m_timer, SIGNAL(timeout()), SIGNAL(heartbeat()));
	
	m_timer->setSingleShot(false);
	
	setInterval(1000);
}

SessionHeartbeat::~SessionHeartbeat() {
	
}

int SessionHeartbeat::interval() const {
	return m_timer->interval();
}

void SessionHeartbeat::disable() {
	m_timer->stop();
}

void SessionHeartbeat::enable() {
	m_timer->start();
}

void SessionHeartbeat::setInterval(int msec) {
	m_timer->setInterval(msec);
}
