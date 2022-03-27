//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_WEBSERVICE_OMEGAWEBSERVICE_H
#define __PLAYERAPP_WEBSERVICE_OMEGAWEBSERVICE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/webservice/inc/HTTPFileTransfer.h"
#include "playerapp/webservice/inc/OmegaPLWebInterface.h"
#include "playerapp/webservice/inc/OmegaWebEventService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaWebService : public QCoreApplication
{
	Q_OBJECT
	public:
		const int c_serverPort = 5121;
	
	public:
		OmegaWebService(const QString& rootDir, int argc, char **argv);
		virtual ~OmegaWebService();
		
	private:
		QString m_rootDir;
		network::http::HTTPService *m_webService;
		network::http::HTTPServer *m_webServer;
		QSharedPointer<OmegaPLWebInterface> m_pWebInterface;
		QSharedPointer<OmegaWebEventService> m_pWebEvents;
		
		void printError(const tchar *strR, const tchar *strE) const;
		bool isValidSetup() const;
		void postResponse(network::http::HTTPReceive *receive, int code);
		bool setupWebEvents();
		
	private slots:
		void onStartService();
		void onStopService();
		
		void onWebRoot(network::http::HTTPReceive *recieve);
		void onPlaylistAPI(network::http::HTTPReceive *receive);
		void onEventStream(network::http::HTTPReceive *receive);
		void onStateAPI(network::http::HTTPReceive *receive);
		void onPressPlay(network::http::HTTPReceive *receive);
		void onStartPlayback(network::http::HTTPReceive *receive);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
