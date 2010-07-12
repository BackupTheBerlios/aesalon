#ifndef SessionLauncher_H
#define SessionLauncher_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

class SessionLauncher : public QDialog { Q_OBJECT
public:
	SessionLauncher(QWidget *parent);
	virtual ~SessionLauncher();
private:
	QLineEdit *m_host;
	QSpinBox *m_port;
public slots:
	virtual void accept();
private:
	void setupDialog();
signals:
	void newTab(QWidget *widget);
};

#endif
