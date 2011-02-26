/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/artisan/Viewport.h
*/

#ifndef AesalonArtisan_Viewport_H
#define AesalonArtisan_Viewport_H

#include <QWidget>

namespace Artisan {

class Viewport : public QWidget {
public:
	virtual ~Viewport() {}
};

} // namespace Artisan

#endif
