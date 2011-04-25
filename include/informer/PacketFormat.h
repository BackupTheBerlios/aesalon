/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file informer/PacketFormat.h
*/

#ifndef AesalonInformer_PacketFormat_H
#define AesalonInformer_PacketFormat_H

#ifdef __cplusplus
namespace Informer {
#endif

/** Describes an Informer packet's type.
	General transmission format is one byte describing the packet type, then the rest as type-specific.
*/
enum PacketType {
	/** A module was just loaded; find the corresponding shared library and load it.
		Format:
			- id: ModuleID type.
			- name: 8-bit ASCII string. (length specified by packet header's dataSize - sizeof(ModuleID) - 1).
	*/
	ModuleLoaded,
	/** A new file was mapped to an executable page; the symbol data should be loaded from it.
		Format:
			- Base address (base address of memory map)
			- File offset (which portion of the file is mapped)
			- filename: 8-bit NULL-terminated ASCII string.
	*/
	FileLoaded,
	/**
		A new thread was just created; an ID for it is provided.
	*/
	NewThread,
	/**
		A new process was just created; an ID for it is provided.
	*/
	NewProcess,
	ThreadExiting,
	ProcessExiting,
};

#ifdef __cplusplus
} // namespace Informer
#endif

#endif
