#ifndef Symbol_H
#define Symbol_H

#include "DataTypes.h"

namespace Program {

class Symbol {
public:
	Symbol(Address address);
	~Symbol();
private:
	Address m_address;
public:
	Address address() const { return m_address; }
	
	bool operator<(const Symbol &other) const;
};

} // namespace Program

#endif
