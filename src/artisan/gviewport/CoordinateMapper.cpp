/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/artisan/gviewport/CoordinateMapper.cpp
*/

#include "artisan/gviewport/CoordinateMapper.h"

namespace Artisan {
namespace GViewport {

CoordinateMapper::CoordinateMapper(const Rect &dataRect, const Rect &pixelRect)
	: m_dataRect(dataRect), m_pixelRect(pixelRect) {
	
}

CoordinateMapper::CoordinateMapper(const RenderedImage &image) : m_dataRect(Rect()), m_pixelRect(Rect()) {
	
}

Point CoordinateMapper::dataToPixel(const Point &dataPoint) {
	double xPercentage = (dataPoint.x() - m_dataRect.left()) / m_dataRect.width();
	double yPercentage = (dataPoint.y() - m_dataRect.top()) / m_dataRect.height();
	
	return Point(
		(xPercentage * m_pixelRect.width()) + m_pixelRect.left(),
		(yPercentage * m_pixelRect.height()) + m_pixelRect.top());
}

Rect CoordinateMapper::dataToPixel(const Rect &dataRect) {
	return Rect(dataToPixel(dataRect.topLeft()), dataToPixel(dataRect.bottomRight()));
}

Point CoordinateMapper::pixelToData(const Point &pixelPoint) {
	double xPercentage = (pixelPoint.x() - m_pixelRect.left()) / m_pixelRect.width();
	double yPercentage = (pixelPoint.y() - m_pixelRect.top()) / m_pixelRect.height();
	
	return Point(
		(xPercentage * m_dataRect.width()) + m_dataRect.left(),
		(yPercentage * m_dataRect.height()) + m_dataRect.top());
}

Rect CoordinateMapper::pixelToData(const Rect &pixelRect) {
	return Rect(pixelToData(pixelRect.topLeft()), pixelToData(pixelRect.bottomRight()));
}


} // namespace GViewport
} // namespace Artisan