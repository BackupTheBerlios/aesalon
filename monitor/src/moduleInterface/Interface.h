#ifndef ModuleInterface_H
#define ModuleInterface_H

#include "DataTypes.h"

namespace Program {
class Analyzer;
} // namespace Program

class ModuleInterface {
public:
	ModuleInterface();
	virtual ~ModuleInterface();
private:
	Program::Analyzer *m_analyzer;
public:
	Program::Analyzer *analyzer() const { return m_analyzer; }
	void setAnalyzer(Program::Analyzer *analyzer) { m_analyzer = analyzer; }
	
	/** Handles a packet from the corresponding collector library.
		@param packet The packet to process.
		@return A DataPacket instance (can be the same as @a packet) to be sent
			to the visualizer library.
	*/
	virtual DataPacket *handlePacket(DataPacket *packet) = 0;
};

extern "C" {

/* This function is expected to be implemented inside the monitor modules. */
ModuleInterface *AesalonMonitorCreateInstance();

} // extern "C"

#endif
