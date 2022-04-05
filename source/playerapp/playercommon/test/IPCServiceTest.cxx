#include "dlna/inc/DiskIF.h"
#include "playerapp/playercommon/test/IPCServiceTest.h"
#include "track/model/test/TrackDBTestEnviroment.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------
// IPCTestService
//-------------------------------------------------------------------------------------------

IPCTestService::IPCTestService(const char *serviceName, QObject *parent) : IPCService(serviceName, parent),
	m_isError(false),
	m_timeEventCounter(0)
{}

//-------------------------------------------------------------------------------------------

IPCTestService::~IPCTestService()
{}

//-------------------------------------------------------------------------------------------

void IPCTestService::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "IPCTestService::" << strR << " - " << strE << common::c_endl;
	m_isError = true;
}

//-------------------------------------------------------------------------------------------

bool IPCTestService::isError() const
{
	return m_isError;
}

//-------------------------------------------------------------------------------------------

int IPCTestService::timeEventCounter() const
{
	return m_timeEventCounter;
}

//-------------------------------------------------------------------------------------------

void IPCTestService::handleRPCJson(const QJsonDocument& doc)
{
	if(doc.isObject())
	{
		QJsonObject json = doc.object();
		QString funcName = json.value(OMEGA_IPC_FUNCTION_NAME).toString();
		if(!funcName.isEmpty())
		{
			// { "function": "onTime", "value": 12.345 }
			if(funcName == "onTime")
			{
				common::TimeStamp tS = json.value("timestamp").toDouble();
				common::Log::g_Log.print("A - onTime - %.8f", static_cast<tfloat64>(tS));
				m_timeEventCounter++;
			}
			else
			{
				QString err = QString("Unknown RPC function '%1'").arg(funcName);
				printError("handleRPCJson", err.toUtf8().constData());
			}
		}
		else
		{
			printError("handleRPCJson", "Expected function name not in JSON data");
		}
	}
	else
	{
		printError("handleRPCJson", "RPC JSON data is not an object");
	}
}

//-------------------------------------------------------------------------------------------
// IPCService_QtTestApplication
//-------------------------------------------------------------------------------------------

IPCService_QtTestApplication::IPCService_QtTestApplication(TestType testNo, const QString& pathExec, int argc, char **argv) : QCoreApplication(argc, argv),
	m_testNo(testNo),
	m_isError(false),
	m_pathToTestExec(pathExec)
{
	QTimer::singleShot(1, this, SLOT(onRunTest()));
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "IPCService_QtTestApplication::" << strR << " - " << strE << common::c_endl;
	m_isError = true;
}

//-------------------------------------------------------------------------------------------

bool IPCService_QtTestApplication::isError() const
{
	return m_isError;
}

//-------------------------------------------------------------------------------------------

QString IPCService_QtTestApplication::pathToTestProgramB()
{
	QString testProgName = "ipcservice_test_prog_b";
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

void IPCService_QtTestApplication::runProcessTest(IPCTestService *service)
{
	QString auxProgAPath = pathToTestProgramB();
	
	if(!auxProgAPath.isEmpty())
	{
		QProcess *processA = new QProcess(this);
		QStringList argsA;
	
		argsA.append(QString::number(static_cast<int>(m_testNo)));
		fprintf(stdout, "A - starting process B\n");
		processA->start(auxProgAPath, argsA);
		
		processEvents();
		while(processA->state() == QProcess::Starting)
		{
			QThread::msleep(50);
			processEvents();
			fprintf(stdout, "A(%d) - waiting on B to start\n", getpid());		
		}

		if(processA->state() == QProcess::Running)
		{
			if(m_testNo == e_handleServiceEventsOnlyWithNoResponse)
			{
				handleServiceEventsOnlyWithNoResponse(service);
			}
			else
			{
				printError("onRunTest", "Unknown unit test process");
			}
		}
		else
		{
			printError("runProcessTest", "A - process B is not running");
		}

		fprintf(stdout, "A - wait for B to finish\n");
		if(processA->waitForFinished(5000))
		{
			int res;
			fprintf(stdout, "A - process B has exited\n");
			res = processA->exitCode();
			if(res < 0)
			{
				QString err = QString("Process B exited with error code %1").arg(res);
				printError("runProcessTest", err.toUtf8().constData());
			}
		}
		else
		{
			do
			{
				fprintf(stdout, "A - killing B\n");
				processA->kill();
			} while(processA->waitForFinished());
			printError("runProcessTest", "Program didn't finish as expected");
		}
		delete processA;
	}
	else
	{
		printError("runProcessTest", "Failed to find path to test program");
	}
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::onRunTest()
{
	IPCTestService *service = new IPCTestService(IPC_SERVICE_TEST_NAME, this);

	if(service->start())
	{
		runProcessTest(service);
		
		service->stop();
		if(service->isError())
		{
			printError("onRunTest", "IPC Test failed with errors in service");
		}
	}
	else
	{
		printError("onRunTest", "Failed to start IPC service");
	}
	delete service;
	quit();
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::handleServiceEventsOnlyWithNoResponse(IPCTestService *service)
{
	while(service->timeEventCounter() < c_testIPCServiceCountLimit && !service->isError())
	{
		processEvents();
	}
	service->stop();
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------

using namespace orcus;

//-------------------------------------------------------------------------------------------

TEST(IPCService,handleServiceEventsOnlyWithNoResponse)
{
    QStringList libPaths = QApplication::libraryPaths();
    {
        char *argv = 0;
        track::model::TrackDBTestEnviroment *pTrackDBTest = track::model::TrackDBTestEnviroment::instance();
		IPCService_QtTestApplication tester(IPCService_QtTestApplication::e_handleServiceEventsOnlyWithNoResponse,
        	pTrackDBTest->execPath(), 0, &argv);
        tester.exec();
        ASSERT_FALSE(tester.isError());
    }
    QApplication::setLibraryPaths(libPaths);
}

//-------------------------------------------------------------------------------------------
