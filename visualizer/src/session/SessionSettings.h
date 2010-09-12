#ifndef SessionSettings_H
#define SessionSettings_H

#include <QDialog>
#include <QSpinBox>

#include "Session.h"

class SessionSettings : public QDialog {
public:
	SessionSettings(QWidget *parent, Session *session);
	virtual ~SessionSettings();
private:
	Session *m_session;
	QLineEdit *m_sessionName;
	QSpinBox *m_heartbeatTime;
public:
	virtual void accept();
};

#endif
