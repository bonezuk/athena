#include "playerapp/webservice/inc/OmegaWebService.h"

#include <QFileInfo>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaWebService::OmegaWebService(const QString& rootDir, int argc, char **argv) : QCoreApplication(argc, argv),
	m_rootDir(rootDir),
	m_webService(0),
	m_webServer(0)
{
	QTimer::singleShot(10, this, SLOT(onStartService()));
	QObject::connect(this, SIGNAL(aboutToQuit()), this, SLOT(onStopService()));
}

//-------------------------------------------------------------------------------------------

OmegaWebService::~OmegaWebService()
{}

//-------------------------------------------------------------------------------------------

void OmegaWebService::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "OmegaWebService::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool OmegaWebService::isValidSetup() const
{
	QFileInfo root(m_rootDir);
	
	if(!root.isDir())
	{
		QString err = QString("Given www content directory '%1' is not a directory").arg(m_rootDir);
		printError("isValidSetup", err.toUtf8().constData());
		return false;
	}
	return true;
}

//-------------------------------------------------------------------------------------------

void OmegaWebService::onStartService()
{
	bool res = false;
	
	if(!isValidSetup())
	{
		printError("onStartService", "Invalid configuration, cannot start server");
		quit();
		return;
	}
	
	try
	{
		network::Resource& nResource = network::Resource::instance();
		network::Controller::ControllerSPtr ctrl(network::Controller::instance());
		
		if(ctrl.data()!=0)
		{
			m_webService = dynamic_cast<network::http::HTTPService *>(ctrl->newService("http_server"));
			if(m_webService!=0)
			{
				m_webServer = m_webService->getServer(c_serverPort);
				if(m_webServer!=0)
				{
					m_webServer->connect("/",this,SLOT(onWebRoot(network::http::HTTPReceive *)));
					
					QString successMsg = QString("Web service successfully started on port=%1").arg(c_serverPort);
					common::Log::g_Log << successMsg << common::c_endl;
					res = true;
				}
				else
				{
					QString err = QString("Failed to start HTTP server on port %1").arg(c_serverPort);
					printError("startService", err.toUtf8().constData());
				}
			}
			else
			{
				printError("startService", "Failed to instantiate HTTP service");
			}
		}
		else
		{
			printError("startService", "Failed to instantiate network controller");
		}	
	}
	catch(common::Exception&)
	{
		res = false;
	}
	
	if(!res)
	{
		quit();
	}
}

//-------------------------------------------------------------------------------------------

void OmegaWebService::onStopService()
{
	network::Controller::ControllerSPtr ctrl(network::Controller::instance());
	if(m_webService!=0)
	{
		if(m_webServer!=0)
		{
			m_webService->deleteServer(m_webServer);
			m_webServer = 0;
		}
		ctrl->deleteService(m_webService);
		m_webService = 0;
	}
	network::Controller::end();
	
	common::Log::g_Log << "Web service shut down" << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaWebService::onWebRoot(network::http::HTTPReceive *recieve)
{
	HTTPFileTransfer transfer(m_rootDir);
	transfer.process(recieve);
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
