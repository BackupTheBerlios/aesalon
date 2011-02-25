/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/Mempool.h
*/

#ifndef AesalonStorage_Mempool_H
#define AesalonStorage_Mempool_H

#include <stdint.h>
#include <vector>

namespace Storage {

class Mempool {
protected:
	typedef std::vector<uint8_t *> MapVector;
	typedef std::vector<uint32_t> UseVector;
private:
	MapVector m_mapVector;
	UseVector m_useVector;
	/** Mempool size: 16MB. */
	static const uint32_t m_mempoolSize = 16777216;
public:
	Mempool();
	~Mempool();
	
	void *request(uint32_t size);
private:
	void createNew();
};

} // namespace Storage

#endif
