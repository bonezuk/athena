//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_WEBEVENTBUSINTERFACE_H
#define __PLAYERAPP_PLAYLISTMANAGER_WEBEVENTBUSINTERFACE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCInterfaceBase.h"
#include "playerapp/playercommon/inc/WebEventInterface.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class WebEventBusInterface : public IPCInterfaceBase, public WebEventInterface
{
	public:
		WebEventBusInterface();
		virtual ~WebEventBusInterface();

	protected:
	
		virtual void printError(const char *strR, const char *strE) const;
		virtual void sendEvent(const QJsonDocument& doc);
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
