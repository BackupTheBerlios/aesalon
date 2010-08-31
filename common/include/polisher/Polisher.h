#ifndef PolisherInterface_H
#define PolisherInterface_H

#include "DataTypes.h"

namespace Program {
class Analyzer;
} // namespace Program

class PolisherInterface {
public:
	PolisherInterface() {}
	virtual ~PolisherInterface() {}
private:
	Program::Analyzer *m_analyzer;
public:
	Program::Analyzer *analyzer() const { return m_analyzer; }
	void setAnalyzer(Program::Analyzer *analyzer) { m_analyzer = analyzer; }
	
	/** Handles a packet from the corresponding collector library.
		@param packet The packet to process.
		@return A DataPacket instance (can be the same as @a packet) to be sent
			to the visualizer library. Note that packet->data will be freed, so
			it should be dynamically-allocated memory.
	*/
	virtual DataPacket *handlePacket(DataPacket *packet) = 0;
};

#define CreateInstantiationFunction(type) \
	extern "C" { \
	PolisherInterface *Instantiate() { \
		return new type(); \
	} \
	} // extern "C"

#endif
