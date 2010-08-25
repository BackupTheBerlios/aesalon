#ifndef NetworkSessionLauncher_H
#define NetworkSessionLauncher_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

class NetworkSessionLauncher : public QDialog { Q_OBJECT
public:
	NetworkSessionLauncher(QWidget *parent);
	virtual ~NetworkSessionLauncher();
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
