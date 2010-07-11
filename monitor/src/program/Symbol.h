#ifndef Symbol_H
#define Symbol_H

#include "DataTypes.h"

namespace Program {

class Symbol {
public:
	Symbol(Address address, Address size, const char *name);
	~Symbol();
private:
	Address m_address;
	Address m_size;
	const char *m_name;
public:
	Address address() const { return m_address; }
	Address size() const { return m_size; }
	const char *name() const { return m_name; }
	
	bool operator<(const Symbol &other) const;
};

} // namespace Program

#endif
