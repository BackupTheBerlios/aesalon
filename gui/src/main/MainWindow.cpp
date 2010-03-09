#include <QApplication>
#include <QMetaType>

#include "MainWindow.h"
#include "MainWindow.moc"
#include "MainArea.h"
#include "Configuration.h"
#include "storage/Timestamp.h"

MainWindow::MainWindow(QWidget *parent) {
    QSettings settings;
    setup_menus();
    setCentralWidget(new MainArea());
    /*this->showMaximized();*/
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(400, 200);
    setGeometry(0, 0, 800, 300);
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
