//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_WEBSERVICE_OMEGAPLWEBINTERFACE_H
#define __PLAYERAPP_WEBSERVICE_OMEGAPLWEBINTERFACE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCInterfaceBase.h"

#include <QJsonDocument>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaPLWebInterface : public IPCInterfaceBase
{
	public:
		OmegaPLWebInterface();
		virtual ~OmegaPLWebInterface();
		
		// { "function": "getFullPlaylist" }
		QJsonDocument getFullPlaylist();
		
	protected:
	
		virtual void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
