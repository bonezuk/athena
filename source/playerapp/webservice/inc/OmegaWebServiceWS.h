//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_WEBSERVICE_OMEGAWEBSERVICEWS_H
#define __PLAYERAPP_WEBSERVICE_OMEGAWEBSERVICEWS_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/OmegaWebService.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class OmegaWebServiceWS : public OmegaWebService
{
	public:
		OmegaWebServiceWS(const QString& rootDir);
		virtual ~OmegaWebServiceWS();
		
	protected:	
		virtual void printError(const tchar *strR, const tchar *strE) const;
		virtual bool setupWebEvents();
		virtual void setupPLWebInterface();
		virtual void stopEvents();
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
