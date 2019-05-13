//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_HTTP_HTTPCLIENT_H
#define __ORCUS_NETWORK_HTTP_HTTPCLIENT_H
//-------------------------------------------------------------------------------------------

#include "network/inc/TCPConnClientSocket.h"
#include "network/http/inc/HTTPClientService.h"
#include "network/http/inc/EventStreamItem.h"

#include <QMutex>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace http
{
//-------------------------------------------------------------------------------------------

class HTTP_EXPORT HTTPCTransaction
{
	public:
		HTTPCTransaction(HTTPClient *client,tint id);
		HTTPCTransaction(const HTTPCTransaction& rhs);
		virtual ~HTTPCTransaction();
				
		const HTTPCTransaction& operator = (const HTTPCTransaction& rhs);
		
		virtual HTTPClient *client();
		virtual tint id() const;
				
		virtual Unit& request();
		virtual const Unit& request() const;
		virtual NetArray& requestData();
		virtual const NetArray& requestData() const;
				
		virtual Unit& response();
		virtual const Unit& response() const;
		virtual NetArray& responseData();
		virtual const NetArray& responseData() const;

		virtual bool isStreaming() const;
		virtual void setStreaming(bool isOn);

		virtual bool isComplete() const;
		virtual void setComplete(bool isComplete);

	protected:
		
		HTTPClient *m_client;
		tint m_id;
		
		Unit m_request;
		NetArray m_requestData;
		Unit m_response;
		NetArray m_responseData;
		
		bool m_isStreaming;
		bool m_isComplete;
				
		virtual void copy(const HTTPCTransaction& rhs);
};

//-------------------------------------------------------------------------------------------

class HTTP_EXPORT HTTPClient : public TCPConnClientSocket
{
	public:
		Q_OBJECT
		
		typedef enum
		{
			e_onHTTPClientRun = QEvent::User + 1030
		} HTTPClientEventType;
		
	public:
		HTTPClient(Service *svr,QObject *parent = 0);
		virtual ~HTTPClient();
		
		// external interface for http client
		virtual void setHost(const tchar *host,tint port);
		virtual void setHost(const QString& host,tint port);
		virtual void setProxy(const tchar *host,tint port);
		virtual void setProxy(const QString& host,tint port);
		
		// Create a series of HTTP transactions that are executed on calling run
		virtual tint newTransaction();
		virtual HTTPCTransaction& transaction(tint id);
		
		virtual void run();
		
		// internal interface as called in by network service thread
		virtual bool process();
		
	protected:
		
		HTTPClientService *m_httpCService;
		Qt::HANDLE m_threadId;
		
		QList<HTTPCTransaction *> m_transactions;
		
		QString m_hostName;
		tint m_hostPort;
		QString m_proxyName;
		tint m_proxyPort;
		
		tint m_currentID;
		tint m_state;
		
		tint m_bodyOffset;
		tint m_bodyLength;
		
		tint m_chunkState;
		QString m_chunkLine;
		NetArray m_chunkArray;
		
		QMutex m_lock;
		
		virtual void printError(const tchar *strR,const tchar *strE) const;
		virtual void printError(const tchar *strR,const tchar *strE,tint eNo) const;
		
		virtual bool event(QEvent *e);
		
		virtual void doRun();
		
		virtual bool doConnect();
		virtual void doRequest();
		virtual void doResponse(bool& loop);
		virtual void doResData(bool& loop);
		virtual void doResChunked(bool& loop);
		virtual void doResStreamed(bool& loop);
		
		virtual bool isBody(const Unit& item) const;
		virtual bool isChunked(const Unit& item) const;
		virtual bool isStreaming(const Unit& item) const;
		
		virtual bool parseChunkHeader(const QString& str,tint& size,QString& field);
		
	signals:
	
		void onTransaction(network::http::HTTPCTransaction*);
		void onTransactionChunk(network::http::HTTPCTransaction*,const QString&,network::NetArrayPtr);
		void onTransactionError(network::http::HTTPCTransaction*,const QString&);
		void onStream(network::http::HTTPCTransaction*,const network::http::EventStreamItem&);
		void onError(network::http::HTTPClient*,const QString&);
		void onComplete(network::http::HTTPClient*);
};

//-------------------------------------------------------------------------------------------
} // namespace http
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

