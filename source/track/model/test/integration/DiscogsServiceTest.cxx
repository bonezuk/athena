#include "network/inc/Resource.h"
#include "network/http/inc/Query.h"
#include "network/http/inc/Unit.h"
#include "common/inc/Log.h"
#include "track/model/test/integration/DiscogsServiceTest.h"

#include "gtest/gtest.h"

using namespace omega;

//-------------------------------------------------------------------------------------------

DiscogsServiceTest::DiscogsServiceTest(int& argc, char **argv) : QCoreApplication(argc, argv)
{
	QTimer::singleShot(10, this, SLOT(onRun()));
}

//-------------------------------------------------------------------------------------------

DiscogsServiceTest::~DiscogsServiceTest()
{}

//-------------------------------------------------------------------------------------------

void DiscogsServiceTest::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "DiscogsServiceTest::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void DiscogsServiceTest::onRun()
{
	QSslSocket *socket = new QSslSocket(this);
	socket->connectToHostEncrypted("www.blackomega.co.uk", 443);
	if(socket->waitForEncrypted())
	{
		network::http::Unit req;
		req.request(network::http::Unit::e_Get);
		req.add("Host", "www.blackomega.co.uk");
		req.add("User-Agent", "Black Omega test");
		req.add("Connection", "close");
		
		QString reqTxt;
		req.print(reqTxt);
		QByteArray reqArray = reqTxt.toUtf8();
		socket->write(reqArray.constData(), reqArray.size());
		
		while(socket->isOpen() && socket->waitForReadyRead(30000))
		{
			QByteArray arr = socket->readAll();
			QString respTxt = QString::fromUtf8(arr);
			fprintf(stdout, "%s", respTxt.toUtf8().constData());
		}
		socket->close();
	}
	else
	{
		QString err = socket->errorString();
		printError("onRun", err.toUtf8().constData());
	}
	delete socket;
}

//-------------------------------------------------------------------------------------------

TEST(DiscogsServiceTest, runTest)
{
	int argc = 0;
	DiscogsServiceTest serviceApp(argc, 0);
	serviceApp.exec();
}

//-------------------------------------------------------------------------------------------
