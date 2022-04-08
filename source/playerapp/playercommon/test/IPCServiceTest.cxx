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
	common::Log::g_Log << "A - handle -  " << QString::fromUtf8(doc.toJson(QJsonDocument::Compact)) << common::c_endl;

	if(doc.isObject())
	{
		QJsonObject json = doc.object();
		QString funcName = json.value(OMEGA_IPC_FUNCTION_NAME).toString();
		if(!funcName.isEmpty())
		{
			// get - { "function": "onTime", "value": 12.345 }
			// no response
			if(funcName == "onTime")
			{
				common::TimeStamp tS = json.value("timestamp").toDouble();
				common::Log::g_Log.print("A - onTime - %.8f", static_cast<tfloat64>(tS));
				m_timeEventCounter++;
			}
			// get - { "function": "onResponse", "value": 12.345 }
			// return - { "count: 123, "value": 12.345 }
			else if(funcName == "onResponse")
			{
				m_timeEventCounter++;
				tfloat64 v = json.value("value").toDouble();
				int count = m_timeEventCounter;
				QVariantMap sMap;
				QJsonDocument respDoc;
				sMap.insert("count", QVariant(count));
				sMap.insert("value", QVariant(v));
				common::Log::g_Log.print("A - onResponse - count=%d, value=%.8f\n", count, v);
				respDoc.setObject(QJsonObject::fromVariantMap(sMap));
				QByteArray rArr = respDoc.toJson(QJsonDocument::Compact);
				m_pServiceThread->postResponse(rArr);
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
// IPCProgBInterface
//-------------------------------------------------------------------------------------------

IPCProgBInterface::IPCProgBInterface() : IPCInterfaceBase(IPC_SERVICE_PROGB_NAME),
	m_isError(false)
{}

//-------------------------------------------------------------------------------------------

IPCProgBInterface::~IPCProgBInterface()
{}

//-------------------------------------------------------------------------------------------

void IPCProgBInterface::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "IPCProgBInterface::" << strR << " - " << strE << common::c_endl;
	m_isError = true;
}

//-------------------------------------------------------------------------------------------

bool IPCProgBInterface::isError() const
{
	return m_isError;
}

//-------------------------------------------------------------------------------------------

void IPCProgBInterface::onClientTime(tfloat64 val)
{
	QVariantMap rpcMap;
	common::TimeStamp tStamp(val);
	
	common::Log::g_Log.print("A - onClientTime - timestamp=%.8f\n", static_cast<tfloat64>(tStamp));
	rpcMap.insert("timestamp", QVariant(static_cast<tfloat64>(tStamp)));
	if(!sendRPCCall("onClientTime", rpcMap))
	{
		printError("onClientTime", "Failed to send RPC call");
	}
}

//-------------------------------------------------------------------------------------------

tfloat64 IPCProgBInterface::onClientResponse(tfloat64 val, int& count)
{
	tfloat64 res = 0.0;
	QVariantMap rpcMap;
	
	common::Log::g_Log.print("A - onClientResponse - value=%.8f\n", val);
	rpcMap.insert("value", QVariant(val));
	if(sendRPCCall("onClientResponse", rpcMap))
	{
		QJsonDocument doc = receiveJsonReply();
		if(doc.isObject())
		{
			common::Log::g_Log << "A - onClientResponse - " << QString::fromUtf8(doc.toJson(QJsonDocument::Compact)) << common::c_endl;
		
			QJsonValue countJ = doc.object().value("count");
			QJsonValue valueJ = doc.object().value("value");
			if(countJ.isDouble() && valueJ.isDouble())
			{
				count = countJ.toInt();
				res = valueJ.toDouble();
			}
			else
			{
				QString err = QString("JSON unexpected '%1'").arg(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
				printError("onClientResponse", err.toUtf8().constData());
			}
		}
		else
		{
			printError("onClientResponse", "Unexpected JSON response");
		}
	}
	else
	{
		printError("onClientResponse", "Failed to send RPC call");
	}
	return res;
}

//-------------------------------------------------------------------------------------------
// IPCService_QtTestApplication
//-------------------------------------------------------------------------------------------

IPCService_QtTestApplication::IPCService_QtTestApplication(TestType testNo, const QString& pathExec, int argc, char **argv) : QCoreApplication(argc, argv),
	m_testNo(testNo),
	m_isError(false),
	m_pathToTestExec(pathExec),
	m_pInterface(0),
	m_testCount(0)
{
	QTimer::singleShot(1, this, SLOT(onRunTest()));
}

//-------------------------------------------------------------------------------------------

IPCService_QtTestApplication::~IPCService_QtTestApplication()
{}

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

bool IPCService_QtTestApplication::isTestService()
{
	bool isService;
	
	switch(m_testNo)
	{
		case e_handleServiceEventsOnlyWithNoResponse:
		case e_handleServiceEventsOnlyWithResponse:
		case e_sendAndReceiveEventsWithNoResponse:
		case e_sendAndReceiveEventsWithResponse:
			isService = true;
			break;
		case e_sendEventsToClientWithNoResponse:
		case e_sendEventsToClientWithResponse:
			isService = false;
			break;
		default:
			isService = false;
			break;
	}
	return isService;
}

//-------------------------------------------------------------------------------------------

bool IPCService_QtTestApplication::isClientService()
{
	bool isClient;
	
	switch(m_testNo)
	{
		case e_handleServiceEventsOnlyWithNoResponse:
		case e_handleServiceEventsOnlyWithResponse:
			isClient = false;
			break;
		case e_sendEventsToClientWithNoResponse:
		case e_sendEventsToClientWithResponse:
		case e_sendAndReceiveEventsWithNoResponse:
		case e_sendAndReceiveEventsWithResponse:
			isClient = true;
			break;
		default:
			isClient = false;
			break;
	}
	return isClient;
}

//-------------------------------------------------------------------------------------------

bool IPCService_QtTestApplication::startProgBInterface()
{
	common::TimeStamp timeoutTS = common::TimeStamp::now() + 5.0;
	bool res = true;
	
	if(isClientService())
	{
		QString iPath = pathToUNIXSocket(IPC_SERVICE_PROGB_NAME);
		
		while(!common::DiskOps::exist(iPath) && res)
		{
			if(timeoutTS < common::TimeStamp::now())
			{
				QString err = QString("Timeout waiting for creation of socket at '%1'").arg(iPath);
				printError("startProgBInterface", err.toUtf8().constData());
				res = false;
			}
		}
		if(res)
		{
			m_pInterface = new IPCProgBInterface();
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::stopProgBInterface()
{
	if(m_pInterface != 0)
	{
		delete m_pInterface;
		m_pInterface = 0;
	}
}

//-------------------------------------------------------------------------------------------

bool IPCService_QtTestApplication::startProgBProcess(QProcess **pProcessA)
{
	track::model::TrackDBTestEnviroment *pTrackDBTest = track::model::TrackDBTestEnviroment::instance();
	bool res = false;
	
	if(pTrackDBTest->spawnICSProcess())
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
				*pProcessA = processA;
				res = true;
			}
			else
			{
				printError("startProgBProcess", "A - process B is not running");
				delete processA;
			}
		}
		else
		{
			printError("runProcessTest", "Failed to find path to test program");
		}
	}
	else
	{
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::stopProgBProcess(QProcess *processA)
{
	track::model::TrackDBTestEnviroment *pTrackDBTest = track::model::TrackDBTestEnviroment::instance();
	
	if(pTrackDBTest->spawnICSProcess())
	{
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
			printError("stopProgBProcess", "Program didn't finish as expected");
		}
		delete processA;
	}
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::runProcessTest(IPCTestService *service)
{
	QProcess *processA = 0;

	if(startProgBProcess(&processA))
	{
		if(startProgBInterface())
		{
			if(m_testNo == e_handleServiceEventsOnlyWithNoResponse)
			{
				handleServiceEventsOnlyWithNoResponse(service);
			}
			else if(m_testNo == e_sendEventsToClientWithNoResponse)
			{
				sendEventsToClientWithNoResponse();
			}
			else if(m_testNo == e_handleServiceEventsOnlyWithResponse)
			{
				handleServiceEventsOnlyWithResponse(service);
			}
			else if(m_testNo == e_sendEventsToClientWithResponse)
			{
				sendEventsToClientWithResponse();
			}
			else if(m_testNo == e_sendAndReceiveEventsWithNoResponse)
			{
				sendAndReceiveEventsWithNoResponse(service);
			}
			else if(m_testNo == e_sendAndReceiveEventsWithResponse)
			{
				sendAndReceiveEventsWithResponse(service);
			}
			else
			{
				printError("runProcessTest", "Unknown unit test process");
			}
			stopProgBInterface();
		}
		else
		{
			printError("runProcessTest", "Failed to get test interface");
		}
		stopProgBProcess(processA);
	}
	else
	{
		printError("runProcessTest", "Failed to start test program");
	}
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::onRunTest()
{
	if(isTestService())
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
	}
	else
	{
		runProcessTest(0);
	}
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

void IPCService_QtTestApplication::onSendClientTimeEvent()
{
	if(m_pInterface != 0 && !m_pInterface->isError())
	{
		common::TimeStamp tS = common::TimeStamp::now();
		common::Log::g_Log.print("A - onClientTime - %.8f", static_cast<tfloat64>(tS));
		m_pInterface->onClientTime(static_cast<tfloat64>(tS));
		m_testCount++;
	}
	else
	{
		printError("onSendClientTimeEvent", "Invalid interface state");
	}
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::sendEventsToClientWithNoResponse()
{
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onSendClientTimeEvent()));
	timer->setInterval(1);
	timer->setSingleShot(false);
	timer->start();
	
	while(m_testCount < c_testIPCServiceCountLimit && !isError() && !m_pInterface->isError())
	{
		processEvents();
	}

	delete timer;
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::handleServiceEventsOnlyWithResponse(IPCTestService *service)
{
	while(service->timeEventCounter() < c_testIPCServiceCountLimit && !service->isError())
	{
		processEvents();
	}
	service->stop();
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::onSendClientResponseEvent()
{
	if(m_pInterface != 0 && !m_pInterface->isError())
	{
		int count;
		tfloat64 val, res;
		common::TimeStamp tS = common::TimeStamp::now();
		
		count = 0;
		val = static_cast<tfloat64>(tS);
		res = m_pInterface->onClientResponse(val, count);
		if(isEqual(val, res))
		{
			common::Log::g_Log.print("A - onClientResponse: val=%.8f, res=%.8f, count=%d\n", val, res, count);
			m_testCount = count;
		}
		else
		{
			QString err = QString("Unexpected difference between value=%1 and result=%2").arg(val).arg(res);
			printError("onSendClientResponseEvent", err.toUtf8().constData());
		}
	}
	else
	{
		printError("onSendClientResponseEvent", "Invalid interface state");
	}
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::sendEventsToClientWithResponse()
{
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onSendClientResponseEvent()));
	timer->setInterval(1);
	timer->setSingleShot(false);
	timer->start();
	
	while(m_testCount < c_testIPCServiceCountLimit && !isError() && !m_pInterface->isError())
	{
		processEvents();
	}

	delete timer;
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::sendAndReceiveEventsWithNoResponse(IPCTestService *service)
{
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onSendClientTimeEvent()));
	timer->setInterval(1);
	timer->setSingleShot(false);
	timer->start();
	
	while((m_testCount < c_testIPCServiceCountLimit || service->timeEventCounter() < c_testIPCServiceCountLimit) && 
		!isError() && !m_pInterface->isError())
	{
		processEvents();
	}

	delete timer;
}

//-------------------------------------------------------------------------------------------

void IPCService_QtTestApplication::sendAndReceiveEventsWithResponse(IPCTestService *service)
{
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onSendClientResponseEvent()));
	timer->setInterval(1);
	timer->setSingleShot(false);
	timer->start();
	
	while((m_testCount < c_testIPCServiceCountLimit || service->timeEventCounter() < c_testIPCServiceCountLimit) && 
		!isError() && !m_pInterface->isError())
	{
		processEvents();
	}

	delete timer;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------

using namespace orcus;

//-------------------------------------------------------------------------------------------

void clearIPCServiceTestEnv()
{
	QString path;
	track::model::TrackDBTestEnviroment *pTrackDBTest = track::model::TrackDBTestEnviroment::instance();

	path = pathToUNIXSocket(IPC_SERVICE_TEST_NAME);
	if(common::DiskOps::exist(path))
	{
		common::DiskOps::remove(path);
	}	
	if(pTrackDBTest->spawnICSProcess())
	{
		path = pathToUNIXSocket(IPC_SERVICE_PROGB_NAME);
		if(common::DiskOps::exist(path))
		{
			common::DiskOps::remove(path);
		}
	}
}

//-------------------------------------------------------------------------------------------

TEST(IPCService,handleServiceEventsOnlyWithNoResponse)
{
	clearIPCServiceTestEnv();
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
    clearIPCServiceTestEnv();
}

//-------------------------------------------------------------------------------------------

TEST(IPCService,sendEventsToClientWithNoResponse)
{
	clearIPCServiceTestEnv();
    QStringList libPaths = QApplication::libraryPaths();
    {
        char *argv = 0;
        track::model::TrackDBTestEnviroment *pTrackDBTest = track::model::TrackDBTestEnviroment::instance();
		IPCService_QtTestApplication tester(IPCService_QtTestApplication::e_sendEventsToClientWithNoResponse,
        	pTrackDBTest->execPath(), 0, &argv);
        tester.exec();
        ASSERT_FALSE(tester.isError());
    }
    QApplication::setLibraryPaths(libPaths);
    clearIPCServiceTestEnv();
}

//-------------------------------------------------------------------------------------------
	
TEST(IPCService,handleServiceEventsOnlyWithResponse)
{
	clearIPCServiceTestEnv();
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
    clearIPCServiceTestEnv();
}

//-------------------------------------------------------------------------------------------

TEST(IPCService,sendEventsToClientWithResponse)
{
	clearIPCServiceTestEnv();
    QStringList libPaths = QApplication::libraryPaths();
    {
        char *argv = 0;
        track::model::TrackDBTestEnviroment *pTrackDBTest = track::model::TrackDBTestEnviroment::instance();
		IPCService_QtTestApplication tester(IPCService_QtTestApplication::e_sendEventsToClientWithResponse,
        	pTrackDBTest->execPath(), 0, &argv);
        tester.exec();
        ASSERT_FALSE(tester.isError());
    }
    QApplication::setLibraryPaths(libPaths);
    clearIPCServiceTestEnv();
}

//-------------------------------------------------------------------------------------------

TEST(IPCService,sendAndReceiveEventsWithNoResponse)
{
	clearIPCServiceTestEnv();
    QStringList libPaths = QApplication::libraryPaths();
    {
        char *argv = 0;
        track::model::TrackDBTestEnviroment *pTrackDBTest = track::model::TrackDBTestEnviroment::instance();
		IPCService_QtTestApplication tester(IPCService_QtTestApplication::e_sendAndReceiveEventsWithNoResponse,
        	pTrackDBTest->execPath(), 0, &argv);
        tester.exec();
        ASSERT_FALSE(tester.isError());
    }
    QApplication::setLibraryPaths(libPaths);
    clearIPCServiceTestEnv();
}

//-------------------------------------------------------------------------------------------

TEST(IPCService,sendAndReceiveEventsWithResponse)
{
	clearIPCServiceTestEnv();
    QStringList libPaths = QApplication::libraryPaths();
    {
        char *argv = 0;
        track::model::TrackDBTestEnviroment *pTrackDBTest = track::model::TrackDBTestEnviroment::instance();
		IPCService_QtTestApplication tester(IPCService_QtTestApplication::e_sendAndReceiveEventsWithResponse,
        	pTrackDBTest->execPath(), 0, &argv);
        tester.exec();
        ASSERT_FALSE(tester.isError());
    }
    QApplication::setLibraryPaths(libPaths);
    clearIPCServiceTestEnv();
}

//-------------------------------------------------------------------------------------------
