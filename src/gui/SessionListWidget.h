#ifndef AESALON_GUI_SESSION_LIST_WIDGET_H
#define AESALON_GUI_SESSION_LIST_WIDGET_H

#include <QListWidget>

#include "Session.h"

namespace Aesalon {
namespace GUI {

class SessionListWidget : public QListWidget { Q_OBJECT
public:
    SessionListWidget(QWidget *parent = 0);
    virtual ~SessionListWidget() {}
    
    void update_list(QList<Session *> sessions);
};

} // namespace GUI
} // namespace Aesalon

#endif
