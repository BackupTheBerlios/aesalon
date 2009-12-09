#include <iostream>
#include <QtGui/QApplication>
#include "MainWindow.h"
#include "Application.h"

#include "misc/ReferenceCounter.h"

int main(int argc, char *argv[]) {
    Aesalon::GUI::Application application(argc, argv);
    
    new Aesalon::Misc::ReferenceCounter();
    
    Aesalon::GUI::MainWindow *main_window = new Aesalon::GUI::MainWindow();
    main_window->show();
    
    int ret_value = application.exec();
    
    delete main_window;
    delete Aesalon::Misc::ReferenceCounter::get_instance();
    
    return ret_value;
}
