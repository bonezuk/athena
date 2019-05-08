//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_DAEMON_TEST_MUSICDAEMONTEST_H
#define __ORCUS_DAEMON_TEST_MUSICDAEMONTEST_H
//-------------------------------------------------------------------------------------------

#include "daemon/inc/MusicDaemon.h"

//-------------------------------------------------------------------------------------------

class MusicDaemonTest : public QCoreApplication
{
	public:
		Q_OBJECT
	public:
		MusicDaemonTest(int argc,char **argv);
		virtual ~MusicDaemonTest();

	private:
		int m_count;
		network::http::HTTPService *m_webClientService;
		
	private slots:
		void onStartTest();
		void onStopTest();
	
		void onHTTPClientTransaction(network::http::HTTPCTransaction *);
		void onHTTPClientTransactionChunk(network::http::HTTPCTransaction*,const QString&,network::NetArrayPtr);
		void onHTTPClientTransactionError(network::http::HTTPCTransaction*,const QString&);
		void onHTTPClientError(network::http::HTTPClient*,const QString&);
		void onHTTPClientComplete(network::http::HTTPClient*);
		void onStream(network::http::HTTPCTransaction *tran, int id, const QString& evt, const QString& data);
};

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

#include "daemon/test/integration/MusicDaemonTest.h"

//-------------------------------------------------------------------------------------------

MusicDaemonTest::MusicDaemonTest(int argc,char **argv) : QCoreApplication(argc,argv),
	m_count(0),
	m_webClientService(0)
{
	QObject::connect(this,SIGNAL(aboutToQuit()),this,SLOT(onStopTest()));
	QTimer::singleShot(100,this,SLOT(onStartTest()));
}

//-------------------------------------------------------------------------------------------

MusicDaemonTest::~MusicDaemonTest()
{}

//-------------------------------------------------------------------------------------------

void MusicDaemonTest::onStartTest()
{
	bool res = false;
	
	try
	{
		network::Resource& nResource = network::Resource::instance();
		network::Controller::ControllerSPtr ctrl(network::Controller::instance());
		
		if(ctrl.data()!=0)
		{
			m_webClientService = dynamic_cast<network::http::HTTPService *>(ctrl->newService("http_client"));
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

void MusicDaemonTest::onStopTest()
{
	network::Controller::ControllerSPtr ctrl(network::Controller::instance());
	if(m_webClientService!=0)
	{
		ctrl->deleteService(m_webClientService);
		m_webClientService = 0;
	}
	network::Controller::end();
}

//-------------------------------------------------------------------------------------------

void MusicDaemonTest::startTimeListenTest()
{
	QSharedPointer<HTTPClient> client = m_webClientService->getClient();

	QObject::connect(client.get(), SIGNAL(onTransaction(network::http::HTTPCTransaction*)), 
		this, SLOT(onHTTPClientTransaction(network::http::HTTPCTransaction *)));
	QObject::connect(client.get(), SIGNAL(onTransactionChunk(network::http::HTTPCTransaction*,const QString&,network::NetArrayPtr)), 
		this, SLOT(onHTTPClientTransactionChunk(network::http::HTTPCTransaction*,const QString&,network::NetArrayPtr)));
	QObject::connect(client.get(), SIGNAL(onTransactionError(network::http::HTTPCTransaction*,const QString&)), 
		this, SLOT(onHTTPClientTransactionError(network::http::HTTPCTransaction*,const QString&)));
	QObject::connect(client.get(), SIGNAL(onError(network::http::HTTPClient*,const QString&)), 
		this, SLOT(onHTTPClientError(network::http::HTTPClient*,const QString&)));
	QObject::connect(client.get(), SIGNAL(onComplete(network::http::HTTPClient*)), 
		this, SLOT(onHTTPClientComplete(network::http::HTTPClient*)));
	QObject::connect(client.get(), SIGNAL(onStream(network::http::HTTPCTransaction*,int,const QString&,const QString&)),
		this, SLOT(onStream(network::http::HTTPCTransaction*,int,const QString&,const QString&)));
		
	client->setHost("127.0.0.1", MUSIC_DAEMON_PORT);
	
	int id = client->newTransaction();
	network::http::HTTPCTransaction& tran = client->transaction();

	QVERIFY(!tran.isSteaming());
	tran->setStreaming(true);
	QVERIFY(tran->isStreaming());
	
	network::http::Unit req;
	req.request(network::http::e_Get, "/event/stream/test");
	
	client->run();
}

//-------------------------------------------------------------------------------------------

void MusicDaemonTest::onStream(network::http::HTTPCTransaction *tran, int id, const QString& evt, const QString& data)
{
	QString n;
	
	if(id >= 0)
	{
		n += "id=" + QString::number(id) + " ";
	}
	if(!evt.isEmpty())
	{
		n += "evt=" + evt + " ";
	}
	n += "data='" + data + "'";
	common::Log::g_Log << n.toUtf8().constData() << common::c_endl;
	m_count++;
	if(m_count > 20)
	{
		tran->setComplete(true);
	}
}

//-------------------------------------------------------------------------------------------

void MusicDaemonTest::onHTTPClientComplete(network::http::HTTPClient*)
{
	if(m_count > 20)
	{
		common::Log::g_Log << "HTTP Streaming server test complete" << common::c_endl;
	}
	else
	{
		common::Log::g_Log << "Unexpected onComplete from HTTPClient" << common::c_endl;
	}
	quit();
}

//-------------------------------------------------------------------------------------------

void MusicDaemonTest::onHTTPClientTransaction(network::http::HTTPCTransaction *)
{
	common::Log::g_Log << "Unexpected onTransaction from HTTPCTransaction" << common::c_endl;
	quit();
}

//-------------------------------------------------------------------------------------------

void MusicDaemonTest::onHTTPClientTransactionChunk(network::http::HTTPCTransaction*,const QString&,network::NetArrayPtr)
{
	common::Log::g_Log << "Unexpected onTransactionChunk from HTTPCTransaction" << common::c_endl;
	quit();
}

//-------------------------------------------------------------------------------------------

void MusicDaemonTest::onHTTPClientTransactionError(network::http::HTTPCTransaction*,const QString& err)
{
	common::Log::g_Log << "Unexpected onTransactionError from HTTPCTransaction - '"
	common::Log::g_Log << err << "'" << common::c_endl;
	quit();
}

//-------------------------------------------------------------------------------------------

void MusicDaemonTest::onHTTPClientError(network::http::HTTPClient*,const QString& err)
{
	common::Log::g_Log << "Unexpected onError from HTTPClient - '"
	common::Log::g_Log << err << "'" << common::c_endl;
	quit();
}

//-------------------------------------------------------------------------------------------

TEST(MusicDaemonTest,daemonConnectionTest)
{
	int argc = 0;
	MusicDaemonTest daemonConnectionApp(argc,0);
	daemonConnectionApp.exec();
}

//-------------------------------------------------------------------------------------------
