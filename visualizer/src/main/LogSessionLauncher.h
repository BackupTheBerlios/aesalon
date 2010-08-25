#ifndef LogSessionLauncher_H
#define LogSessionLauncher_H

#include <QDialog>
#include <QFileDialog>

class LogSessionLauncher : public QFileDialog { Q_OBJECT
public:
	LogSessionLauncher(QWidget *parent);
	virtual ~LogSessionLauncher();
public:
    virtual void accept();
signals:
	void newTab(QWidget *widget);
};

#endif
