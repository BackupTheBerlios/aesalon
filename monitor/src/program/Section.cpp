#include "Section.h"

namespace Program {

Section::Section(Address address, Address size, Address fileOffset, Address nameOffset)
	: m_address(address), m_size(size), m_fileOffset(fileOffset), m_nameOffset(nameOffset), m_name(NULL), m_data(NULL) {
}

Section::~Section() {
	if(m_data) delete[] m_data;
}

} // namespace Program
