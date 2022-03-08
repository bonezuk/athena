//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_WEBSERVICE_OMEGAWEBSERVICE_H
#define __PLAYERAPP_WEBSERVICE_OMEGAWEBSERVICE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/webservice/inc/HTTPFileTransfer.h"

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
		
		void printError(const tchar *strR, const tchar *strE) const;
		bool isValidSetup() const;
		
	private slots:
		void onStartService();
		void onStopService();
		
		void onWebRoot(network::http::HTTPReceive *recieve);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
