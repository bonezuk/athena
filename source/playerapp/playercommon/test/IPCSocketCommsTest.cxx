#include "dlna/inc/DiskIF.h"
#include "playerapp/playercommon/test/IPCSocketCommsTest.h"
#include "track/model/test/TrackDBTestEnviroment.h"

using namespace orcus;

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
		
		while(processA->state() == QProcess::Starting)
		{
			QThread::msleep(50);
			fprintf(stdout, "A - waiting on B to start\n");
		}
		
		if(processA->state() == QProcess::Running)
		{
			fprintf(stdout, "A - process B started\n");

			i = 0;
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
		delete processA;
		
		serverComm.close();
	}
	else
	{
		fprintf(stdout, "A - failed to open server connection to '%s'\n", m_socketPath.toUtf8().constData());
	}
	
	QVERIFY(res);
}

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
