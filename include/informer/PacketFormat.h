/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file informer/PacketFormat.h
*/

#ifndef AesalonInformerPacketFormat_H
#define AesalonInformerPacketFormat_H

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
	FileLoaded,
	NewThread,
	NewProcess,
	ThreadExiting,
	ProcessExiting,
};

#ifdef __cplusplus
} // namespace Informer
#endif

#endif
