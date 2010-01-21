#ifndef AESALON_GUI_ACTIVE_SESSION_BLOCK_VIEW_H
#define AESALON_GUI_ACTIVE_SESSION_BLOCK_VIEW_H

#include <QWidget>
#include <QTableWidget>

#include "platform/Memory.h"
#include "platform/Event.h"

namespace Aesalon {
namespace GUI {

class ActiveSessionBlock {
public:
    
};

class ActiveSessionBlockView : public QTableWidget { Q_OBJECT
private:
    Platform::Memory *memory;
public:
    ActiveSessionBlockView(Platform::Memory *memory, QWidget *parent = 0);
    virtual ~ActiveSessionBlockView();
public slots:
    void memory_changed(Platform::Event *event);
};

} // namespace GUI
} // namespace Aesalon

#endif
