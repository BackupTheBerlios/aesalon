/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/Data.cpp
*/

#include <QAtomicInt>

#include "artisan/gviewport/Data.h"
#include "artisan/gviewport/Object.h"

namespace Artisan {
namespace GViewport {

void Data::startReading() {
	m_treeLock.lockForRead();
}

void Data::stopReading() {
	m_treeLock.unlock();
}

void Data::startWriting() {
	m_treeLock.lockForWrite();
}

void Data::stopWriting() {
	m_treeLock.unlock();
}

void Data::addObject(Object *object) {
	startWriting();
	m_tree.insert(object->bound(), object);
	stopWriting();
}

void Data::removeObject(Object *object) {
	startWriting();
	m_tree.remove(object->bound(), object);
	stopWriting();
}

} // namespace GViewport
} // namespace Artisan
