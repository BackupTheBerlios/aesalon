/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/gviewport/Data.h
*/

#ifndef AesalonArtisan_GViewport_Data_H
#define AesalonArtisan_GViewport_Data_H

#include <QReadWriteLock>

#include "TreeType.h"

namespace Artisan {
namespace GViewport {

class Data {
private:
	TreeType m_tree;
	
	QReadWriteLock m_treeLock;
public:
	Data() {}
	~Data() {}
	
	TreeType &tree() { return m_tree; }
	
	void startReading();
	void stopReading();
	void startWriting();
	void stopWriting();
	
	void addObject(Object *object);
	void removeObject(Object *object);
};

} // namespace GViewport
} // namespace Artisan

#endif
