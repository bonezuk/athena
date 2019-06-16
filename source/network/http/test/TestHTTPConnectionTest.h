//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_HTTP_TEST_TESTHTTPCONNECTION_H
#define __ORCUS_NETWORK_HTTP_TEST_TESTHTTPCONNECTION_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"
#include "network/inc/Controller.h"
#include "network/http/inc/HTTPService.h"
#include "network/http/inc/HTTPServer.h"
#include "network/http/inc/HTTPConnection.h"
#include "network/http/inc/HTTPClientService.h"
#include "network/http/inc/HTTPClient.h"
#include "network/http/inc/EventStreamHandler.h"
#include "common/inc/TimeStamp.h"

//-------------------------------------------------------------------------------------------

#define TEST_DAEMON_PORT 13768

using namespace orcus;

//-------------------------------------------------------------------------------------------

class TestHTTPClientThread : public QThread
{
	public:
		const int c_numberOfPersistentIds = 10;

	Q_OBJECT
	public:
		TestHTTPClientThread(int unitTestCase, QObject *parent = 0);
		virtual ~TestHTTPClientThread();
		bool isError() const;
		
	private:
		int m_unitTestCase;
		bool m_isError;
		int m_streamCounter;
		network::http::HTTPClientService *m_webClientService;
		
		QSharedPointer<network::http::HTTPClient> m_pClient;
		QVector<int> m_idList;
		QSet<int> m_idTransSet;
		QSet<int> m_idReplySet;
		QTimer *m_timer;
		
		virtual void printError(const char *strR, const char *strE);
		virtual bool startHTTPClient();
		virtual void startEventStream();
		virtual void run();

		virtual void populateIdList();
		virtual bool checkReplyIdSet();
		virtual void queueNextQuery();
		virtual void startPersistentTest();
		
	private slots:
		void onTransactionError(network::http::HTTPCTransaction *trans,const QString& err);
		void onStream(network::http::HTTPCTransaction *trans,const network::http::EventStreamItem& item);
		void onError(network::http::HTTPClient *client,const QString& err);
		void onComplete(network::http::HTTPClient *client);

		void onPersistentTimer();
		void onPersistentTransaction(network::http::HTTPCTransaction *pTrans);
		void onPersistentComplete(network::http::HTTPClient *client);
};

//-------------------------------------------------------------------------------------------

class TestHTTPConnection : public QCoreApplication
{
	Q_OBJECT
	public:
		TestHTTPConnection(int unitTestCase, int argc, char **argv);
		virtual ~TestHTTPConnection();
		bool isError() const;
		
	private:
		int m_unitTestCase;
		bool m_isError;
		network::http::HTTPService *m_webService;
		network::http::HTTPServer *m_webServer;
		network::http::EventStreamHandler *m_eventStreamHandler;
		QTimer *m_eventStreamTimer;
		int m_eventStreamCounter;
		TestHTTPClientThread *m_client;
		
		bool m_isClientComplete;
		bool m_isServerComplete;
		
		virtual void printError(const char *strR, const char *strE);
		virtual bool startWebServer();
		
	private slots:
		virtual void onStartup();
		virtual void onShutdown();
		virtual void onEventStreamTest(network::http::HTTPReceive *);
		virtual void onEventTimer();
		
		virtual void onStreamClientComplete();
		virtual void onStreamServerComplete();

		virtual void onPersistentConnectionTest(network::http::HTTPReceive *receive);
};

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
