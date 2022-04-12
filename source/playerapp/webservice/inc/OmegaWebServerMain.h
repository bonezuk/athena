//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_WEBSERVICE_OMEGAWEBSERVERMAIN_H
#define __PLAYERAPP_WEBSERVICE_OMEGAWEBSERVERMAIN_H
//-------------------------------------------------------------------------------------------

#include "playerapp/webservice/inc/OmegaWebServiceWS.h"

//-------------------------------------------------------------------------------------------
namespace orcus
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
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
