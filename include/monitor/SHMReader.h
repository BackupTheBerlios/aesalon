/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/monitor/SHMReader.h
*/

#ifndef AesalonMonitor_SHMReader_H
#define AesalonMonitor_SHMReader_H

#include <stdint.h>

#include <string>
#include <vector>

#include "shm/Header.h"

namespace Monitor {

class SHMReader {
public:
	class ReadBroker {
	private:
		uint8_t *m_temporaryBuffer;
		uint32_t m_temporaryBufferSize;
		uint32_t m_zoneID;
		uint32_t m_size;
		void *m_data;
	public:
		ReadBroker();
		~ReadBroker();
		
		uint32_t zoneID() const { return m_zoneID; }
		uint32_t size() const { return m_size; }
		void *data() const { return m_data; }
		
		void setupRequest(uint32_t zoneID, uint32_t size) {
			m_zoneID = zoneID;
			m_size = size;
		}
	protected:
		void resizeBuffer(uint32_t newSize);
		uint8_t *temporaryBuffer() const { return m_temporaryBuffer; }
		uint32_t temporaryBufferSize() const { return m_temporaryBufferSize; }
		void setData(void *data) { m_data = data; }
	private:
		friend class SHMReader;
	};
private:
	int m_fd;
	std::string m_shmName;
	
	SHM::Header *m_header;
	
	uint8_t *m_zoneUseData;
	
	typedef std::vector<uint8_t *> ZoneList;
	ZoneList m_zoneList;
public:
	SHMReader();
	~SHMReader();
	
	uint32_t zoneCount();
	int32_t zoneWithData();
	
	uint32_t zoneProcessID(uint32_t zoneID);
	uint32_t zoneThreadID(uint32_t zoneID);
	
	void closeZones(uint32_t processID);
	
	void waitForPacket();
	
	void processRequest(ReadBroker &request);
private:
	void closeZone(uint32_t id);
	uint8_t *getZone(uint32_t id);
	void *mapRegion(uint32_t start, uint32_t size);
	void unmapRegion(void *data, uint32_t size);
	void setupHeader();
	void setupConfiguration();
	void setupZones();
};

} // namespace Monitor

#endif
