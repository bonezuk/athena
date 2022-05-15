#include "player/inc/NoASIODriverDialog.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace player
{
//-------------------------------------------------------------------------------------------

NoASIODriverDialog::NoASIODriverDialog(QWidget *parent,Qt::WindowFlags f) : QDialog(parent,f)
{
	ui.setupUi(this);
	connect(ui.m_closeButton,SIGNAL(clicked()),this,SLOT(onClose()));
}

//-------------------------------------------------------------------------------------------

NoASIODriverDialog::~NoASIODriverDialog()
{}

//-------------------------------------------------------------------------------------------

void NoASIODriverDialog::onClose()
{
	accept();
}

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace omega
//-------------------------------------------------------------------------------------------
