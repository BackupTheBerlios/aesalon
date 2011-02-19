/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/monitor/vcommunication/GeneralSink.h

*/

#ifndef AesalonMonitor_VCommunication_GeneralSink_H
#define AesalonMonitor_VCommunication_GeneralSink_H

#include <list>

#include "DataSink.h"

namespace Monitor {
namespace VCommunication {

class GeneralSink : public DataSink {
private:
	typedef std::list<DataSink *> DataSinkList;
	DataSinkList m_dataSinkList;
public:
	GeneralSink();
	virtual ~GeneralSink();
	
	void addDataSink(DataSink *dataSink);
	
	virtual void sinkPacket(Common::VPacket *packet);
};

} // namespace VCommunication
} // namespace Monitor

#endif
