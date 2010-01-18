#include <iostream>
#include <QApplication>
#include "misc/ReferenceCounter.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    new Aesalon::Misc::ReferenceCounter();
    
    QCoreApplication::setOrganizationName("aesalon");
    QCoreApplication::setApplicationName("gui");
    
    Aesalon::GUI::MainWindow *mw = new Aesalon::GUI::MainWindow();
    mw->show();
    int ret_value = app.exec();
    delete mw;
    
    delete Aesalon::Misc::ReferenceCounter::get_instance();
    return ret_value;
}