//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_WEBSERVICE_OMEGAPLWEBINTERFACEWS_H
#define __PLAYERAPP_WEBSERVICE_OMEGAPLWEBINTERFACEWS_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCInterfaceBase.h"
#include "playerapp/playercommon/inc/OmegaPLWebInterface.h"

#include <QJsonDocument>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaPLWebInterfaceWS : public IPCInterfaceBase, public OmegaPLWebInterface
{
	public:
		OmegaPLWebInterfaceWS();
		virtual ~OmegaPLWebInterfaceWS();
		
		// { "function": "getFullPlaylist" }
		virtual QJsonDocument getFullPlaylist();
		
		// { "function": "getPlaybackState" }
		virtual QJsonDocument getPlaybackState();
		
		// { "function": "onPressPlay" }
		virtual void onPressPlay();
		
		// { "function": "onStartPlaying", "id": 12345 }
		virtual void onStartPlaying(tuint64 id);
		
	protected:
	
		virtual void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
