#include <iostream>
#include "MainArea.h"
#include "MainArea.moc"

MainArea::MainArea(QWidget *parent) {
    this->setTabsClosable(true);
    
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(tab_close_requested(int)));
}

void MainArea::add_tab(QWidget *tab, QString name) {
    this->addTab(tab, name);
    this->setCurrentWidget(tab);
}

void MainArea::remove_tab(QWidget *tab) {
    if(this->indexOf(tab)) this->removeTab(indexOf(tab));
}

void MainArea::tab_close_requested(int index) {
    if(index == 0) return;
    this->widget(index)->deleteLater();
}