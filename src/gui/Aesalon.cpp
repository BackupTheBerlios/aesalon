#include <iostream>
#include <QtGui/QApplication>
#include "Framework.h"

#include "misc/ReferenceCounter.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    
    new Aesalon::Misc::ReferenceCounter();
    
    Aesalon::GUI::Framework *framework = new Aesalon::GUI::Framework;
    framework->show();
    
    int ret_value = application.exec();
    
    delete Aesalon::Misc::ReferenceCounter::get_instance();
    
    return ret_value;
}
