/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/Mempool.h
*/

#ifndef AesalonStorage_Mempool_H
#define AesalonStorage_Mempool_H

#include <ext/pool_allocator.h>

#define AesalonPoolAlloc(type, pointer, object) \
	do { \
		__gnu_cxx::__pool_alloc<type> allocator; \
		pointer = allocator.allocate(1); \
		allocator.construct(pointer, object); \
	} while(0)

#define AesalonPoolDestroy(type, pointer) \
	do { \
		__gnu_cxx::__pool_alloc<type> allocator; \
		allocator.destroy(pointer); \
	} while(0)

#define AesalonPoolArray(type, pointer, size) \
	do { \
		__gnu_cxx::__pool_alloc<type> allocator; \
		pointer = allocator.allocate(size); \
	} while(0)


#endif
