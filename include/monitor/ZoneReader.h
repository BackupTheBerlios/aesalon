/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/ZoneReader.h
*/

#ifndef AesalonMonitor_ZoneReader_H
#define AesalonMonitor_ZoneReader_H

#include "monitor/SHMReader.h"

namespace Monitor {

class ZoneReader {
private:
	SHMReader *m_shmReader;
public:
	ZoneReader(SHMReader *shmReader);
	~ZoneReader();
	
private:
	static void *run(void *voidInstance);
};

} // namespace Monitor

#endif
