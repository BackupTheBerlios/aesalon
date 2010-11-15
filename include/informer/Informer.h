/**
	Aesalon, a tool to visualize a program's behaviour at run-time.
	Copyright (C) 2010, Aesalon Development Team.

	Aesalon is distributed under the terms of the GNU GPLv3. For more
	licensing information, see the file LICENSE included with the distribution.
	
	@file include/common/Informer.h

*/

#ifndef AesalonInformer_Informer_H
#define AesalonInformer_Informer_H

#include <stdint.h>

#include "common/SharedMemoryHeader.h"
#include "common/Packet.h"

void __attribute__((constructor)) AI_Construct();
void __attribute__((destructor)) AI_Destruct();

void AI_CreateSHM();
void AI_LoadModule(const char *moduleName);

void AI_SendPacket(Packet *packet);

const char *AI_ConfigurationString(const char *name);
int32_t AI_ConfigurationLong(const char *name);
int AI_ConfigurationBool(const char *name);

struct {
	int fd;
	uint8_t *data;
	uint32_t size;
	
	SharedMemoryHeader *header;
} SharedMemory;

#endif
