#include "network/http/inc/EventStreamHandler.h"
#include "network/http/inc/HTTPConnection.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace http
{
//-------------------------------------------------------------------------------------------

EventStreamHandler::EventStreamHandler(QObject *parent) : QObject(parent),
	m_lock(),
	m_receivers()
{}

//-------------------------------------------------------------------------------------------

EventStreamHandler::~EventStreamHandler()
{}

//-------------------------------------------------------------------------------------------

void EventStreamHandler::addReceiverWithResponse(HTTPReceive *pReceiver)
{
	if(pReceiver != 0)
	{
		Unit streamResponse;

		m_lock.lock();
		m_receivers.insert(pReceiver);
		m_lock.unlock();
	
		streamResponse.response(200);
		streamResponse.add("Content-Type", "text/event-stream");
		pReceiver->connection()->postResponse(streamResponse);
	}
}

//-------------------------------------------------------------------------------------------

void EventStreamHandler::post(const EventStreamItem& item)
{
	QString txt = item.toString();
	network::NetArraySPtr body(new network::NetArray);
	
	body->AppendRaw(txt.toUtf8().constData(), txt.toUtf8().length());
	m_lock.lock();
	for(QSet<HTTPReceive *>::iterator ppI = m_receivers.begin(); ppI != m_receivers.end(); ppI++)
	{
		HTTPReceive *pReceiver = *ppI;
		pReceiver->connection()->postBody(body);
	}
	m_lock.unlock();
}

//-------------------------------------------------------------------------------------------

void EventStreamHandler::onComplete(HTTPReceive *pReceiver)
{
	m_lock.lock();
	{
		QSet<HTTPReceive *>::iterator ppI = m_receivers.find(pReceiver);
		if(ppI != m_receivers.end())
		{
			m_receivers.erase(ppI);
		}
	}
	m_lock.unlock();
}

//-------------------------------------------------------------------------------------------
} // namespace http
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
