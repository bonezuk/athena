//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_WEBSERVICE_OMEGAWEBSERVERMAIN_H
#define __PLAYERAPP_WEBSERVICE_OMEGAWEBSERVERMAIN_H
//-------------------------------------------------------------------------------------------

#include "playerapp/webservice/inc/OmegaWebServiceWS.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class OmegaWebServerMain : public QCoreApplication
{
	Q_OBJECT
	public:
		OmegaWebServerMain(const QString& rootDir, int& argc, char **argv);
		virtual ~OmegaWebServerMain();
		
	private:
		OmegaWebServiceWS *m_webService;
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
