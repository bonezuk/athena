//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERAPP_OMEGAWEBEVENTHANDLER_H
#define __PLAYERAPP_PLAYERAPP_OMEGAWEBEVENTHANDLER_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/HTTPEventBroker.h"
#include "playerapp/playercommon/inc/WebEventInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaWebEventHandler : public WebEventInterface
{
	public:
		OmegaWebEventHandler();
		virtual ~OmegaWebEventHandler();
		
		virtual bool registerConnection(network::http::HTTPReceive *recieve);
		
	protected:
	
		HTTPEventBroker m_broker;
	
		virtual void printError(const char *strR, const char *strE) const;
		virtual void sendEvent(const QJsonDocument& doc);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
