#include "network/http/test/TestHTTPConnectionTest.h"

#include "gtest/gtest.h"

//-------------------------------------------------------------------------------------------

TestHTTPClientThread::TestHTTPClientThread(QObject *parent) : QThread(parent),
	m_isError(false),
	m_streamCounter(0),
	m_webClientService(0)
{}

//-------------------------------------------------------------------------------------------

TestHTTPClientThread::~TestHTTPClientThread()
{
	if(m_webClientService != 0)
	{
		network::Controller::ControllerSPtr ctrl(network::Controller::instance());
		ctrl->deleteService(m_webClientService);
		m_webClientService = 0;
	}
}

//-------------------------------------------------------------------------------------------

bool TestHTTPClientThread::isError() const
{
	return m_isError;
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::printError(const char *strR, const char *strE)
{
	common::Log::g_Log << "TestHTTPClientThread::" << strR << " - " << strE << common::c_endl;
	m_isError = true;
}

//-------------------------------------------------------------------------------------------

bool TestHTTPClientThread::startHTTPClient()
{
	bool res = false;
	
	try
	{
		network::Resource& nResource = network::Resource::instance();
		network::Controller::ControllerSPtr ctrl(network::Controller::instance());
		
		if(ctrl.data()!=0)
		{
			m_webClientService = dynamic_cast<network::http::HTTPClientService *>(ctrl->newService("http_client"));
			if(m_webClientService!=0)
			{
				res = true;
			}
		}
	}
	catch(common::Exception&)
	{
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::startEventStream()
{
	QSharedPointer<network::http::HTTPClient> client = m_webClientService->getClient();

	QObject::connect(client.data(), SIGNAL(onTransactionError(network::http::HTTPCTransaction*,const QString&)),
		this, SLOT(onTransactionError(network::http::HTTPCTransaction*,const QString&)));
	QObject::connect(client.data(), SIGNAL(onError(network::http::HTTPClient*,const QString&)),
		this, SLOT(onError(network::http::HTTPClient*,const QString&)));
	QObject::connect(client.data(), SIGNAL(onComplete(network::http::HTTPClient*)),
		this, SLOT(onComplete(network::http::HTTPClient*)));
	QObject::connect(client.data(), SIGNAL(onStream(network::http::HTTPCTransaction*,const network::http::EventStreamItem&)),
		this, SLOT(onStream(network::http::HTTPCTransaction*,const network::http::EventStreamItem&)));
		
	client->setHost("127.0.0.1", TEST_DAEMON_PORT);
	
	int id = client->newTransaction();
	network::http::HTTPCTransaction& tran = client->transaction(id);

	Q_ASSERT(!tran.isStreaming());
	tran.setStreaming(true);
	Q_ASSERT(tran.isStreaming());
	
	network::http::Unit req;
	req.request(network::http::Unit::e_Get, "/test/event-stream");
	tran.request() = req;
	
	client->run();
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::onTransactionError(network::http::HTTPCTransaction *trans,const QString& err)
{
	printError("onTransactionError", err.toUtf8().constData());
	exit();
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::onStream(network::http::HTTPCTransaction *trans,const network::http::EventStreamItem& item)
{
	if(m_streamCounter < 30 && !trans->isComplete())
	{
		fprintf(stdout, "client - onStream\n");
		QString n = item.toString().replace("\r\n", "\n");
		fprintf(stdout, "%s", item.toString().toUtf8().constData());
	}
	else
	{
		fprintf(stdout, "client - onStream, complete\n");
		trans->setComplete(true);
	}
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::onError(network::http::HTTPClient *client,const QString& err)
{
	printError("onError", err.toUtf8().constData());
	exit();
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::onComplete(network::http::HTTPClient *client)
{
	fprintf(stdout,"HTTPClient instance done\n");
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::run()
{
	if(startHTTPClient())
	{
		startEventStream();
		exec();
	}
	else
	{
		printError("run", "Failed to start HTTP client service");
	}
}

//-------------------------------------------------------------------------------------------

TestHTTPConnection::TestHTTPConnection(int argc, char **argv) : QCoreApplication(argc, argv),
	m_isError(false),
	m_webService(0),
	m_webServer(0),
	m_eventStreamHandler(0),
	m_eventStreamCounter(0),
	m_client(0)
{
	m_eventStreamHandler = new network::http::EventStreamHandler(this);
	m_eventStreamTimer = new QTimer(this);
	m_eventStreamTimer->setInterval(100);
	m_eventStreamTimer->setSingleShot(false);
	m_eventStreamTimer->start();
	QObject::connect(m_eventStreamTimer, SIGNAL(timeout()), this, SLOT(onEventTimer()));
	QTimer::singleShot(10, this, SLOT(onStartup()));
	QObject::connect(this, SIGNAL(aboutToQuit()), this, SLOT(onShutdown()));
}

//-------------------------------------------------------------------------------------------

TestHTTPConnection::~TestHTTPConnection()
{}

//-------------------------------------------------------------------------------------------

void TestHTTPConnection::printError(const char *strR, const char *strE)
{
	common::Log::g_Log << "TestHTTPConnection::" << strR << " - " << strE << common::c_endl;
	m_isError = true;
}

//-------------------------------------------------------------------------------------------

bool TestHTTPConnection::isError() const
{
	return (m_isError && (m_client==0 || m_client->isError()));
}

//-------------------------------------------------------------------------------------------

bool TestHTTPConnection::startWebServer()
{
	bool res = false;
	
	try
	{
		network::Resource& nResource = network::Resource::instance();
		network::Controller::ControllerSPtr ctrl(network::Controller::instance());
		
		if(ctrl.data()!=0)
		{
			m_webService = dynamic_cast<network::http::HTTPService *>(ctrl->newService("http_server"));
			if(m_webService!=0)
			{
				m_webServer = m_webService->getServer(TEST_DAEMON_PORT);
				if(m_webServer!=0)
				{
					m_webServer->connect("/test/event-stream",this,SLOT(onEventStreamTest(network::http::HTTPReceive *)));
					fprintf(stdout, "Test web server started\n");
					res = true;
				}
			}
		}
	}
	catch(common::Exception&)
	{
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void TestHTTPConnection::onStartup()
{
	bool res = false;
	
	if(startWebServer())
	{
		m_client = new TestHTTPClientThread();
		QObject::connect(m_client, SIGNAL(finished()), this, SLOT(onStreamClientComplete()));
		m_client->start();
		res = true;
	}
	
	if(!res)
	{
		quit();
	}
}

//-------------------------------------------------------------------------------------------

void TestHTTPConnection::onShutdown()
{
	network::Controller::ControllerSPtr ctrl(network::Controller::instance());

	if(m_client != 0)
	{
		if(m_client->isRunning())
		{
			m_client->quit();
			while(m_client->isRunning())
			{
				QThread::msleep(10);
			}
		}
		delete m_client;
		m_client = 0;
	}
	if(m_webService != 0)
	{
		if(m_webServer != 0)
		{
			m_webService->deleteServer(m_webServer);
			m_webServer = 0;
		}
		ctrl->deleteService(m_webService);
		m_webService = 0;
	}
	network::Controller::end();
}

//-------------------------------------------------------------------------------------------

void TestHTTPConnection::onEventStreamTest(network::http::HTTPReceive *receive)
{
	m_eventStreamHandler->addReceiverWithResponse(receive);
	fprintf(stdout, "Server recieved event connection from client\n");
}

//-------------------------------------------------------------------------------------------

void TestHTTPConnection::onEventTimer()
{
	network::http::EventStreamItem item;
	item.id() = m_eventStreamCounter;
	item.data() = QString("Event with count='%1'").arg(m_eventStreamCounter);
	QString n = item.data();
	fprintf(stdout, "onEventTimer - post\n%s\n", n.toUtf8().constData());
	m_eventStreamCounter++;
	m_eventStreamHandler->post(item);
}

//-------------------------------------------------------------------------------------------

void TestHTTPConnection::onStreamClientComplete()
{
	quit();
}

//-------------------------------------------------------------------------------------------

TEST(TestHTTPConnection,testEventStream)
{
	int argc = 0;
	TestHTTPConnection connectionApp(argc,0);
	connectionApp.exec();
	EXPECT_FALSE(connectionApp.isError());
}

//-------------------------------------------------------------------------------------------
