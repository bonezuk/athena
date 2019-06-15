#include "daemon/inc/MusicClient.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace daemon
{
//-------------------------------------------------------------------------------------------

MusicClient::MusicClient(QObject *parent) : QObject(parent),
	m_hostName(),
	m_clients()
{
	network::Resource::instance();
	network::Controller::ControllerSPtr ctrl(network::Controller::instance());
	m_webClientService = dynamic_cast<network::http::HTTPClientService *>(ctrl->getService("http_client"));
}

//-------------------------------------------------------------------------------------------

MusicClient::~MusicClient()
{}

//-------------------------------------------------------------------------------------------

QString MusicClient::lastConnectedHost()
{
	QString lastHost;
	QSettings settings;

	settings.beginGroup("daemon");
	if(settings.contains("last_host"))
	{
		lastHost = settings.value("last_host").toString();
	}
	else
	{
		lastHost = "localhost";
	}
	settings.endGroup();
	return lastHost;
}

//-------------------------------------------------------------------------------------------

void MusicClient::setLastConnectedHost(const QString& hostName)
{
	if(!hostName.isEmpty())
	{
		QSettings settings;

		settings.beginGroup("daemon");
		settings.setValue("last_host", QVariant(hostName));
		settings.endGroup();
	}
}

//-------------------------------------------------------------------------------------------

void MusicClient::runTrackRequest()
{
	QSharedPointer<network::http::HTTPClient> client = m_webClientService->getClient();
	client->setHost(m_hostName, MUSIC_DAEMON_PORT);
	
	network::http::HTTPCTransaction& tran = client->newTransaction(true);
	
	addClient(client);
	
	QObject::connect(client.data(), SIGNAL(onTransactionError(network::http::HTTPCTransaction*,const QString&)),
		this, SLOT(onTransactionError(network::http::HTTPCTransaction*,const QString&)));
	QObject::connect(client.data(), SIGNAL(onError(network::http::HTTPClient*,const QString&)),
		this, SLOT(onError(network::http::HTTPClient*,const QString&)));
	QObject::connect(client.data(), SIGNAL(onComplete(network::http::HTTPClient*)),
		this, SLOT(onClientComplete(network::http::HTTPClient*)));
	QObject::connect(client.data(), SIGNAL(onTransaction(network::http::HTTPCTransaction *)),
		this, SLOT(onTrackTransaction(network::http::HTTPCTransaction*)));
	
	network::http::Unit req;
	req.request(network::http::Unit::e_Get, "/track");
	tran.request() = req;
	
	client->enqueueTransaction(tran.id());
	client->run();
}

//-------------------------------------------------------------------------------------------

void MusicClient::runEventRequest()
{
	QSharedPointer<network::http::HTTPClient> client = m_webClientService->getClient();
	client->setHost(m_hostName, MUSIC_DAEMON_PORT);
	network::http::HTTPCTransaction& tran = client->newTransaction();
	
	addClient(client);
	
	QObject::connect(client.data(), SIGNAL(onTransactionError(network::http::HTTPCTransaction*,const QString&)),
		this, SLOT(onTransactionError(network::http::HTTPCTransaction*,const QString&)));
	QObject::connect(client.data(), SIGNAL(onError(network::http::HTTPClient*,const QString&)),
		this, SLOT(onError(network::http::HTTPClient*,const QString&)));
	QObject::connect(client.data(), SIGNAL(onComplete(network::http::HTTPClient*)),
		this, SLOT(onClientComplete(network::http::HTTPClient*)));
	QObject::connect(client.data(), SIGNAL(onStream(network::http::HTTPCTransaction*,const network::http::EventStreamItem&)),
		this, SLOT(onEventStream(network::http::HTTPCTransaction*,const network::http::EventStreamItem&)));
		
	network::http::Unit req;
	req.request(network::http::Unit::e_Get, "/event");
	tran.request() = req;
	tran.setStreaming(true);
	
	client->enqueueTransaction(tran.id());
	client->run();
}

//-------------------------------------------------------------------------------------------

void MusicClient::connect(const QString& hostName)
{
	if(!m_hostName.isEmpty())
	{
		disconnect();
	}
	if(!hostName.isEmpty())
	{
		m_hostName = hostName;
		runTrackRequest();
		runEventRequest();
	}
}

//-------------------------------------------------------------------------------------------

void MusicClient::disconnect()
{
	if(!m_hostName.isEmpty())
	{
		common::TimeStamp timeout = common::TimeStamp::now() + 1.0;
		
		for(QVector<QSharedPointer<network::http::HTTPClient> >::iterator ppI = m_clients.begin(); ppI != m_clients.end(); ppI++)
		{
			QSharedPointer<network::http::HTTPClient>& pClient = *ppI;
			pClient->shutdown();
		}
		while(common::TimeStamp::now() < timeout && !m_clients.isEmpty())
		{
			QCoreApplication::processEvents();
		}
		m_clients.clear();
		m_hostName = "";
	}
}

//-------------------------------------------------------------------------------------------

void MusicClient::addClient(QSharedPointer<network::http::HTTPClient>& pClient)
{
	m_clients.append(pClient);
}

//-------------------------------------------------------------------------------------------

void MusicClient::removeClient(network::http::HTTPClient *client)
{
	for(QVector<QSharedPointer<network::http::HTTPClient> >::iterator ppI = m_clients.begin(); ppI != m_clients.end(); ppI++)
	{
		QSharedPointer<network::http::HTTPClient>& pLClient = *ppI;
		if(pLClient.data() == client)
		{
			m_clients.erase(ppI);
			break;
		}
	}
}

//-------------------------------------------------------------------------------------------

void MusicClient::onTransactionError(network::http::HTTPCTransaction *trans,const QString& err)
{
	QString errStr = "Transaction error in Daemon. " + err;
	emit onError(errStr);
}

//-------------------------------------------------------------------------------------------

void MusicClient::onError(network::http::HTTPClient *client,const QString& err)
{
	QString errStr = "Error in Daemon. " + err;
	removeClient(client);
	emit onError(errStr);
}

//-------------------------------------------------------------------------------------------

void MusicClient::onClientComplete(network::http::HTTPClient *client)
{
	removeClient(client);
}

//-------------------------------------------------------------------------------------------

void MusicClient::onTrackTransaction(network::http::HTTPCTransaction *trans)
{
	if(trans->request().resource() == "/track")
	{
		if(trans->request().query().isEmpty())
		{
			setLastConnectedHost(m_hostName);
		}
		processTrackList(trans);
	}
}

//-------------------------------------------------------------------------------------------

void MusicClient::processTrackList(network::http::HTTPCTransaction *trans)
{
	QVector<QSharedPointer<track::info::Info> > trackList;
	
	if(trans->response().response() == 200)
	{
		const char *jsonTxt = reinterpret_cast<const char *>(trans->responseData().GetData());
		int jsonLen = trans->responseData().GetSize();
		QByteArray jsonArr(jsonTxt, jsonLen);

		QJsonDocument doc = QJsonDocument::fromJson(jsonArr);
		
		if(doc.isObject())
		{
			QJsonObject root = doc.object();
			QJsonArray tracks = root["tracks"].toArray();
			
			for(int idx = 0; idx < tracks.size(); idx++)
			{
				QJsonObject track = tracks[idx].toObject();
				int id = track["id"].toInt(-1);
				if(id >= 0)
				{
					QSharedPointer<track::info::DaemonInfo> item(new track::info::DaemonInfo(id, this));
					
					item->setFilename(track["filename"].toString());
					tuint64 tLen = static_cast<tuint64>(track["length"].toVariant().toULongLong());
					item->length() = tLen;
					item->setBitrate(track["bitrate"].toInt());
					item->setNoChannels(track["channels"].toInt());
					item->setFrequency(track["frequency"].toInt());
					item->artist() = track["artist"].toString();
					item->title() = track["title"].toString();
					item->album() = track["album"].toString();
					item->year() = track["year"].toString();
					item->comment() = track["comment"].toString();
					item->genre() = track["genre"].toString();
					item->track() = track["track"].toString();
					item->disc() = track["disc"].toString();
					item->composer() = track["composer"].toString();
					item->originalArtist() = track["original_artist"].toString();
					item->copyright() = track["copyright"].toString();
					item->encoder() = track["encoder"].toString();
					item->imageUrl() = track["image"].toString();
					
					QSharedPointer<track::info::Info> infoItem = item.dynamicCast<track::info::Info>();
					trackList.append(infoItem);
				}
			}
		}
	}
	
	emit onLoadTracks(trackList);
}

//-------------------------------------------------------------------------------------------

void MusicClient::onEventStream(network::http::HTTPCTransaction *trans,const network::http::EventStreamItem& item)
{
	QJsonDocument doc;
	
	doc = QJsonDocument::fromJson(item.data().toUtf8());
	if(doc.isObject())
	{
		QJsonObject eventData(doc.object());
		
		if(item.event() == "onAudioTime")
		{
			int trackId = eventData["trackId"].toInt();
			tuint64 tLen = static_cast<tuint64>(eventData["playTime"].toVariant().toULongLong());
			emit onAudioTime(trackId, tLen);
		}
		else if(item.event() == "onAudioStart" || item.event() == "onAudioPlay")
		{
			int trackId = eventData["trackId"].toInt();
			emit onAudioPlay(trackId);
		}
		else if(item.event() == "onAudioPause")
		{
			int trackId = eventData["trackId"].toInt();
			emit onAudioPause(trackId);
		}
		else if(item.event() == "onAudioStop")
		{
			int trackId = eventData["trackId"].toInt();
			emit onAudioStop();
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
