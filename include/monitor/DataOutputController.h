/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/DataOutputController.h
*/

#ifndef AesalonMonitor_DataOutputController_H
#define AesalonMonitor_DataOutputController_H

#include <vector>
#include <string>

#include "DataOutput.h"

namespace Monitor {

class DataOutputController {
protected:
	typedef std::vector<DataOutput *> DataOutputVector;
private:
	DataOutputVector m_dataOutputVector;
public:
	DataOutputController();
	~DataOutputController();
	
	void output(Comm::Packet *packet);
private:
	DataOutput *createOutput(const std::string &spec);
};

} // namespace Monitor

#endif
