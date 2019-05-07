#include "daemon/inc/MusicDaemon.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace daemon
{
//-------------------------------------------------------------------------------------------

MusicDaemon::MusicDaemon(int argc,char **argv) : QCoreApplication(argc, argv),
	m_cmdArgs(),
	m_webService(0),
	m_webServer(0),
	m_playlist()
{
	QObject::connect(this,SIGNAL(aboutToQuit()),this,SLOT(onStopService()));
	collectCommandLine(argc, argv);
	QTimer::singleShot(100,this,SLOT(onStartService()));
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

void MusicDaemon::printCommandLineUsage() const
{
	fprintf(stdout, "phoenix - Black Omega Music Daemon\n");
	fprintf(stdout, "Load playlist (*.m3u, *.m3u8, *.pls, *.xspf)\n");
	fprintf(stdout, "-P <playlist file>\n");
	fprintf(stdout, "e.g. Load playlist files mymix.m3u party.xspf\n");
	fprintf(stdout, "phoenix -P mymix.m3u -P party.xspf\n");
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::collectCommandLine(int argc, char **argv)
{
	int i;
	
	for(i = 1; i < argc; i++)
	{
		QString arg = QString::fromUtf8(argv[i]);
		if(!arg.isEmpty())
		{
			m_cmdArgs << arg;
		}
	}
}

//-------------------------------------------------------------------------------------------

bool MusicDaemon::processCommandLine()
{
	int cmdState;
	bool processCmdLine = false;
	
	cmdState = 0;
	for(QStringList::iterator ppI = m_cmdArgs.begin(); ppI != m_cmdArgs.end(); ppI++)
	{
		const QString& arg = *ppI;
		if(arg == "-P")
		{
			cmdState = 1;
		}
		else
		{
			if(cmdState == 1)
			{
				if(loadPlaylist(arg))
				{
					processCmdLine = true;
				}
				else
				{
					fprintf(stdout, "Failed to load playlist '%s'.\n", arg.toUtf8().constData());
				}
				cmdState = 0;
			}
		}
	}
	
	if(processCmdLine)
	{
		printCommandLineUsage();
	}
	return processCmdLine;
}

//-------------------------------------------------------------------------------------------

bool MusicDaemon::loadPlaylist(const QString& fileName)
{
	bool res = false;
	
	if(track::db::PlaylistAbstractIO::isSupported(fileName))
	{
		try
		{
			QSharedPointer<track::db::PlaylistAbstractIO> pLoader = track::db::PlaylistIOFactory::createShared(track::db::PlaylistAbstractIO::factoryName(fileName));
            if(pLoader.data()!=0)
            {
				track::db::PLProgress progress;
	            QVector<track::info::InfoSPtr> pList;
	            
            	if(pLoader->load(fileName, pList, &progress))
            	{
            		m_playlist.append(pList);
            		res = true;
            	}
            }
		}
		catch(...) {}
	}
	return res;
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

void MusicDaemon::setupLog()
{
	QString logFilename = dlna::DiskIF::mergeName(appHomeDirectory(), "phoenix_daemon.log");
	orcus::common::Log nLog(logFilename.toUtf8().constData());
	orcus::common::Log::g_Log = nLog;
}

//-------------------------------------------------------------------------------------------

bool MusicDaemon::loadTrackDatabase()
{
	QString dbFilename = dlna::DiskIF::mergeName(appHomeDirectory(), "phoenix_track.db");
	return (track::db::TrackDB::instance(dbFilename) != 0) ? true : false;
}

//-------------------------------------------------------------------------------------------

bool MusicDaemon::startWebServer()
{
	bool res = false;
	
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
	return res;
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onStartService()
{
	bool res = false;
	
	setupLog();
	if(loadTrackDatabase())
	{
		if(processCommandLine())
		{
			if(startWebServer())
			{
				res = true;
			}
			else
			{
				printLog("Failed to setup HTTP server");
			}
		}
		else
		{
			printLog("Failed to process command line");
		}
	}
	else
	{
		printLog("Failed to load track database");
	}
	
	if(!res)
	{
		quit();
	}
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::stopWebServer()
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
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onStopService()
{
	stopWebServer();
	if(track::db::TrackDB::instance() != 0)
	{
		delete track::db::TrackDB::instance();
	}
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
// Audio interface
//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioStart(const QString& name)
{}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioPlay()
{}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioPause()
{}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioTime(quint64 t)
{}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioBuffer(tfloat32 percent)
{}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioReadyForNext()
{}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioNoNext()
{}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioCrossfade()
{}

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
