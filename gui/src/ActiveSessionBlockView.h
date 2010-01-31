#ifndef AESALON_ACTIVE_SESSION_BLOCK_VIEW_H
#define AESALON_ACTIVE_SESSION_BLOCK_VIEW_H

#include <QWidget>
#include <QTableWidget>

#include "ActiveSessionMemory.h"

class ActiveSessionBlockView : public QTableWidget { Q_OBJECT
public:
    ActiveSessionBlockView(QWidget* parent = 0);
    virtual ~ActiveSessionBlockView() {}
public slots:
};

#endif