#include <iostream>

#include <QMenuBar>
#include <QString>
#include <QStatusBar>
#include <QMessageBox>

#include "MainWindow.h"
#include "MainWindow.moc"
#include "ProgramDisplay.h"

namespace Aesalon {
namespace GUI {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->setGeometry(0, 0, 800, 600);
    this->setWindowTitle("Aesalon GUI interface");
    
    create_menus();
    
    mdi_area = new QMdiArea(this);
    setCentralWidget(mdi_area);
    
    try {
        named_pipe = new Aesalon::Platform::NamedPipe(Aesalon::Platform::NamedPipe::READ_PIPE, Misc::StreamAsString() << "/tmp/aesalon_gui-" << getpid(), true);
    }
    catch(Misc::Exception e) {
        std::cout << "Exception: " << e.get_message() << std::endl;
    }
    
    statusBar()->showMessage("Ready.");
}

MainWindow::~MainWindow() {
    delete named_pipe;
}

void MainWindow::create_menus() {
    aesalon_menu = new QMenu(tr("&Aesalon"));
    
    aesalon_menu_new = new QAction(tr("&New program"), this);
    connect(aesalon_menu_new, SIGNAL(triggered()), this, SLOT(new_program()));
    aesalon_menu->addAction(aesalon_menu_new);
    
    aesalon_menu_connect = new QAction(tr("&Connect to program . . ."), this);
    aesalon_menu->addAction(aesalon_menu_connect);
    
    aesalon_menu->addSeparator();
    
    aesalon_menu_quit = new QAction(tr("&Quit"), this);
    connect(aesalon_menu_quit, SIGNAL(triggered()), this, SLOT(quit_requested()));
    aesalon_menu->addAction(aesalon_menu_quit);
    
    menuBar()->addMenu(aesalon_menu);
    
    help_menu = new QMenu(tr("&Help"));
    
    help_menu_about = new QAction(tr("&About Aesalon"), this);
    connect(help_menu_about, SIGNAL(triggered()), this, SLOT(about_aesalon()));
    help_menu->addAction(help_menu_about);
    
    menuBar()->addMenu(help_menu);
}

void MainWindow::quit_requested() {
    if(!mdi_area->subWindowList().size()) this->close();
}

void MainWindow::new_program() {
    ProgramDisplay *new_program = new ProgramDisplay(this);
    mdi_area->addSubWindow(new_program);
    new_program->show();
}

void MainWindow::about_aesalon() {
    QMessageBox::about(this, QString("About Aesalon"), QString(std::string(Misc::StreamAsString() << "Aesalon is a program to graphically display, "
        << "in real-time, the dynamically-allocated memory of a program, both the blocks that have been allocated and "
        << "the references to said blocks.").c_str()));
}

} // namespace GUI
} // namespace Aesalon
