#include <iostream>

#include <QMenuBar>
#include <QString>
#include <QStatusBar>

#include "MainWindow.h"
#include "MainWindow.moc"
#include "ProgramDisplay.h"

namespace Aesalon {
namespace GUI {

MainWindow::MainWindow(QWidget *parent) {
    this->setGeometry(0, 0, 800, 600);
    this->setWindowTitle("Aesalon GUI interface");
    
    create_menus();
    
    mdi_area = new QMdiArea(this);
    setCentralWidget(mdi_area);
    
    statusBar()->showMessage("Ready.");
}

void MainWindow::create_menus() {
    aesalon_menu = new QMenu(tr("&Aesalon"));
    
    aesalon_menu_new = new QAction(tr("&New program"), this);
    connect(aesalon_menu_new, SIGNAL(triggered()), this, SLOT(new_program()));
    aesalon_menu->addAction(aesalon_menu_new);
    
    aesalon_menu_quit = new QAction(tr("&Quit"), this);
    connect(aesalon_menu_quit, SIGNAL(triggered()), this, SLOT(quit_requested()));
    aesalon_menu->addAction(aesalon_menu_quit);
    
    menuBar()->addMenu(aesalon_menu);
}

void MainWindow::quit_requested() {
    std::cout << "Quit requested." << std::endl;
}

void MainWindow::new_program() {
    ProgramDisplay *new_program = new ProgramDisplay(this);
    mdi_area->addSubWindow(new_program);
    new_program->show();
}

} // namespace GUI
} // namespace Aesalon
