#ifndef AESALON_GUI_DATA_RECEIVER_H
#define AESALON_GUI_DATA_RECEIVER_H

#include <QThread>

class DataReceiver : public QThread { Q_OBJECT
public:
    virtual ~DataReceiver() {}
protected:
    virtual void run() = 0;
};

#endif
