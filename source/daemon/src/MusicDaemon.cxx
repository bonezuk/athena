#include "daemon/inc/MusicDaemon.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace daemon
{
//-------------------------------------------------------------------------------------------

MusicDaemon::MusicDaemon(int argc,char **argv) : QCoreApplication(argc, argv)
{
	QTimer::singleShot(100,this,SLOT(onStartService()));
	QObject::connect(this,SIGNAL(aboutToQuit()),this,SLOT(onStopService()));
}

//-------------------------------------------------------------------------------------------

MusicDaemon::~MusicDaemon()
{}

//-------------------------------------------------------------------------------------------

void MusicDaemon::printLog(const QString& msg) const
{
	common::Log::g_Log << msg.toUtf8().constData() << common::c_endl;
}

//-------------------------------------------------------------------------------------------

QString MusicDaemon::appHomeDirectory() const
{
	QString path = dlna::DiskIF::mergeName(QDir::homePath(), ".black_omega_phoenix");
	if(!common::DiskOps::path(path, true))
	{
		QString msg = QString("Failed to create application home directory '%1'").arg(path);
		printLog(msg);
	}
	return path;
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onStartService()
{
	bool res = false;

	QString logFilename = dlna::DiskIF::mergeName(appHomeDirectory(), "phoenix_daemon.log");
	orcus::common::Log nLog(logFilename.toUtf8().constData());
	orcus::common::Log::g_Log = nLog;

	try
	{
		network::Resource& nResource = network::Resource::instance();
		network::Controller::ControllerSPtr ctrl(network::Controller::instance());
		
		if(ctrl.data()!=0)
		{
			m_webService = dynamic_cast<network::http::HTTPService *>(ctrl->newService("http_server"));
			if(m_webService!=0)
			{
				m_webServer = m_webService->getServer(MUSIC_DAEMON_PORT);
				if(m_webServer!=0)
				{
					m_webServer->connect("/",this,SLOT(onWebRoot(network::http::HTTPReceive *)));
					printLog("Daemon HTTP server started successfully");
					res = true;
				}
			}
		}
	}
	catch(common::Exception&)
	{
		res = false;
	}
	
	if(!res)
	{
		printLog("Failed to setup HTTP server");
		quit();
	}
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onStopService()
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
	
	printLog("Phoenix Music Daemon service shut down");
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onWebRoot(network::http::HTTPReceive *recieve)
{
	network::http::Unit hdr;
	network::NetArraySPtr body(new network::NetArray);
	
	QString htmlText;
	htmlText += "<html>";
	htmlText += "<head><title>Black Omega Music Daemon</title></head>";
	htmlText += "<body>Service is Running</body>";
	htmlText += "</html>";

	hdr.response(200);
	hdr.add("Content-Type","text/html");
	hdr.add("Connection","close");
	recieve->connection()->postResponse(hdr);
	body->AppendRaw(htmlText.toUtf8().constData(),htmlText.toUtf8().length());
	recieve->connection()->postBody(body);
	recieve->connection()->complete();
	recieve->endProcess();
}

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
