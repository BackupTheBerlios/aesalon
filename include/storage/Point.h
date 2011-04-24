/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/Point.h
*/

#ifndef AesalonStorage_Point_H
#define AesalonStorage_Point_H


namespace Storage {

template<typename Key, int Dimensions>
class Point {
private:
	Key m_coordinates[Dimensions];
public:
	Point() {
		for(int i = 0; i < Dimensions; i ++) {
			m_coordinates[i] = 0;
		}
	}
	Point(Key c1, ...) {
		va_list ap;
		va_start(ap, c1);
		
		m_coordinates[0] = c1;
		
		for(int i = 0; i < Dimensions; i ++) {
			m_coordinates[i] = va_arg(ap, Key);
		}
		
		va_end(ap);
	}
	
	double coordinate(int which) const { return m_coordinates[which]; }
};

} // namespace Storage

#endif
