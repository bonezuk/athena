//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_DAEMON_MUSICDAEMON_H
#define __ORCUS_DAEMON_MUSICDAEMON_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"
#include "network/inc/Controller.h"
#include "network/http/inc/HTTPService.h"
#include "network/http/inc/HTTPServer.h"
#include "network/http/inc/HTTPConnection.h"
#include "daemon/inc/DaemonDLL.h"
#include "dlna/inc/DiskIF.h"
#include "track/info/inc/Info.h"
#include "track/db/inc/TrackDB.h"
#include "track/db/inc/PlaylistAbstractIO.h"

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
		
		QVector<track::info::InfoSPtr> m_playlist;
		
		void printLog(const QString& msg) const;
		
		void printCommandLineUsage() const;
		void collectCommandLine(int argc, char **argv);
		bool processCommandLine();
		bool loadPlaylist(const QString& fileName);
		bool loadTrackDatabase();
		
		QString appHomeDirectory() const;
		
		void setupLog();
		bool startWebServer();
		void stopWebServer();
		
	private slots:
	
		void onStartService();
		void onStopService();
		
		void onWebRoot(network::http::HTTPReceive *recieve);
		
		void onAudioStart(const QString& name);
		void onAudioPlay();
		void onAudioPause();
		void onAudioTime(quint64 t);
		void onAudioBuffer(tfloat32 percent);
		void onAudioReadyForNext();
		void onAudioNoNext();
		void onAudioCrossfade();
};

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
