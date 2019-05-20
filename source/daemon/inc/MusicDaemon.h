//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_DAEMON_MUSICDAEMON_H
#define __ORCUS_DAEMON_MUSICDAEMON_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"
#include "network/inc/Controller.h"
#include "network/http/inc/HTTPService.h"
#include "network/http/inc/HTTPServer.h"
#include "network/http/inc/HTTPConnection.h"
#include "network/http/inc/EventStreamHandler.h"
#include "daemon/inc/DaemonDLL.h"
#include "dlna/inc/DiskIF.h"
#include "track/info/inc/Info.h"
#include "track/db/inc/TrackDB.h"
#include "track/db/inc/PlaylistAbstractIO.h"
#include "audioio/inc/AOBase.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace daemon
{
//-------------------------------------------------------------------------------------------

#define MUSIC_DAEMON_PORT 13766

//-------------------------------------------------------------------------------------------

class DAEMON_EXPORT MusicDaemon : public QCoreApplication
{
	Q_OBJECT
	
	public:
		MusicDaemon(int argc,char **argv);
		virtual ~MusicDaemon();
		
	private:
		QStringList m_cmdArgs;
	
		network::http::HTTPService *m_webService;
		network::http::HTTPServer *m_webServer;
		
		QSharedPointer<network::http::EventStreamHandler> m_pEventStream;
		int m_nextEventId;
		
		QVector<track::info::InfoSPtr> m_playlist;
		
		QSharedPointer<audioio::AOBase> m_audio;
		
		int m_currentPlayIndex;
		int m_nextCount;
		
		virtual void printLog(const QString& msg) const;
		
		virtual void printCommandLineUsage() const;
		virtual void collectCommandLine(int argc, char **argv);
		virtual bool processCommandLine();
		virtual bool loadPlaylist(const QString& fileName);
		virtual bool loadTrackDatabase();
		
		virtual QString appHomeDirectory() const;
		
		virtual void setupLog();
		virtual bool startWebServer();
		virtual void stopWebServer();

		virtual bool startAudioEngine();
		virtual void stopAudioEngine();
		
		virtual int indexOfPlayItem(const QString& name) const;
		virtual void postAudioEvent(const QString& eventName);
		virtual void startPlaybackOfList();

		virtual QJsonObject getTrackJSON(int id);
		virtual bool isQueryIdSingle(const network::http::Query& q, int& id);
		virtual bool isQueryIdRange(const network::http::Query& q, int& lowId, int& highId);
		virtual bool tracksListJSON(int lowId, int highId, QJsonDocument& doc);

	private slots:
	
		virtual void onStartService();
		virtual void onStopService();
		
		virtual void onWebRoot(network::http::HTTPReceive *recieve);
		virtual void onConnectToEventStream(network::http::HTTPReceive *recieve);
		virtual void onWebTrack(network::http::HTTPReceive *recieve);

		virtual void onAudioStart(const QString& name);
		virtual void onAudioPlay();
		virtual void onAudioPause();
		virtual void onAudioStop();
		virtual void onAudioTime(quint64 t);
		virtual void onAudioBuffer(tfloat32 percent);
		virtual void onAudioReadyForNext();
		virtual void onAudioNoNext();
		virtual void onAudioCrossfade();
};

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
