/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/LogInput.cpp
*/

#include <QString>

#include "visualizer/LogInput.h"
#include "util/MessageSystem.h"

namespace Visualizer {

LogInput::LogInput(const std::string &filename) {
	m_file.setFileName(QString::fromStdString(filename));
	
	m_file.open(QIODevice::ReadOnly);
	
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(readMore()));
	connect(this, SIGNAL(moreData(QByteArray)), this, SLOT(addData(QByteArray)));
	
	m_timer.start(10);
}

LogInput::~LogInput() {
	
}

void LogInput::readMore() {
	QByteArray more = m_file.read(1048576);
	
	if(more.size() == 0) m_timer.stop();
	else {
		emit moreData(more);
	}
}

} // namespace Visualizer
