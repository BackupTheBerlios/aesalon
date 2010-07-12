#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>

class MainWindow : public QMainWindow { Q_OBJECT
public:
	MainWindow();
	virtual ~MainWindow();
private:
	
public:
	
private:
	void setupMenus();
private slots:
	void about();
};

#endif
