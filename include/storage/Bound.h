/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/storage/Bound.h
*/

#ifndef AesalonStorage_Bound_H
#define AesalonStorage_Bound_H


namespace Storage {

template<typename Key, int Dimensions>
class Bound {
private:
	Key m_bounds[Dimensions][2];
public:
	Bound() {
		for(int i = 0; i < Dimensions; i ++) {
			m_bounds[i][0] = m_bounds[i][1] = 0.0;
		}
	}
	Bound(Key d1_start, Key d1_end, ...) {
		va_list ap;
		va_start(ap, d1_end);
		
		m_bounds[0][0] = d1_start, m_bounds[0][1] = d1_end;
		
		for(int i = 1; i < Dimensions; i ++) {
			m_bounds[i][0] = va_arg(ap, Key);
			m_bounds[i][1] = va_arg(ap, Key);
		}
		
		va_end(ap);
	}
	
	Key &start(int dimension) { return m_bounds[dimension][0]; }
	Key start(int dimension) const { return m_bounds[dimension][0]; }
	Key &end(int dimension) { return m_bounds[dimension][1]; }
	Key end(int dimension) const { return m_bounds[dimension][1]; }
	
	bool overlaps(const Bound &other) const {
		for(int i = 0; i < Dimensions; i ++) {
			if(m_bounds[i][0] > other.m_bounds[i][1] ||
				m_bounds[i][1] < other.m_bounds[i][0]) return false;
		}
		return true;
	}
	
	Key toCover(const Bound &other) {
		Key volume = 0;
		Key bounds[Dimensions][2];
		Key sizes[Dimensions];
		
		for(int i = 0; i < Dimensions; i ++) {
			bounds[i][0] = m_bounds[i][0];
			bounds[i][1] = m_bounds[i][1];
			sizes[i] = m_bounds[i][1] - m_bounds[i][0];
		}
		
		for(int i = 0; i < Dimensions; i ++) {
			Key delta = 0;
			if(other.m_bounds[i][0] < bounds[i][0]) delta += bounds[i][0] - other.m_bounds[i][0];
			if(other.m_bounds[i][1] > bounds[i][1]) delta += other.m_bounds[i][1] - bounds[i][1];
			sizes[i] += delta;
			
			for(int i = 0; i < Dimensions; i ++) {
				volume += delta*sizes[i];
			}
		}
		return volume;
	}
	
	void cover(const Bound &other) {
		for(int i = 0; i < Dimensions; i ++) {
			m_bounds[i][0] = std::min(m_bounds[i][0], other.m_bounds[i][0]);
			m_bounds[i][1] = std::max(m_bounds[i][1], other.m_bounds[i][1]);
		}
	}

	Key volume() const {
		Key value = m_bounds[0][1] - m_bounds[0][0];
		for(int i = 1; i < Dimensions && value != 0; i ++) {
			value *= (m_bounds[i][1] - m_bounds[i][0]);
		}
		return value;
	}
};

} // namespace Storage

#endif
