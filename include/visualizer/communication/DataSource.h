/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/visualizer/communication/DataSource.h

*/

#ifndef AesalonVisualizer_Communication_DataSource_H
#define AesalonVisualizer_Communication_DataSource_H

#include "common/VPacket.h"

namespace Visualizer {
namespace Communication {

class DataSource {
public:
	virtual ~DataSource() {}
	
	/** Attempts to open the data source. May take some time -- network
		timeouts may be involved.
		@return True if the data source was opened successfully.
	*/
	virtual bool open() = 0;
	
	/** Returns the next available packet. This function may block.
		@return The next packet, or NULL if there are no more packets
			forthcoming (e.g. the socket has closed, EOF, etc.)
	*/
	virtual Common::VPacket *nextPacket() = 0;
};

} // namespace Communication
} // namespace Visualizer

#endif
