#include "LogSessionLauncher.h"
#include "LogSessionLauncher.moc"
#include "session/Session.h"
#include "session/LogWrapper.h"

LogSessionLauncher::LogSessionLauncher(QWidget *parent) : QFileDialog(parent) {
	
}

LogSessionLauncher::~LogSessionLauncher() {

}

void LogSessionLauncher::accept() {
    Session *session = new Session(new LogWrapper(selectedFiles()[0]));
	emit newTab(session->sessionDisplay());
	QDialog::accept();
}
