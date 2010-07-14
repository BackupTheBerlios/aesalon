#ifndef Visualizer_H
#define Visualizer_H

#include "visualizerInterface/Interface.h"

class CpuTimeVisualizer : public ModuleInterface {
public:
	CpuTimeVisualizer();
	virtual ~CpuTimeVisualizer();
private:
	
public:
	virtual void processIncoming(DataPacket *packet);
};

#endif
