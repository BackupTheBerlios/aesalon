#include <stdio.h>
#include <QApplication>
#include <QTranslator>

#include "RootWindow.h"

#include "common/Config.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	
	QCoreApplication::setApplicationName("visualizer");
	QCoreApplication::setOrganizationName("aesalon");
	QCoreApplication::setApplicationVersion(AesalonVersion);
	
	Visualizer::RootWindow rw;
	rw.show();
	
	return app.exec();
}
