#ifndef AESALON_GUI_SESSION_ACTIVE_BLOCKS_ENGINE_H
#define AESALON_GUI_SESSION_ACTIVE_BLOCKS_ENGINE_H

#include "GraphDataEngine.h"

class ActiveBlocksEngine : public GraphDataEngine { Q_OBJECT
public:
    ActiveBlocksEngine(QObject *parent, DataThread *data_thread);
    virtual ~ActiveBlocksEngine();
};

#endif
