#include "player/inc/ConnectToDaemonDialog.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

ConnectToDaemonDialog::ConnectToDaemonDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f),
	m_hostName()
{
	ui.setupUi(this);
	connect(ui.m_hostNameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onHostEdit(const QString&)));
	connect(ui.m_connectButton, SIGNAL(clicked()), this, SLOT(onConnect()));
	connect(ui.m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancel()));
	ui.m_connectButton->setEnabled(false);
}

//-------------------------------------------------------------------------------------------

ConnectToDaemonDialog::~ConnectToDaemonDialog()
{}

//-------------------------------------------------------------------------------------------

void ConnectToDaemonDialog::onHostEdit(const QString& text)
{
	ui.m_connectButton->setEnabled(!text.isEmpty());
}

//-------------------------------------------------------------------------------------------

const QString& ConnectToDaemonDialog::hostName() const
{
	return m_hostName;
}

//-------------------------------------------------------------------------------------------

void ConnectToDaemonDialog::onConnect()
{
	m_hostName = ui.m_hostNameEdit->text();
	done(1);
}

//-------------------------------------------------------------------------------------------

void ConnectToDaemonDialog::onCancel()
{
	done(0);
}

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
