#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QTabWidget>

class MainWindow : public QMainWindow { Q_OBJECT
public:
	MainWindow();
	virtual ~MainWindow();
private:
	QTabWidget *m_centralWidget;
public slots:
	void addTab(QWidget *widget);
private:
	void setupMenus();
private slots:
	void createLauncher();
	void about();
};

#endif
