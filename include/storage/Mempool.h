/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/Mempool.h
*/

#ifndef AesalonStorage_Mempool_H
#define AesalonStorage_Mempool_H

#include <ext/pool_allocator.h>
#include <stdint.h>

#include "Config.h"

#define AesalonPoolAlloc(type, pointer, constructor) \
	do { \
		(pointer) = new(Storage::Mempool::allocate(sizeof(type))) constructor; \
	} while(0)

#define AesalonPoolDestroy(type, pointer) \
	do { \
		type *poolPointer = pointer; \
		(poolPointer) ->~type(); \
		Storage::Mempool::release(poolPointer, sizeof(type)); \
	} while(0)

namespace Storage {

class Mempool {
private:
	class ElementSize {
	public:
		ElementSize() : start(NULL), end(NULL) {}
		uint64_t *start;
		uint64_t *end;
	};
	
	static ElementSize m_elementSizes[AesalonPoolSizeThreshold];
	
	static uint64_t *m_poolHead;
	static uint64_t *m_poolTail;
public:
	static void create();
	static void destroy();
	
	static void *allocate(uint64_t size);
	static void release(void *data, uint64_t size);
private:
	/** Normalizes the input size to a power of two >= size. */
	static inline uint64_t normalize(uint64_t size);
	/** Calculates the log, base 2, of value (which must be a power of two). */
	static inline int8_t logBase2(uint64_t value);
	
	static void allocatePool(uint64_t size);
};

} // namespace Storage

/*#undef AesalonPoolAlloc
#define AesalonPoolAlloc(type, pointer, object) \
	do { \
		pointer = new object ; \
	} while(0)*/

#endif
