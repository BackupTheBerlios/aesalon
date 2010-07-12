#include <QApplication>

#include "main/MainWindow.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	
	QCoreApplication::setOrganizationDomain("aesalon");
	QCoreApplication::setApplicationName("visualizer");
	QCoreApplication::setApplicationVersion(AesalonVersion);
	
	MainWindow mw;
	mw.show();
	return app.exec();
}
