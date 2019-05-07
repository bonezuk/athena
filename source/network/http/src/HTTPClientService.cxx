#include "network/http/inc/HTTPClientService.h"
#include "network/http/inc/HTTPClient.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace http
{
//-------------------------------------------------------------------------------------------

CONCRETE_QOBJECT_FACTORY_CLASS_IMPL(ServiceFactory,Service, \
                                    HTTPClientServiceFactory,HTTPClientService, \
                                    "http_client",false)

//-------------------------------------------------------------------------------------------
// HTTPClientServiceEvent
//-------------------------------------------------------------------------------------------

HTTPClientServiceEvent::HTTPClientServiceEvent(HTTPClientServiceEventType t) : QEvent(static_cast<QEvent::Type>(t))
{
	m_threadId = QThread::currentThreadId();
}

//-------------------------------------------------------------------------------------------

Qt::HANDLE HTTPClientServiceEvent::threadId()
{
	return m_threadId;
}

//-------------------------------------------------------------------------------------------
// HTTPClientService
//-------------------------------------------------------------------------------------------

HTTPClientService::HTTPClientService(QObject *parent) : TCPClientService(parent)
{
	m_serviceThreadId = QThread::currentThreadId();
}

//-------------------------------------------------------------------------------------------

HTTPClientService::~HTTPClientService()
{}

//-------------------------------------------------------------------------------------------

void HTTPClientService::printError(const tchar *strR,const tchar *strE) const
{
	common::Log::g_Log << "HTTPClientService::" << strR << " - " << strE << "." << common::c_endl;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<HTTPClient> HTTPClientService::getClient()
{
	QSharedPointer<HTTPClient> client;
	
	if(m_serviceThreadId!=QThread::currentThreadId())
	{
		HTTPClientServiceEvent *e = new HTTPClientServiceEvent(HTTPClientServiceEvent::e_newHTTPClientEvent);
		ServiceWaitCondition *c = getCondition();
		QCoreApplication::postEvent(this,e);
		c->wait();
		client = c->sharedPtr().dynamicCast<HTTPClient>();
	}
	else
	{
		client = onGetClient();
	}
	return client;
}

//-------------------------------------------------------------------------------------------

bool HTTPClientService::event(QEvent *e)
{
	if(e!=0 && static_cast<HTTPClientServiceEvent::HTTPClientServiceEventType>(e->type())>=HTTPClientServiceEvent::e_newHTTPClientEvent)
	{
		HTTPClientServiceEvent::HTTPClientServiceEventType t = static_cast<HTTPClientServiceEvent::HTTPClientServiceEventType>(e->type());
		
		switch(t)
		{
			case HTTPClientServiceEvent::e_newHTTPClientEvent:
				{
					QSharedPointer<HTTPClient> client = onGetClient();
					QSharedPointer<QObject> cObject = client.dynamicCast<QObject>();
					processCustomEvent(reinterpret_cast<HTTPClientServiceEvent *>(e),cObject);
				}
				break;
				
			default:
				return QObject::event(e);
		}
		return true;
	}
	else
	{
		return QObject::event(e);
	}
}

//-------------------------------------------------------------------------------------------

void HTTPClientService::processCustomEvent(HTTPClientServiceEvent *e,QSharedPointer<QObject>& result)
{
	ServiceWaitCondition *c;
	
	c = getCondition(e->threadId());
	c->setSharedPointer(result);
	c->wake();
	e->accept();
}

//-------------------------------------------------------------------------------------------

QSharedPointer<HTTPClient> HTTPClientService::onGetClient()
{
	QSharedPointer<HTTPClient> c(new HTTPClient(this,this));
	return c;
}

//-------------------------------------------------------------------------------------------
} // namespace http
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
