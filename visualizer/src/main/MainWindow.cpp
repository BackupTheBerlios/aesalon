#include <QMenu>
#include <QMenuBar>
#include <QStyle>
#include <QSettings>
#include <QMessageBox>
#include <QApplication>

#include "MainWindow.h"
#include "MainWindow.moc"
#include "SessionLauncher.h"

MainWindow::MainWindow() {
	setMinimumSize(600, 400);
	setGeometry(100, 100, 800, 600);
	setupMenus();
	setWindowTitle("Aesalon Visualizer");
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	QSettings settings;
	setWindowIcon(QIcon(":/data/aesalon_48x48.png"));
	/*setWindowIcon(QIcon(settings.value("data-path", "visualizer/data/").toString() + "aesalon_48x48.png"));*/
	
	m_centralWidget = new QTabWidget();
#if QT_VERSION >= 0x045000
	m_centralWidget->setTabsClosable(true);
#endif
	setCentralWidget(m_centralWidget);
}

MainWindow::~MainWindow() {
	
}

void MainWindow::addTab(QWidget *widget) {
	m_centralWidget->addTab(widget, "Session");
}

void MainWindow::setupMenus() {
	QSettings settings;
	QMenu *aesalonMenu = new QMenu(tr("&Aesalon"));
	
	aesalonMenu->addAction(style()->standardIcon(QStyle::SP_DriveNetIcon),
		tr("&Connect to monitor . . ."), this, SLOT(createLauncher()), settings.value("shortcuts/connect", "Ctrl+E").toString());
		
	aesalonMenu->addAction(style()->standardIcon(QStyle::SP_DirOpenIcon),
		tr("&Open log file . . ."), this, SLOT(createLauncher()), settings.value("shortcuts/open", "Ctrl+O").toString());
	
	aesalonMenu->addAction(style()->standardIcon(QStyle::SP_DialogCloseButton),
		tr("E&xit"), this, SLOT(close()), settings.value("shortcuts/quit", "Ctrl+Q").toString());
	menuBar()->addMenu(aesalonMenu);
	
	QMenu *helpMenu = new QMenu(tr("&Help"));
	helpMenu->addAction(tr("&About"), this, SLOT(about()));
	helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
	menuBar()->addMenu(helpMenu);
}

void MainWindow::createLauncher() {
	SessionLauncher *launcher = new SessionLauncher(this);
	connect(launcher, SIGNAL(newTab(QWidget*)), SLOT(addTab(QWidget*)));
	launcher->exec();
}

void MainWindow::about() {
	QString aboutMessage;
	aboutMessage += "<div align=\"center\"><b>Aesalon</b> version " AesalonVersion "</div><hr />";
	aboutMessage += "Aesalon is copyrighted (C) 2009-2010 by strange. <br />";
	aboutMessage += "Aesalon is released under the GNU General Public License, version 3. For more license information, ";
	aboutMessage += "see the file LICENSE included in the distribution.<br /><br />";
	aboutMessage += "Aesalon is a tool to display data on a running program. The types of data ";
	aboutMessage += "collected vary depending on the modules loaded into memory, and modules can be written ";
	aboutMessage += "easily to provide new types of data collection. <br /><br />This is the visualization component of Aesalon. <br /><br />";
	
	QMessageBox::about(this, tr("About Aesalon"), aboutMessage);
}
