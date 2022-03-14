//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_WEBSERVICE_OMEGAWEBEVENTSERVICE_H
#define __PLAYERAPP_WEBSERVICE_OMEGAWEBEVENTSERVICE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCService.h"
#include "playerapp/webservice/inc/HTTPEventBroker.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaWebEventService : public IPCService
{
	Q_OBJECT
	
	public:
		OmegaWebEventService(QObject *parent = 0);
		virtual ~OmegaWebEventService();
		
		virtual bool registerConnection(network::http::HTTPReceive *recieve);
		
	protected:
		
		HTTPEventBroker m_broker;
	
		virtual void printError(const char *strR, const char *strE) const;
		
		virtual void handleRPCJson(const QJsonDocument& doc);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

