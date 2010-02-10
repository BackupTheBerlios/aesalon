#include <iostream>
#include "MainArea.h"
#include "MainArea.moc"

#include "DataSourceManager.h"
#include "data/NetworkSource.h"

MainArea::MainArea(QWidget *parent) {
    this->setTabsClosable(true);
    
    DataSourceManager *manager = new DataSourceManager(this);
    connect(manager, SIGNAL(tab_request(QWidget*,QString)), SLOT(add_tab(QWidget*,QString)));
    this->add_tab(manager, "&Data source manager");
    
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
