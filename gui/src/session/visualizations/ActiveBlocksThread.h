#ifndef AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_THREAD_H
#define AESALON_GUI_SESSION_VISUALIZATION_ACTIVE_BLOCKS_THREAD_H

#include "../VisualizationThread.h"

class ActiveBlocksThread : public VisualizationThread {
public:
    ActiveBlocksThread(DataThread *data_thread);
    virtual ~ActiveBlocksThread();
};

#endif