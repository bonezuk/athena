#include "playerapp/playerios/inc/PlayerUISettings.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

PlayerUISettings::PlayerUISettings(QObject *parent) : QObject(parent),
	m_ftpService(0),
	m_ftpServer(0),
	m_ftpStatusMsg("FTP server is disabled")
{}

//-------------------------------------------------------------------------------------------

PlayerUISettings::~PlayerUISettings()
{}

//-------------------------------------------------------------------------------------------

void PlayerUISettings::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "PlayerUISettings::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool PlayerUISettings::isFTPEnabled()
{
	return (m_ftpServer != 0) ? true : false;
}

//-------------------------------------------------------------------------------------------

void PlayerUISettings::enableFTPServer(bool enable)
{
	if(enable && m_ftpServer == 0)
	{
		startFTPServer();
		emit ftpServerChanged((m_ftpServer != 0) ? true : false);
	}
	else if(!enable && m_ftpServer != 0)
	{
		stopFTPServer();
		emit ftpServerChanged(false);
	}
}

//-------------------------------------------------------------------------------------------

QString PlayerUISettings::getFTPStatus()
{
	return m_ftpStatusMsg;
}

//-------------------------------------------------------------------------------------------

QString PlayerUISettings::appDataDirectory() const
{
	QString appDir;
	
#ifdef OMEGA_IOS
	appDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#else
	appDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#endif
	return appDir;
}

//-------------------------------------------------------------------------------------------

QString PlayerUISettings::musicDirectory() const
{
	QString mDir = common::DiskOps::mergeName(appDataDirectory(), "music");
	if(!common::DiskOps::path(mDir, true))
	{
		QString err = QString("Failed to create root music directory '%1'").arg(mDir);
		printError("musicDirectory", err.toUtf8().constData());
		mDir = "";
	}
	return mDir;
}

//-------------------------------------------------------------------------------------------

bool PlayerUISettings::startFTPServer()
{
	bool res = false;
	
	try
	{
		network::Resource& nResource = network::Resource::instance();
		network::Controller::ControllerSPtr ctrl(network::Controller::instance());
		
		if(ctrl.data()!=0)
		{
			m_ftpService = dynamic_cast<network::ftp::FTPService *>(ctrl->newService("ftp_server"));
			if(m_ftpService != 0)
			{
				m_ftpServer = m_ftpService->getServer(c_ftpPort);
				if(m_ftpServer != 0)
				{
					m_ftpServer->config().setRootPath(musicDirectory());
					m_ftpServer->config().setPassivePort(c_ftpPassivePort);
					res = true;
				}
				else
				{
					QString err = QString("Failed to start FTP server on port %1").arg(c_ftpPort);
					printError("startFTPServer", err.toUtf8().constData());
				}
			}
			else
			{
				printError("startFTPServer", "Failed to get FTP service");
			}
		}
		else
		{
			printError("startFTPServer", "Failed to instantiate network controller");
		}

	}
	catch(const common::Exception& e)
	{
		QString err = QString("Exception thrown in starting FTP server. %1").arg(static_cast<const char *>(e.error()));
		printError("startFTPServer", err.toUtf8().constData());
		res = false;
	}
	
	if(res)
	{
		m_ftpStatusMsg = QString::fromUtf8("FTP server on port %1").arg(c_ftpPort);
	}
	else
	{
		m_ftpStatusMsg = QString::fromUtf8("Failed to start FTP server");
	}
	emit ftpStatusChanged(m_ftpStatusMsg);
	return res;
}

//-------------------------------------------------------------------------------------------

void PlayerUISettings::stopFTPServer()
{
	if(m_ftpService != 0)
	{
		network::Controller::ControllerSPtr ctrl(network::Controller::instance());
		
		if(m_ftpServer != 0)
		{
			m_ftpService->deleteServer(m_ftpServer);
			m_ftpServer = 0;
		}
		ctrl->deleteService(m_ftpService);
		m_ftpService = 0;
		m_ftpStatusMsg = QString::fromUtf8("FTP server is disabled");
		emit ftpStatusChanged(m_ftpStatusMsg);
	}
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
