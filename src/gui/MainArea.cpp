#include <iostream>
#include "MainArea.h"
#include "MainArea.moc"
#include "SessionManager.h"

namespace Aesalon {
namespace GUI {

MainArea::MainArea(QWidget *parent) {
    SessionManager *manager = new SessionManager;
    this->addTab(manager, tr("&Session Manager"));
    connect(manager, SIGNAL(new_tab_request(QWidget*, QString)), this, SLOT(add_tab(QWidget*, QString)));
    connect(manager, SIGNAL(remove_tab_request(QWidget*)), this, SLOT(remove_tab(QWidget*)));
    this->setTabsClosable(true);
    
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close_requested(int)));
}

void MainArea::add_tab(QWidget *tab, QString name) {
    this->addTab(tab, name);
}

void MainArea::remove_tab(QWidget *tab) {
    if(this->indexOf(tab)) this->removeTab(indexOf(tab));
}

void MainArea::tab_close_requested(int index) {
    std::cout << "Requested close of tab #" << index << std::endl;
    if(index == 0) return;
    this->widget(index)->deleteLater();
}

} // namespace GUI
} // namespace Aesalon
