#ifndef Interface_H
#define Interface_H

#include "DataTypes.h"
#include "storage/DataRange.h"
#include "visualization/VisualizationWrapper.h"

class VisualizerInterface {
public:
	VisualizerInterface();
	virtual ~VisualizerInterface();
public:
	/** Process an incoming packet from the monitor or log file.
		@param packet The packet to process.
	*/
	virtual void processIncoming(DataPacket *packet) = 0;
	
	/** Fill a Visualization object with visualization data.
		@param visualization The visualization to render.
		@param valid The dereferenced value of this will become true if the
			visualization should be aborted for any reason. If a loop is used,
			check once per loop iteration. The value <i>can</i> be ignored, 
			but ignoring the value <b>will</b> lead to unnecessary processing,
			to a magnitude of as much as a factor of a hundred in certain
			conditions. The state of @a visualization does not matter if the
			visualization process is aborted.
	*/
	virtual void visualize(VisualizationWrapper *visualization, bool *abort) = 0;
	/** Calculate a data range suitable for a default visualization view.
		@return A DataRange object, suitable for usage as a Visualization range.
	*/
	virtual DataRange defaultDataRange() = 0;
};

#endif
