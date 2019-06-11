//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_CONNECTTODAEMONDIALOG_H
#define __ORCUS_PLAYER_CONNECTTODAEMONDIALOG_H
//-------------------------------------------------------------------------------------------

#include <QDialog>
#include "player/ui_ConnectToDaemonDialog.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class ConnectToDaemonDialog : public QDialog
{
	Q_OBJECT
	
	public:
		ConnectToDaemonDialog(QWidget *parent=0, Qt::WindowFlags f=0);
		virtual ~ConnectToDaemonDialog();
		
		virtual const QString& hostName() const;
		
	private:
		Ui::ConnectToDaemonDialog ui;
		QString m_hostName;
		
	private slots:
		virtual void onHostEdit(const QString& text);
		virtual void onConnect();
		virtual void onCancel();
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

