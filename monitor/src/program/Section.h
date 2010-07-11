#ifndef Section_H
#define Section_H

#include <string>

#include "DataTypes.h"

namespace Program {

class Section {
public:
	Section(Address address, Address size, Address fileOffset, Address nameOffset);
	~Section();
private:
	Address m_address;
	Address m_size;
	Address m_fileOffset;
	Address m_nameOffset;
	const char *m_name;
	uint8_t *m_data;
public:
	Address address() const { return m_address; }
	Address size() const { return m_size; }
	Address fileOffset() const { return m_fileOffset; }
	Address nameOffset() const { return m_nameOffset; }
	const char *name() const { return m_name; }
	void setName(const char *name) { m_name = name; }
	uint8_t *data() const { return m_data; }
	void setData(uint8_t *data) { m_data = data; }
};

} // namespace Program

#endif
