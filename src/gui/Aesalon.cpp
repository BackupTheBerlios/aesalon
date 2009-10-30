#include <iostream>
#include <QtGui/QApplication>
#include "Framework.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    Aesalon::GUI::Framework *framework = new Aesalon::GUI::Framework;
    framework->show();
    
    return application.exec();
}
