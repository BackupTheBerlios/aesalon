/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/Aesalon.cpp
*/

#include <QApplication>
#include <QThreadPool>

#include "visualizer/RootWindow.h"
#include "util/MessageSystem.h"
#include "config/GlobalVault.h"

int main(int argc, char *argv[]) {
	QApplication qapp(argc, argv);
	
	Message(Debug, "Ideal thread count is " << QThread::idealThreadCount() << ".");
	
	Config::GlobalVault gv;
	
	Visualizer::RootWindow rw;
	rw.show();
	
	return qapp.exec();
}
