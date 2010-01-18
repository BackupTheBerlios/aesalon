#include "SessionListWidget.h"
#include "SessionListWidget.moc"

namespace Aesalon {
namespace GUI {

SessionListWidget::SessionListWidget(QWidget *parent) {
    
}

void SessionListWidget::update_list(QList<Session *> sessions) {
    this->clear();
    for(int i = 0; i < sessions.size(); i ++) {
        this->addItem(sessions[i]->get_session_name());
    }
    this->sortItems(Qt::AscendingOrder);
}

} // namespace GUI
} // namespace Aesalon
