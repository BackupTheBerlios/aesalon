/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/shm/ZoneReader.h

*/

#ifndef AesalonSHM_ZoneReader_H
#define AesalonSHM_ZoneReader_H

#include <pthread.h>

#include "module/List.h"
#include "SharedMemory.h"
#include "vcommunication/DataSink.h"

namespace SHM {

class ZoneReader {
private:
	SharedMemory *m_sharedMemory;
	Module::List *m_moduleList;
	pthread_t m_threadID;
	VCommunication::DataSink *m_dataSink;
public:
	ZoneReader(SharedMemory *sharedMemory, Module::List *moduleList, VCommunication::DataSink *dataSink);
	~ZoneReader();
	
	void start();
	void startInThread();
	
	void join();
private:
	static void *run(void *voidInstance);
};

} // namespace SHM

#endif
