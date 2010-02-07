#include <iostream>
#include <QApplication>
#include "main/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QCoreApplication::setOrganizationName("aesalon");
    QCoreApplication::setApplicationName("gui");
    
    MainWindow mw;
    mw.show();
    return app.exec();
}