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
		network::http::HTTPService *m_webService;
		network::http::HTTPServer *m_webServer;
		
		void printLog(const QString& msg) const;
		
		QString appHomeDirectory() const;
		
	private slots:
	
		void onStartService();
		void onStopService();
		
		void onWebRoot(network::http::HTTPReceive *recieve);
};

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
