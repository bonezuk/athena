#include "network/http/test/TestHTTPConnectionTest.h"

#include "gtest/gtest.h"

//-------------------------------------------------------------------------------------------

TestHTTPClientThread::TestHTTPClientThread(int unitTestCase, QObject *parent) : QThread(parent),
	m_unitTestCase(unitTestCase),
	m_isError(false),
	m_streamCounter(0),
	m_webClientService(0),
	m_pClient(),
	m_idList(),
	m_idTransSet(),
	m_idReplySet()
{}

//-------------------------------------------------------------------------------------------

TestHTTPClientThread::~TestHTTPClientThread()
{}

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
	
	network::http::HTTPCTransaction& tran = client->newTransaction();

	Q_ASSERT(!tran.isStreaming());
	tran.setStreaming(true);
	Q_ASSERT(tran.isStreaming());
	
	network::http::Unit req;
	req.request(network::http::Unit::e_Get, "/test/event-stream");
	tran.request() = req;
	
	client->enqueueTransaction(tran.id());
	
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
		m_streamCounter++;
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
	exit();
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::populateIdList()
{
	m_idList.clear();
	m_idTransSet.clear();
	m_idReplySet.clear();
	
	for(int id = 0; id < c_numberOfPersistentIds; id++)
	{
		m_idList.append(id);
	}
}

//-------------------------------------------------------------------------------------------

bool TestHTTPClientThread::checkReplyIdSet()
{
	int id;
	QSet<int> missing;
	QSet<int>::iterator ppI;

	for(id = 0; id < c_numberOfPersistentIds; id++)
	{
		ppI = m_idReplySet.find(id);
		if(ppI == m_idReplySet.end())
		{
			missing.insert(id);
		}
	}
	if(!missing.isEmpty())
	{
		fprintf(stdout, "Missing transaction ids : ");
		for(ppI = missing.begin(); ppI != missing.end(); ppI++)
		{
			fprintf(stdout, "%d, ", *ppI);
		}
		fprintf(stdout,"\n");
	}
	return missing.isEmpty();
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::queueNextQuery()
{
	if(!m_idList.isEmpty())
	{
		network::http::HTTPCTransaction& tran = m_pClient->newTransaction();
	
		int id = m_idList.takeFirst();
		network::http::Unit req;
		req.request(network::http::Unit::e_Get, "/test/persistent");
		req.query().add("id", QString::number(id));
		tran.request() = req;
		
		m_idTransSet.insert(id);
		m_pClient->enqueueTransaction(tran.id());
	}
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::startPersistentTest()
{
	m_pClient = m_webClientService->getClient();
	
	m_pClient->setHost("127.0.0.1", TEST_DAEMON_PORT);

	QObject::connect(m_pClient.data(), SIGNAL(onTransactionError(network::http::HTTPCTransaction*,const QString&)),
		this, SLOT(onTransactionError(network::http::HTTPCTransaction*,const QString&)));
	QObject::connect(m_pClient.data(), SIGNAL(onError(network::http::HTTPClient*,const QString&)),
		this, SLOT(onError(network::http::HTTPClient*,const QString&)));
	QObject::connect(m_pClient.data(), SIGNAL(onComplete(network::http::HTTPClient*)),
		this, SLOT(onPersistentComplete(network::http::HTTPClient*)));
	QObject::connect(m_pClient.data(), SIGNAL(onTransaction(network::http::HTTPCTransaction*)),
		this, SLOT(onPersistentTransaction(network::http::HTTPCTransaction*)));

	populateIdList();
	queueNextQuery();
	queueNextQuery();
	queueNextQuery();
	
	m_pClient->run();
	
	m_timer = new QTimer();
	m_timer->setInterval(100);
	m_timer->setSingleShot(false);
	m_timer->start();
	QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(onPersistentTimer()));
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::onPersistentTimer()
{
	if(!m_idList.isEmpty())
	{
		queueNextQuery();
	}
	else if(m_idTransSet.isEmpty())
	{
		if(isRunning())
		{
			m_isError = checkReplyIdSet();
			m_pClient->shutdown();
			QThread::msleep(1000);
			quit();
			wait();

			if(m_webClientService != 0)
			{
				network::Controller::ControllerSPtr ctrl(network::Controller::instance());
				ctrl->deleteService(m_webClientService);
				m_webClientService = 0;
			}

			m_timer->stop();
			delete m_timer;
			m_timer = 0;
		}
	}
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::onPersistentTransaction(network::http::HTTPCTransaction *pTrans)
{
	if(pTrans->response().response() == 200)
	{
		if(pTrans->responseData().GetSize() > 0)
		{
			int id;
			QString idStr = QString::fromUtf8(reinterpret_cast<const char *>(pTrans->responseData().GetData()), pTrans->responseData().GetSize());
			bool res = false;
			
			id = idStr.toInt(&res);
			if(res)
			{
				QSet<int>::iterator ppI;
				
				ppI = m_idTransSet.find(id);
				if(ppI != m_idTransSet.end())
				{
					fprintf(stdout, "Client reply id = %d\n", id);
					m_idTransSet.erase(ppI);
					m_idReplySet.insert(id);
				}
				else
				{
					QString err = "Id " + QString::number(id) + " not in expected transaction list";
					printError("onPersistentTransaction", err.toUtf8().constData());
					exit();
				}
			}
			else
			{
				QString err = "Failed to get id number from body: '" + idStr + "'";
				printError("onPersistentTransaction", err.toUtf8().constData());
				exit();
			}
		}
		else
		{
			printError("onPersistentTransaction", "Error no body in response");
			exit();
		}
	}
	else
	{
		printError("onPersistentTransaction", "Error in transaction response");
		exit();
	}
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::onPersistentComplete(network::http::HTTPClient *client)
{
	fprintf(stdout, "HTTP Client complete");
}

//-------------------------------------------------------------------------------------------

void TestHTTPClientThread::run()
{
	if(startHTTPClient())
	{
		if(m_unitTestCase == 0)
		{
			startEventStream();
		}
		else
		{
			startPersistentTest();
		}
		exec();
	}
	else
	{
		printError("run", "Failed to start HTTP client service");
	}
}

//-------------------------------------------------------------------------------------------

TestHTTPConnection::TestHTTPConnection(int unitTestCase, int argc, char **argv) : QCoreApplication(argc, argv),
	m_unitTestCase(unitTestCase),
	m_isError(false),
	m_webService(0),
	m_webServer(0),
	m_eventStreamHandler(0),
	m_eventStreamCounter(0),
	m_client(0),
	m_isClientComplete(false),
	m_isServerComplete(false)
{
	if(m_unitTestCase == 0)
	{
		m_eventStreamHandler = new network::http::EventStreamHandler(this);
		m_eventStreamTimer = new QTimer(this);
		m_eventStreamTimer->setInterval(100);
		m_eventStreamTimer->setSingleShot(false);
		m_eventStreamTimer->start();
		QObject::connect(m_eventStreamTimer, SIGNAL(timeout()), this, SLOT(onEventTimer()));
	}
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
	return (m_isError || m_client==0 || m_client->isError());
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
					m_webServer->connect("/test/persistent",this,SLOT(onPersistentConnectionTest(network::http::HTTPReceive *)));
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
		m_client = new TestHTTPClientThread(m_unitTestCase);
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
// /test/persistent?id=3
//-------------------------------------------------------------------------------------------

void TestHTTPConnection::onPersistentConnectionTest(network::http::HTTPReceive *receive)
{
	QString idStr;
	network::http::Unit hdr;
	
	idStr = receive->header().query().data("id");
	if(!idStr.isEmpty())
	{
		QByteArray arr(idStr.toUtf8());
		network::NetArraySPtr body(new network::NetArray);
		body->AppendRaw(arr.constData(), arr.length());
		
		hdr.response(200);
		hdr.add("Content-Type", "text/plain");
		hdr.add("Content-Length", QString::number(arr.length()));
		
		receive->connection()->postResponse(hdr);
		receive->connection()->postBody(body);
		
		fprintf(stdout, "Server recieve persistent id=%s\n", arr.constData());
	}
	else
	{
		hdr.response(400);
		receive->connection()->postResponse(hdr);
	}
	receive->connection()->complete();
}

//-------------------------------------------------------------------------------------------

void TestHTTPConnection::onEventStreamTest(network::http::HTTPReceive *receive)
{
	QObject::connect(receive, SIGNAL(onComplete(network::http::HTTPReceive *)), this, SLOT(onStreamServerComplete()));
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
	fprintf(stdout, "Client HTTP thread complete\n");
	quit();
}

//-------------------------------------------------------------------------------------------

void TestHTTPConnection::onStreamServerComplete()
{
	fprintf(stdout, "Server HTTP event connection complete\n");
	m_isServerComplete = true;
	if(m_isServerComplete && m_isClientComplete)
	{
		quit();
	}
}

//-------------------------------------------------------------------------------------------

TEST(TestHTTPConnection,testEventStream)
{
	int argc = 0;
	TestHTTPConnection connectionApp(0,argc,0);
	connectionApp.exec();
	EXPECT_FALSE(connectionApp.isError());
}

//-------------------------------------------------------------------------------------------

TEST(TestHTTPConnection,testPersistentConnection)
{
	int argc = 0;
	TestHTTPConnection connectionApp(1,argc,0);
	connectionApp.exec();
	EXPECT_FALSE(connectionApp.isError());
}

//-------------------------------------------------------------------------------------------
