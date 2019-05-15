//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_HTTP_EVENTSTREAMHANDLER_H
#define __ORCUS_NETWORK_HTTP_EVENTSTREAMHANDLER_H
//-------------------------------------------------------------------------------------------

#include "network/http/inc/EventStreamItem.h"
#include "network/http/inc/HTTPServer.h"

//-------------------------------------------------------------------------------------------
/* The EventStreamHandler acts to pass information of events (e.g. playNextTrack, playbackTime)
from any thread, such that they are queued on a FIFO, which in turn are transmitted to the open
connection to the client */
//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace http
{
//-------------------------------------------------------------------------------------------

class HTTP_EXPORT EventStreamHandler : public QObject
{
	Q_OBJECT
	public:
		EventStreamHandler(QObject *parent = 0);
		virtual ~EventStreamHandler();
		virtual void addReceiverWithResponse(HTTPReceive *pReceiver);
		virtual void post(const EventStreamItem& item);
	private:
		QMutex m_lock;
		QSet<HTTPReceive *> m_receivers;
	private slots:
		virtual void onComplete(network::http::HTTPReceive *pReceiver);
};

//-------------------------------------------------------------------------------------------
} // namespace http
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

