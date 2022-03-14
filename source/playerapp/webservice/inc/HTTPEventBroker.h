//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_WEBSERVICE_HTTPEVENTBROKER_H
#define __PLAYERAPP_WEBSERVICE_HTTPEVENTBROKER_H
//-------------------------------------------------------------------------------------------

#include "playerapp/webservice/inc/HTTPFileTransfer.h"

#include <QJsonDocument>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class HTTPEventBroker
{
	public:
		HTTPEventBroker();
		virtual ~HTTPEventBroker();
		
		virtual bool registerConnection(network::http::HTTPReceive *recieve);
		
		virtual void postEvent(const QJsonDocument& data);
		
	private:
		int m_nextID;
		QList<network::http::HTTPReceive *> m_connections;
	
		virtual void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

