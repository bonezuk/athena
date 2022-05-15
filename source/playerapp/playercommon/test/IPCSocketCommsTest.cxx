#include "dlna/inc/DiskIF.h"
#include "playerapp/playercommon/test/IPCSocketCommsTest.h"
#include "track/model/test/TrackDBTestEnviroment.h"

using namespace omega;

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
	if(!common::DiskOps::exist(progPath))
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
	int i = 0;
	QString auxProgAPath = pathToTestProgramA();
	QVERIFY(!auxProgAPath.isEmpty());
	
	QCoreApplication *app = dynamic_cast<QCoreApplication *>(parent());
	QVERIFY(app != 0);
	
	IPCSocketComms serverComm(IPCSocketComms::e_Server);
	bool res = false;
	
	fprintf(stdout, "A - open server to '%s'\n", m_socketPath.toUtf8().constData());
	if(serverComm.open(m_socketPath))
	{
		QProcess *processA = new QProcess(this);
		QStringList argsA;
		
		argsA.append(m_socketPath);
		fprintf(stdout, "A - starting process B\n");
		processA->start(auxProgAPath, argsA);
		
		app->processEvents();
		while(processA->state() == QProcess::Starting)
		{
			QThread::msleep(50);
			app->processEvents();
			fprintf(stdout, "A(%d) - waiting on B to start\n", getpid());
		}
		
		if(processA->state() == QProcess::Running)
		{
			int timeoutCount = 0;

			fprintf(stdout, "A(%d) - process B started\n", getpid());
			i = 0;
			while(i < 10000 & timeoutCount < 3)
			{
				int r;
				QByteArray rxArr, txArr;
				
				r = serverComm.read(rxArr);
				if(r > 0)
				{
					QString testStr = QString::fromUtf8(rxArr);
					fprintf(stdout, "A(%d) - receive: %s\n", getpid(), testStr.toUtf8().constData());
					txArr = testStr.toUtf8();
					if(serverComm.write(txArr) == txArr.size())
					{
						fprintf(stdout, "A(%d) - send: %s\n", getpid(), txArr.constData());
						i++;
					}
					else
					{
						fprintf(stdout, "A(%d) - error writing data to comms\n", getpid());
						break;
					}
				}
				else if(!r)
				{
					fprintf(stdout, "A(%d) - waiting to receive data from B.\n", getpid());
					timeoutCount++;
					QThread::msleep(50);
				}
				else
				{
					fprintf(stdout, "A(%d) - error reading data from comms", getpid());
					break;
				}

				if(!(i % 100))
				{
					app->processEvents();
				}
			}
		}
		else
		{
			fprintf(stdout, "A - process B is not running\n");
		}
		
		fprintf(stdout, "A - wait for B to finish\n");
		if(processA->waitForFinished(1000))
		{
			fprintf(stdout, "A - process B has exited\n");
			if(i == 10000)
			{
				fprintf(stdout, "A - all expected messages from B received\n");
				res = true;
			}
			else
			{
				fprintf(stdout, "A - error in send and receive of messages from B\n");
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

		{
			QString stdA = QString::fromUtf8(processA->readAllStandardOutput());
			fprintf(stdout,"B Process final output\n%s", stdA.toUtf8().constData());
		}
		delete processA;
		
		serverComm.close();
	}
	else
	{
		fprintf(stdout, "A - failed to open server connection to '%s'\n", m_socketPath.toUtf8().constData());
	}
	
	QVERIFY(res);
}


/*
void IPCSocketComms_QtTestClass::sendAndReceiveFromClient()
{
	int i = 0;
	IPCSocketComms serverComm(IPCSocketComms::e_Server);
	bool res = false;
	
	fprintf(stdout, "A - open server to '%s'\n", m_socketPath.toUtf8().constData());
	if(serverComm.open(m_socketPath))
	{
		int timeoutCount = 0;

		fprintf(stdout, "A(%d) - process B started\n", getpid());
		i = 0;
		while(i < 1000000000 & timeoutCount < 30)
		{
			int r;
			QByteArray rxArr, txArr;
			
			r = serverComm.read(rxArr);
			if(r > 0)
			{
				QString testStr = QString::fromUtf8(rxArr);
				fprintf(stdout, "A(%d) - receive: %s\n", getpid(), testStr.toUtf8().constData());
				txArr = testStr.toUtf8();
				if(serverComm.write(txArr) == txArr.size())
				{
					fprintf(stdout, "A(%d) - send: %s\n", getpid(), txArr.constData());
					i++;
				}
				else
				{
					fprintf(stdout, "A(%d) - error writing data to comms\n", getpid());
					break;
				}
			}
			else if(!r)
			{
				fprintf(stdout, "A(%d) - waiting to receive data from B.\n", getpid());
				timeoutCount++;
			}
			else
			{
				fprintf(stdout, "A(%d) - error reading data from comms", getpid());
				break;
			}
		}
		serverComm.close();
	}
	else
	{
		fprintf(stdout, "A - failed to open server connection to '%s'\n", m_socketPath.toUtf8().constData());
	}
	
	res = true;
	QVERIFY(res);
}
*/

//-------------------------------------------------------------------------------------------
// IPCSocketComms_QtTestApplication
//-------------------------------------------------------------------------------------------

IPCSocketComms_QtTestApplication::IPCSocketComms_QtTestApplication(const QString& pathExec, const QString& socketPath, int argc,char **argv) : QCoreApplication(argc,argv),
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
	IPCSocketComms_QtTestClass tests(m_pathToTestExec, m_socketPath, this);
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
