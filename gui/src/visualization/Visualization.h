#ifndef AESALON_GUI_VISUALIZATION_H
#define AESALON_GUI_VISUALIZATION_H

#include <QWidget>
#include <QVBoxLayout>

#include "Viewport.h"
#include "session/DataThread.h"

class Visualization : public QWidget { Q_OBJECT
private:
    Viewport *viewport;
public:
    Visualization();
    virtual ~Visualization();
};

#endif
