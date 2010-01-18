#include <QApplication>
#include "MainWindow.h"
#include "MainWindow.moc"

#include "MainArea.h"

namespace Aesalon {
namespace GUI {

MainWindow::MainWindow(QWidget *parent) {
    QSettings settings;
    setup_menus();
    setCentralWidget(new MainArea());
    this->showMaximized();
}

void MainWindow::setup_menus() {
    aesalon_menu = new QMenu(tr("&Aesalon"));
    aesalon_menu->addAction("&Quit", this, SLOT(close()));
    menuBar()->addMenu(aesalon_menu);
}

} // namespace GUI
} // namespace Aesalon
