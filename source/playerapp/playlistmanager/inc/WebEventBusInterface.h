//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_WEBEVENTBUSINTERFACE_H
#define __PLAYERAPP_PLAYLISTMANAGER_WEBEVENTBUSINTERFACE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCInterfaceBase.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class WebEventBusInterface : public IPCInterfaceBase
{
	public:
		WebEventBusInterface();
		virtual ~WebEventBusInterface();
		
		virtual void onPlaybackTime(const common::TimeStamp& tS);
		
	protected:
	
		virtual void printError(const char *strR, const char *strE) const;
		virtual void sendEvent(const QJsonDocument& doc);
		virtual QJsonDocument createJsonEvent(const QString& eventName, const QJsonObject& data);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
