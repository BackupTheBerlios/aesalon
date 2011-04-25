/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file include/visualizer/LogInput.h
*/

#ifndef AesalonVisualizer_LogInput_H
#define AesalonVisualizer_LogInput_H

#include <string>
#include <QFile>
#include <QByteArray>
#include <QTimer>

#include "DataInput.h"

namespace Visualizer {

class LogInput : public DataInput { Q_OBJECT
private:
	QFile m_file;
	QTimer m_timer;
public:
	LogInput(const std::string &filename, ArtisanManager *artisanManager);
	virtual ~LogInput();
	
	virtual QString title() const;
private slots:
	void readMore();
signals:
	void moreData(QByteArray buffer);
};

} // namespace Visualizer

#endif
