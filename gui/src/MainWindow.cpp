#include <QApplication>
#include "MainWindow.h"
#include "MainWindow.moc"

#include "MainArea.h"
#include "Configuration.h"




MainWindow::MainWindow(QWidget *parent) {
    QSettings settings;
    setup_menus();
    setCentralWidget(new MainArea());
    this->showMaximized();
}

void MainWindow::setup_menus() {
    aesalon_menu = new QMenu(tr("&Aesalon"));
    aesalon_menu->addAction("&Configuration . . .", this, SLOT(open_configuration()));
    aesalon_menu->addAction("&Quit", this, SLOT(close()));
    menuBar()->addMenu(aesalon_menu);
    
}

void MainWindow::open_configuration() {
    Configuration *config = new Configuration(this);
    config->exec();
}



