/** Aesalon, a tool to visualize program behaviour in real time.
	Copyright (C) 2009-2011, Aesalon development team.
	
	Aesalon is distributed under the terms of the GNU GPLv3. See
	the included file LICENSE for more information.
	
	@file src/visualizer/InputCreator.cpp
*/

#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QFileDialog>
#include <QStyle>

#include "visualizer/InputCreator.h"
#include "util/MessageSystem.h"
#include "visualizer/LogInput.h"

namespace Visualizer {

InputCreator::InputCreator() {
	setModal(true);
	QVBoxLayout *layout = new QVBoxLayout();
	
	QTabWidget *tabWidget = new QTabWidget();
	
	tabWidget->addTab(setupLogTab(), tr("&Log file"));
	
	layout->addWidget(tabWidget);
	
	setLayout(layout);
}

InputCreator::~InputCreator() {
	
}

QWidget *InputCreator::setupLogTab() {
	QWidget *tab = new QWidget();
	
	QFormLayout *layout = new QFormLayout();
	
	QHBoxLayout *selectionLayout = new QHBoxLayout();
	
	m_fileLocation = new QLabel(tr("Not selected."));
	selectionLayout->addWidget(m_fileLocation);
	
	QPushButton *selectFile = new QPushButton(style()->standardIcon(QStyle::SP_DirOpenIcon), tr(""));
	selectionLayout->addWidget(selectFile);
	connect(selectFile, SIGNAL(clicked()), this, SLOT(fileSelected()));
	
	layout->addRow(tr("File location:"), selectionLayout);
	
	m_logCreateButton = new QPushButton(tr("&Open log"));
	m_logCreateButton->setDisabled(true);
	layout->addWidget(m_logCreateButton);
	connect(m_logCreateButton, SIGNAL(clicked()), this, SLOT(createLogInput()));
	
	tab->setLayout(layout);
	
	return tab;
}

void InputCreator::fileSelected() {
	QString filename = QFileDialog::getOpenFileName(this);
	if(filename == "") return;
	
	m_logCreateButton->setDisabled(false);
	m_fileLocation->setText(filename);
}

void InputCreator::createLogInput() {
	hide();
	LogInput *li = new LogInput(m_fileLocation->text().toStdString());
	emit inputCreated(li);
}

} // namespace Visualizer
