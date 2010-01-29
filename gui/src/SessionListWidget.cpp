#include "SessionListWidget.h"
#include "SessionListWidget.moc"




SessionListWidget::SessionListWidget(QWidget *parent) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(display_context_menu(QPoint)));
    
    context_menu = new QMenu();
    context_menu->addAction("&New session", this, SLOT(context_menu_new()));
    context_menu->addAction("&Edit session", this, SLOT(context_menu_edit()));
    context_menu->addAction("&Delete session", this, SLOT(context_menu_delete()));
    
    new_context_menu = new QMenu();
    new_context_menu->addAction("&New session", this, SLOT(context_menu_new()));
    
    delete_shortcut = new QAction(this);
    delete_shortcut->setShortcut(Qt::Key_Delete);
    connect(delete_shortcut, SIGNAL(triggered(bool)), this, SLOT(context_menu_delete()));
    this->addAction(delete_shortcut);
}

void SessionListWidget::update_list(QList<Session *> sessions) {
    this->clear();
    for(int i = 0; i < sessions.size(); i ++) {
        this->addItem(sessions[i]->get_session_name());
    }
    this->sortItems(Qt::AscendingOrder);
}

void SessionListWidget::display_context_menu(const QPoint &pos) {
    if(this->itemAt(pos)) {
        this->setCurrentItem(this->itemAt(pos));
        context_menu->popup(mapToGlobal(pos));
    }
    else new_context_menu->popup(mapToGlobal(pos));
}



