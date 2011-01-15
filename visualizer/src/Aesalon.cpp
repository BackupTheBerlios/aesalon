#include <stdio.h>
#include <QApplication>
#include <QTranslator>

#include "MainWindow.h"

#include "common/Config.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	
	QCoreApplication::setApplicationName("visualizer");
	QCoreApplication::setOrganizationName("aesalon");
	QCoreApplication::setApplicationVersion(AesalonVersion);
	
	Visualizer::MainWindow mw;
	mw.show();
	
	return app.exec();
}
