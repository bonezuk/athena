#include "daemon/inc/MusicDaemon.h"
#include "daemon/inc/ConsolePLProgress.h"

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
	m_pEventStream(),
	m_nextEventId(1),
	m_playlist(),
	m_audio(),
	m_currentPlayIndex(0),
	m_nextCount(0)
{
	QObject::connect(this,SIGNAL(aboutToQuit()),this,SLOT(onStopService()));
	MusicDaemon::collectCommandLine(argc, argv);
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
	
	if(!processCmdLine)
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
				ConsolePLProgress progress(fileName);
	            QVector<track::info::InfoSPtr> pList;
	            
            	if(pLoader->load(fileName, pList, &progress))
            	{
					progress.done();
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
					m_pEventStream = QSharedPointer<network::http::EventStreamHandler>(new network::http::EventStreamHandler(this));
					m_webServer->connect("/",this,SLOT(onWebRoot(network::http::HTTPReceive *)));
					m_webServer->connect("/event",this,SLOT(onConnectToEventStream(network::http::HTTPReceive *)));
					m_webServer->connect("/track",this,SLOT(onWebTrack(network::http::HTTPReceive *)));
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
				if(startAudioEngine())
				{
					startPlaybackOfList();
					res = true;
				}
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
			m_pEventStream.clear();
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
	stopAudioEngine();
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
// GET /track?id=3					// Get single track id=3
// GET /track?lowId=3&highId=10		// Get tracks from id=3 to id=10 (3, 4, 5, 6, 7, 8, 9, 10)
// GET /track						// Get all tracks
//
//		{
//			"low" : 3
//			"high" : 10
//			"tracks" : [
//				{
//					"id": 3
//					"filename": "/Users/bonez/Music/13-Imagine The Fire.flac",
//					"length": 1234567890,
//					"bitrate": 160000,
//					"channels": 2,
//					"frequency": 44100,
//					"artist": "Hans Zimmer",
//					"title": "Imagine The Fire",
//					"album": "The Dark Knight Rises: Original Motion Picture Soundtrack (Deluxe Edition)",
//					"year": "2008",
//					"comment": "Sample Track",
//					"genre": "Soundtrack",
//					"track": "12",
//					"disc": "1",
//					"composer": "Hans Zimmer",
//					"original_artist": "Hans Zimmer",
//					"copyright": "(c) Hans Zimmer",
//					"encoder": "Flac encoder",
//					"image": "http://127.0.0.1:13766/image?id=3"
//				},
//				{
//					"id": 4
//					"filename": "/Users/bonez/Music/14-Track.flac",
//					...		
//				},
//				...
//				{
//					"id": 10
//					"filename": "/Users/bonez/Music/15-Track.flac",
//					...
//				}
//			]
//		}
//
//-------------------------------------------------------------------------------------------

QJsonObject MusicDaemon::getTrackJSON(int id)
{
	QJsonObject obj;
	
	if(id >= 0 && id < m_playlist.size())
	{
		track::info::InfoSPtr pInfo = m_playlist.at(id);
		
		obj["id"] = id;
		obj["filename"] = pInfo->getFilename();
		obj["length"] = static_cast<tint64>(pInfo->length());
		obj["bitrate"] = pInfo->bitrate();
		obj["channels"] = pInfo->noChannels();
		obj["frequency"] = pInfo->frequency();
		obj["artist"] = pInfo->artist();
		obj["title"] = pInfo->title();
		obj["album"] = pInfo->album();
		obj["year"] = pInfo->year();
		obj["comment"] = pInfo->comment();
		obj["genre"] = pInfo->genre();
		obj["track"] = pInfo->track();
		obj["disc"] = pInfo->disc();
		obj["composer"] = pInfo->composer();
		obj["original_artist"] = pInfo->originalArtist();
		obj["copyright"] = pInfo->copyright();
		obj["encoder"] = pInfo->encoder();
		obj["image"] = "http://" + network::Resource::instance().localIPName() + "/image?id=" + QString::number(id);
	}
	return obj;
}

//-------------------------------------------------------------------------------------------

bool MusicDaemon::isQueryIdSingle(const network::http::Query& q, int& id)
{
	bool res = false;
	
	if(!q.data("id").isEmpty())
	{
		bool idOk = false;
		
		id = q.data("id").toInt(&idOk);
		if(idOk)
		{
			if(id>=0 && id<m_playlist.size())
			{
				res = true;
			}
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool MusicDaemon::isQueryIdRange(const network::http::Query& q, int& lowId, int& highId)
{
	bool res = false;
	
	if(!q.data("lowId").isEmpty() && !q.data("highId").isEmpty())
	{
		bool lowOk = false, highOk = false;
		
		lowId = q.data("lowId").toInt(&lowOk);
		highId = q.data("highId").toInt(&highOk);
		if(lowOk && highOk)
		{
			if(highId < lowId)
			{
				int t = lowId;
				lowId = highId;
				highId = t;
			}
			if(lowId>=0 && lowId<m_playlist.size() && highId>=0 && highId<m_playlist.size())
			{
				res = true;
			}
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool MusicDaemon::tracksListJSON(int lowId, int highId, QJsonDocument& doc)
{
	bool res = false;
	
	if(lowId>=0 && lowId<m_playlist.size() && highId>=0 && highId<m_playlist.size())
	{
		QJsonObject root;
		QJsonArray tracks;
		
		root["low"] = lowId;
		root["high"] = highId;
		
		for(int id = lowId; id <= highId; id++)
		{
			QJsonObject obj = getTrackJSON(id);
			tracks.append(obj);
		}
		root["tracks"] = tracks;
		
		doc.setObject(root);
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onWebTrack(network::http::HTTPReceive *recieve)
{
	int resCode;
	int lowId, highId;
	
	if(recieve != 0)
	{
		const network::http::Query& q = recieve->header().query();
		
		if(q.value().isEmpty())
		{
			lowId = 0;
			highId = m_playlist.size() - 1;
			resCode = 200;
		}
		else if(isQueryIdRange(q, lowId, highId))
		{
			resCode = 200;
		}
		else if(isQueryIdSingle(q, lowId))
		{
			highId = lowId;
			resCode = 200;
		}
		else
		{
			resCode = 400;
		}
		
		if(resCode == 200)
		{
			QJsonDocument doc;
			
			if(tracksListJSON(lowId, highId, doc))
			{
				network::http::Unit hdr;
				QByteArray data = doc.toJson(); // (QJsonDocument::Compact);
				network::NetArraySPtr body(new network::NetArray);

				hdr.response(200);
				hdr.add("Content-Type", "application/json");
				hdr.add("Content-Length", QString::number(data.length()));
				body->AppendRaw(data.constData(), data.length());
				recieve->connection()->postResponse(hdr);
				recieve->connection()->postBody(body);
			}
			else
			{
				resCode = 500;
			}
		}
		
		if(resCode != 200)
		{
			network::http::Unit hdr;
			hdr.response(resCode);
			recieve->connection()->postResponse(hdr);
		}
		recieve->connection()->complete();
		recieve->endProcess();
	}
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onConnectToEventStream(network::http::HTTPReceive *recieve)
{
	m_pEventStream->addReceiverWithResponse(recieve);
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::startPlaybackOfList()
{
	m_currentPlayIndex = 0;
	if(!m_audio.isNull() && m_currentPlayIndex < m_playlist.size())
	{
		m_audio->open(m_playlist.at(m_currentPlayIndex)->getFilename());
	}
}

//-------------------------------------------------------------------------------------------
// Audio interface
//-------------------------------------------------------------------------------------------

bool MusicDaemon::startAudioEngine()
{
	bool res = false;

	printf("Starting audio engine\r");
#if defined(ORCUS_WIN32)
	m_audio = audioio::AOBase::get("win32");
#elif defined(OMEGA_MACOSX)
	m_audio = audioio::AOBase::get("coreaudio");
#elif defined(OMEGA_LINUX)
	m_audio = audioio::AOBase::get("alsa");
#endif
	if(!m_audio.isNull())
	{
		connect(m_audio.data(), SIGNAL(onStart(const QString&)), this, SLOT(onAudioStart(const QString&)));
		connect(m_audio.data(), SIGNAL(onStop()), this, SLOT(onAudioStop()));
		connect(m_audio.data(), SIGNAL(onPlay()), this, SLOT(onAudioPlay()));
		connect(m_audio.data(), SIGNAL(onPause()), this, SLOT(onAudioPause()));
		connect(m_audio.data(), SIGNAL(onTime(quint64)), this, SLOT(onAudioTime(quint64)));
		connect(m_audio.data(), SIGNAL(onBuffer(tfloat32)), this, SLOT(onAudioBuffer(tfloat32)));
		connect(m_audio.data(), SIGNAL(onReadyForNext()), this, SLOT(onAudioReadyForNext()));
		connect(m_audio.data(), SIGNAL(onNoNext()), this, SLOT(onAudioNoNext()));
		connect(m_audio.data(), SIGNAL(onCrossfade()), this, SLOT(onAudioCrossfade()));
		printLog("Audio engine started successfully");
		res = true;
	}
	else
	{
		printLog("Audio engine failed to start");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::stopAudioEngine()
{
	if(!m_audio.isNull())
	{
		disconnect(m_audio.data(),SIGNAL(onStart(const QString&)),this,SLOT(onAudioStart(const QString&)));
		disconnect(m_audio.data(),SIGNAL(onStop()),this,SLOT(onAudioStop()));
		disconnect(m_audio.data(),SIGNAL(onPlay()),this,SLOT(onAudioPlay()));
		disconnect(m_audio.data(),SIGNAL(onPause()),this,SLOT(onAudioPause()));
		disconnect(m_audio.data(),SIGNAL(onTime(quint64)),this,SLOT(onAudioTime(quint64)));
		disconnect(m_audio.data(),SIGNAL(onBuffer(tfloat32)),this,SLOT(onAudioBuffer(tfloat32)));
		disconnect(m_audio.data(),SIGNAL(onReadyForNext()),this,SLOT(onAudioReadyForNext()));
		disconnect(m_audio.data(),SIGNAL(onNoNext()),this,SLOT(onAudioNoNext()));
		disconnect(m_audio.data(),SIGNAL(onCrossfade()),this,SLOT(onAudioCrossfade()));
		m_audio.clear();
		printLog("Audio engine has shutdown");
	}
}

//-------------------------------------------------------------------------------------------

int MusicDaemon::indexOfPlayItem(const QString& name) const
{
	int id, lowId, highId;
	
	id = m_currentPlayIndex;
	if(id >= 0 && id < m_playlist.size())
	{
		if(m_playlist.at(id)->getFilename() != name)
		{
			bool loop = true;
		
			lowId  = highId = m_currentPlayIndex;
			while(loop)
			{
				highId++;
				lowId--;
				if(highId < m_playlist.size() && m_playlist.at(highId)->getFilename() == name)
				{
					id = highId;
					loop = false;
				}
				else if(lowId >= 0 && m_playlist.at(lowId)->getFilename() == name)
				{
					id = lowId;
					loop = false;
				}
			}
		} 
	}
	else
	{
		id = 0;
	}
	return id;
}

//-------------------------------------------------------------------------------------------
// id : 1
// event: onAudioStart
// data: { "trackId": 3 }
//-------------------------------------------------------------------------------------------

void MusicDaemon::postAudioEvent(const QString& eventName)
{
	if(m_currentPlayIndex >= 0 && m_currentPlayIndex < m_playlist.size())
	{
		QString dataStr;
		QJsonDocument doc;
		QJsonObject jData;
		network::http::EventStreamItem item;
	
		item.id() = m_nextEventId++;
		item.event() = eventName;
	
		jData["trackId"] = m_currentPlayIndex;
		doc.setObject(jData);
		item.data() = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
	
		m_pEventStream->post(item);
	}
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioStart(const QString& name)
{
	if(m_nextCount > 0)
	{
		m_currentPlayIndex++;
		if(m_currentPlayIndex >= m_playlist.size())
		{
			m_currentPlayIndex = 0;
		}
	}
	m_nextCount = 0;

	m_currentPlayIndex = indexOfPlayItem(name);
	postAudioEvent("onAudioStart");
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioPlay()
{
	postAudioEvent("onAudioPlay");
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioPause()
{
	postAudioEvent("onAudioPause");
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioStop()
{
	m_nextCount = 0;
	postAudioEvent("onAudioStop");
}

//-------------------------------------------------------------------------------------------
// id : 1
// event: onAudioTime
// data: { "trackId": 3, "playTime": 12345 }
//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioTime(quint64 t)
{
	common::TimeStamp pT(t);

	if(m_currentPlayIndex >= 0 && m_currentPlayIndex < m_playlist.size())
	{
		QString dataStr;
		QJsonDocument doc;
		QJsonObject jData;
		network::http::EventStreamItem item;
		track::info::InfoSPtr pTrack = m_playlist.at(m_currentPlayIndex);
		
		if(pT > pTrack->length())
		{
			pT = pTrack->length();
		}
		
		item.id() = m_nextEventId++;
		item.event() = "onAudioTime";
	
		jData["trackId"] = m_currentPlayIndex;
		jData["playTime"] = static_cast<tint64>(pT);
		doc.setObject(jData);
		item.data() = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
	
		m_pEventStream->post(item);
	}	
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioBuffer(tfloat32 percent)
{}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioReadyForNext()
{
	m_nextCount++;
	if(m_nextCount <= 1)
	{
		int nextId = m_currentPlayIndex + 1;
		if(nextId >= m_playlist.size())
		{
			nextId = 0;
		}
		
		track::info::InfoSPtr pTrack = m_playlist.at(nextId);
		m_audio->next(pTrack->getFilename());
	}
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioNoNext()
{
	onAudioReadyForNext();
}

//-------------------------------------------------------------------------------------------

void MusicDaemon::onAudioCrossfade()
{}

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
