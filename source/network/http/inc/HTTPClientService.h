//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_HTTP_HTTPCLIENTSERVICE_H
#define __ORCUS_NETWORK_HTTP_HTTPCLIENTSERVICE_H
//-------------------------------------------------------------------------------------------

#include "network/inc/TCPClientService.h"
#include "network/http/inc/Unit.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace http
{
//-------------------------------------------------------------------------------------------

class HTTPClient;

//-------------------------------------------------------------------------------------------

class HTTP_EXPORT HTTPClientServiceEvent : public QEvent
{
	public:
		typedef enum
		{
			e_newHTTPClientEvent = QEvent::User + 1040
		} HTTPClientServiceEventType;
			
	public:
		HTTPClientServiceEvent(HTTPClientServiceEventType t);
		
		Qt::HANDLE threadId();
		
	protected:
	
		Qt::HANDLE m_threadId;
};

//-------------------------------------------------------------------------------------------

class HTTP_EXPORT HTTPClientService : public TCPClientService
{
	public:
		Q_OBJECT
		
	public:
		HTTPClientService(QObject *parent = 0);
		virtual ~HTTPClientService();
		
		virtual QSharedPointer<HTTPClient> getClient();
				
	protected:
	
		Qt::HANDLE m_serviceThreadId;
	
		virtual void printError(const tchar *strR,const tchar *strE) const;
		
		virtual bool event(QEvent *e);
		virtual QSharedPointer<HTTPClient> onGetClient();
		
		virtual void processCustomEvent(HTTPClientServiceEvent *e,QSharedPointer<QObject>& result);
};

//-------------------------------------------------------------------------------------------
} // namespace http
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

