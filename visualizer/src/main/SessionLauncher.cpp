#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStyle>
#include <QTcpSocket>

#include "SessionLauncher.h"
#include "SessionLauncher.moc"
#include "session/Session.h"
#include "session/TCPWrapper.h"

SessionLauncher::SessionLauncher(QWidget *parent) : QDialog(parent) {
	setMinimumSize(400, 300);
	setupDialog();
}

SessionLauncher::~SessionLauncher() {
	
}

void SessionLauncher::accept() {
	Session *session = new Session(new TCPWrapper(m_host->text(), m_port->value()));
	emit newTab(session->sessionDisplay());
	QDialog::accept();
}

void SessionLauncher::setupDialog() {
	QVBoxLayout *mainLayout = new QVBoxLayout();
	QFormLayout *formLayout = new QFormLayout();
	
	m_host = new QLineEdit();
	m_host->setText("localhost");
	formLayout->addRow(tr("Host:"), m_host);
	
	m_port = new QSpinBox();
	m_port->setMinimum(1025);
	m_port->setMaximum(65535);
	formLayout->addRow(tr("Port:"), m_port);
	
	mainLayout->addLayout(formLayout);
	mainLayout->addStretch(1);
	
	QDialogButtonBox *buttonBox = new QDialogButtonBox();
	QPushButton *connectButton = new QPushButton(style()->standardIcon(QStyle::SP_DriveNetIcon), tr("&Connect"));
	buttonBox->addButton(connectButton, QDialogButtonBox::AcceptRole);
	
	QPushButton *cancelButton = new QPushButton(style()->standardIcon(QStyle::SP_DialogCancelButton), tr("C&ancel"));
	buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);
	
	connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));
	
	mainLayout->addWidget(buttonBox);
	
	setLayout(mainLayout);
}

