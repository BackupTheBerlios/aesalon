#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>

#include "SessionSettings.h"

SessionSettings::SessionSettings(QWidget *parent, Session *session) : QDialog(parent), m_session(session) {
	QFormLayout *mainLayout = new QFormLayout();
	
	m_sessionName = new QLineEdit();
	m_sessionName->setText(tr("Session"));
	mainLayout->addRow(tr("Session Name:"), m_sessionName);
	
	m_heartbeatTime = new QSpinBox();
	m_heartbeatTime->setSuffix(tr(" milliseconds"));
	m_heartbeatTime->setRange(50, 60000);
	m_heartbeatTime->setAlignment(Qt::AlignRight);
	mainLayout->addRow(tr("Heartbeat interval:"), m_heartbeatTime);
	
	QDialogButtonBox *buttonBox = new QDialogButtonBox();
	buttonBox->addButton(QDialogButtonBox::Cancel);
	buttonBox->addButton(QDialogButtonBox::Ok);
	
	connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));
	
	mainLayout->addWidget(buttonBox);

	setLayout(mainLayout);
	
	setMinimumSize(400, 200);
	setWindowModality(Qt::WindowModal);
}

SessionSettings::~SessionSettings() {
	
}

void SessionSettings::accept() {
	m_session->heartbeat()->setInterval(m_heartbeatTime->value());
	
	QDialog::accept();
}
