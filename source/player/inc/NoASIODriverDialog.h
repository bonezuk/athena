//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_NOASIODRIVERDIALOG_H
#define __ORCUS_PLAYER_NOASIODRIVERDIALOG_H
//-------------------------------------------------------------------------------------------

#include <QDialog>
#include "player/ui_NoASIODriverWidget.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class NoASIODriverDialog : public QDialog
{
	Q_OBJECT
	
	public:
		NoASIODriverDialog(QWidget *parent=0,Qt::WindowFlags f= Qt::WindowFlags());
		virtual ~NoASIODriverDialog();
		
	protected:
	
		Ui::NoASIODriverWidget ui;
		
	protected slots:
	
		void onClose();
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
