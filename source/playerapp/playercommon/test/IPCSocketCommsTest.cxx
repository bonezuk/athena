//-------------------------------------------------------------------------------------------
// Test program
//-------------------------------------------------------------------------------------------
#if defined(IPCSOCKETCOMMS_TEST_PROG_A)
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCSocketComms.h"

//-------------------------------------------------------------------------------------------

void sendAndReceiveOnUNIXDomainSocket_TestProcess(const char *socketPath)
{
	IPCSocketComms comm(e_Client);
	
	fprintf(stdout, "B - open client to '%s'\n", socketPath);
	if(comm.open(QString::fromUtf8(socketPath)))
	{
		for(int i = 0; i < 100; i++)
		{
			int res;
			QString testStr = QString("test no=%1").arg(i);
			QByteArray txArr, rxArr;
			
			txArr = testStr.toUtf8();
			fprintf(stdout, "B - send: %s\n", txArr.constData());
			res = comm.write(txArr);
			if(res == txArr.size())
			{
				fprintf(stdout, "B - ready to receive\n");
				res = comm.read(rxArr);
				if(res > 0)
				{
					fprintf(stdout, "B - receive: %s\n");
				}
				else
				{
					fprintf(stdout, "B - failed to read data from comms\n");
				}
			}
			else
			{
				fprintf(stdout, "B - failed to write data to comms\n");
			}
		}
		fprintf("B - closing client\n");
		comm.close();
	}
	else
	{
		fprintf(stdout, "B - failed to open client connection to '%s'\n", socketPath);
	}
}

//-------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	fprintf(stdout, "B - process started\n");
	if(argc < 2)
	{
		fprintf(stdout, "B - no path to socket defined\n");
	}
	sendAndReceiveOnUNIXDomainSocket_TestProcess()
	fprintf("B - process complete\n");
	return 0;
}

// TODO - Add to CMAKE test program - ipcsocketcomms_test_prog_a

//-------------------------------------------------------------------------------------------
#else
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
// IPCSocketComms_QtTestClass
//-------------------------------------------------------------------------------------------

IPCSocketComms_QtTestClass::IPCSocketComms_QtTestClass(const QString& pathExec, const QString& socketPath, QObject *parent) : QObject(parent),
	m_pathToTestExec(pathExec),
	m_socketPath(socketPath)
{}

//-------------------------------------------------------------------------------------------

QString IPCSocketComms_QtTestClass::pathToTestProgramA()
{
	QString testProgName = "ipcsocketcomms_test_prog_a";
	QFileInfo appFile(m_pathToTestExec);
	QString progPath = dlna::DiskIF::mergeName(appFile.absolutePath(), testProgName);
	if(!DiskOps::exist(progPath))
	{
		fprintf(stdout, "Cannot find test exec '%s'\n", progPath.toUtf8().constData());
		fprintf(stdout, "Check the build system to ensure test program is compiled and in place\n");
		progPath = "";
	}
	return progPath;
}

//-------------------------------------------------------------------------------------------

void IPCSocketComms_QtTestClass::sendAndReceiveFromClient()
{
	QString auxProgAPath = pathToTestProgramA();
	QVERIFY(!auxProgAPath.isEmpty())
	
	IPCSocketComms serverComm(e_Server);
	bool res = false;
	
	fprintf(stdout, "A - open server to '%s'\n");
	if(serverComm.open(m_socketPath))
	{
		QProcess *processA = new QProcess(this);
		QStringList argsA;
		
		argsA.push(m_socketPath);
		fprintf(stdout, "A - starting process B\n");
		processA->start(auxProgAPath, argsA);
		
		while(processA->state() == QProcess::Starting)
		{
			QThread::msleep(50);
			fprintf(stdout, "A - waiting on B to start\n");
		}
		
		if(processA->state() == QProcess::Running)
		{
			int i = 0;
			
			fprintf(stdout, "A - process B started\n");
			while(i < 100)
			{
				int r;
				QByteArray rxArr, txArr;
				
				r = serverComm.read(rxArr);
				if(r > 0)
				{
					QString testStr = QString::fromUtf8(rxArr);
					fprintf(stdout, "A - receive: %s\n", testStr.toUtf8().constData());
					txArr = testStr.toUtf8();
					if(serverComm.write(txArr) == txArr.size())
					{
						fprintf(stdout, "A - send: %s\n", txArr.constData());
						i++;
					}
					else
					{
						fprintf(stdout, "A - error writing data to comms\n");
						break;
					}
				}
				else if(!r)
				{
					fprintf(stdout, "A - waiting to receive data from B.\n");
				}
				else
				{
					fprintf(stdout, "A - error reading data from comms");
					break;
				}
			}
		}
		else
		{
			fprintf(stdout, "A - process B is not running\n");
		}
		
		if(!processA->waitForFinished())
		{
			fprintf(stdout, "A - process B has exited\n");
			if(i == 100)
			{
				fprintf(stdout, "A - all expected messages from B received\n");
				res = true;
			}
			else
			{
				fprintf(stdout, "A - error in send and receive of messages from B\n")
			}
		}
		else
		{
			do
			{
				fprintf(stdout, "A - killing B\n");
				processA->kill();
			} while(processA->waitForFinished());
		}
		delete processA;
		
		serverComm.close();
	}
	else
	{
		fprintf(stdout, "A - failed to open server connection to '%s'\n", m_socketPath);
	}
	
	QVERIFY(res);
}

//-------------------------------------------------------------------------------------------
// IPCSocketComms_QtTestApplication
//-------------------------------------------------------------------------------------------

IPCSocketComms_QtTestApplication(const QString& pathExec, const QString& socketPath, int argc,char **argv) : QCoreApplication(argc,argv),
	m_succeeded(false),
	m_pathToTestExec(pathExec),
	m_socketPath(socketPath)
{
	QTimer::singleShot(10,this,SLOT(runTests()));
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms_QtTestApplication::testSucceeded() const
{
	return m_succeeded;
}

//-------------------------------------------------------------------------------------------

void IPCSocketComms_QtTestApplication::runTests()
{
	IPCSocketComms_QtTestClass tests(m_pathToTestExec, m_socketPath);
	m_succeeded = (QTest::qExec(&tests)==0) ? true : false;
	quit();
}

//-------------------------------------------------------------------------------------------

TEST(IPCSocketComms,RunQtUnitTests)
{
    QStringList libPaths = QApplication::libraryPaths();
    {
        char *argv = 0;
		track::model::TrackDBTestEnviroment *pTrackDBTest = track::model::TrackDBTestEnviroment::instance();
		QString socketPath = dlna::DiskIF::mergeName(pTrackDBTest->getTempDirectory(), "test_socket_A.usock");
        IPCSocketComms_QtTestApplication tester(pTrackDBTest->execPath(), socketPath, 0, &argv);
        tester.exec();
        ASSERT_TRUE(tester.testSucceeded());
    }
    QApplication::setLibraryPaths(libPaths);
}


//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
