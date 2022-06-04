//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_PLAYERIOS_PLAYERUISETTINGS_H
#define __OMEGA_PLAYERAPP_PLAYERIOS_PLAYERUISETTINGS_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"
#include "network/inc/Controller.h"
#include "network/ftp/inc/FTPServer.h"
#include "network/ftp/inc/FTPService.h"
#include "common/inc/DiskOps.h"

#include <QStandardPaths>

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class PlayerUISettings : public QObject
{
	public:
		Q_OBJECT
		Q_PROPERTY(bool ftpServer READ isFTPEnabled WRITE enableFTPServer NOTIFY ftpServerChanged)
		Q_PROPERTY(QString ftpStatus READ getFTPStatus NOTIFY ftpStatusChanged)
		
	public:
		PlayerUISettings(QObject *parent = 0);
		virtual ~PlayerUISettings();
		
		virtual bool isFTPEnabled();
		virtual void enableFTPServer(bool enable);
		virtual QString getFTPStatus();
		
	private:
		network::ftp::FTPService *m_ftpService;
		network::ftp::FTPServer *m_ftpServer;
		QString m_ftpStatusMsg;
		
		const tint c_ftpPort = 5500;
		const tint c_ftpPassivePort = 5505;
		
		virtual void printError(const tchar *strR, const tchar *strE) const;
		virtual QString appDataDirectory() const;
		virtual QString musicDirectory() const;
		virtual bool startFTPServer();
		virtual void stopFTPServer();
		
	signals:
		void ftpServerChanged(bool enabled);
		void ftpStatusChanged(const QString& statusTxt);
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
