#ifndef AESALON_GUI_SESSION_ACTIVE_BLOCKS_ENGINE_H
#define AESALON_GUI_SESSION_ACTIVE_BLOCKS_ENGINE_H

#include "ActiveBlocksRequest.h"
#include "GraphDataEngine.h"

class ActiveBlocksEngine : public GraphDataEngine { Q_OBJECT
protected:
    DataRequest *spawn_new_request(const Timestamp &timestamp) const
        { return new ActiveBlocksRequest(this, timestamp); }
public:
    ActiveBlocksEngine(QObject *parent, DataThread *data_thread);
    virtual ~ActiveBlocksEngine();
};

#endif
