//-------------------------------------------------------------------------------------------
// IPCServiceTest_ProgB
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/test/IPCServiceTest_ProgB.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------
// IPCProgBService
//-------------------------------------------------------------------------------------------

IPCProgBService::IPCProgBService(const char *serviceName, QObject *parent) : IPCService(serviceName, parent),
	m_isError(false),
	m_timeEventCounter(0)
{}

//-------------------------------------------------------------------------------------------

IPCProgBService::~IPCProgBService()
{}

//-------------------------------------------------------------------------------------------

void IPCProgBService::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "IPCProgBService::" << strR << " - " << strE << common::c_endl;
	m_isError = true;
}

//-------------------------------------------------------------------------------------------

bool IPCProgBService::isError() const
{
	return m_isError;
}

//-------------------------------------------------------------------------------------------

int IPCProgBService::timeEventCounter() const
{
	return m_timeEventCounter;
}

//-------------------------------------------------------------------------------------------

void IPCProgBService::handleRPCJson(const QJsonDocument& doc)
{
	if(doc.isObject())
	{
		QJsonObject json = doc.object();
		QString funcName = json.value(OMEGA_IPC_FUNCTION_NAME).toString();
		if(!funcName.isEmpty())
		{
			// { "function": "onTime", "value": 12.345 }
			if(funcName == "onClientTime")
			{
				common::TimeStamp tS = json.value("timestamp").toDouble();
				common::Log::g_Log.print("B - onTime - %.8f", static_cast<tfloat64>(tS));
				m_timeEventCounter++;
			}
			// get - { "function": "onClientResponse", "value": 12.345 }
			// return - { "count: 123, "value": 12.345 }
			else if(funcName == "onClientResponse")
			{
				m_timeEventCounter++;
				tfloat64 v = json.value("value").toDouble();
				int count = m_timeEventCounter;
				QVariantMap sMap;
				QJsonDocument respDoc;
				sMap.insert("count", QVariant(count));
				sMap.insert("value", QVariant(v));
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
// ProgBInterface
//-------------------------------------------------------------------------------------------

ProgBInterface::ProgBInterface() : IPCInterfaceBase(IPC_SERVICE_TEST_NAME),
	m_isError(false)
{}

//-------------------------------------------------------------------------------------------

ProgBInterface::~ProgBInterface()
{}

//-------------------------------------------------------------------------------------------

void ProgBInterface::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "ProgBInterface::" << strR << " - " << strE << common::c_endl;
	m_isError = true;
}

//-------------------------------------------------------------------------------------------

bool ProgBInterface::isError() const
{
	return m_isError;
}

//-------------------------------------------------------------------------------------------

void ProgBInterface::onTime(tfloat64 val)
{
	QVariantMap rpcMap;
	common::TimeStamp tStamp(val);
	rpcMap.insert("timestamp", QVariant(static_cast<tfloat64>(tStamp)));
	if(!sendRPCCall("onTime", rpcMap))
	{
		printError("onTime", "Failed to send RPC call");
	}
}

//-------------------------------------------------------------------------------------------

tfloat64 ProgBInterface::onResponse(tfloat64 val, int& count)
{
	tfloat64 res = 0.0;
	QVariantMap rpcMap;
	rpcMap.insert("value", QVariant(val));
	if(sendRPCCall("onResponse", rpcMap))
	{
		QJsonDocument doc = receiveJsonReply();
		if(doc.isObject())
		{
			QJsonValue countJ = doc.object().value("count");
			QJsonValue valueJ = doc.object().value("value");
			if(countJ.isDouble() && valueJ.isDouble())
			{
				count = countJ.toInt();
				res = countJ.toDouble();
			}
			else
			{
				QString err = QString("JSON unexpected '%1'").arg(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
				printError("onResponse", err.toUtf8().constData());
			}
		}
		else
		{
			printError("onResponse", "Unexpected JSON response");
		}
	}
	else
	{
		printError("onResponse", "Failed to send RPC call");
	}
	return res;
}

//-------------------------------------------------------------------------------------------
// IPCServiceTestProgB
//-------------------------------------------------------------------------------------------

IPCServiceTestProgB::IPCServiceTestProgB(int testNo, int argc,char **argv) : QCoreApplication(argc, argv),
	m_testNo(testNo),
	m_isError(false),
	m_timeEventCounter(0),
	m_pInterface(0)
{
	QTimer::singleShot(1, this, SLOT(onRunTest()));
}

//-------------------------------------------------------------------------------------------

IPCServiceTestProgB::~IPCServiceTestProgB()
{}

//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "IPCServiceTestProgB::" << strR << " - " << strE << common::c_endl;
	m_isError = true;
}

//-------------------------------------------------------------------------------------------

bool IPCServiceTestProgB::isError() const
{
	return (m_isError || m_pInterface->isError());
}

//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::onTimeEvent()
{
	fprintf(stdout, "B - onTimeEvent\n");
	common::TimeStamp tS = common::TimeStamp::now();
	m_pInterface->onTime(static_cast<tfloat64>(tS));
	m_timeEventCounter++;
}

//-------------------------------------------------------------------------------------------
// e_handleServiceEventsOnlyWithNoResponse = 1
//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::runOnTimeEventsOnly()
{
	fprintf(stdout, "B - runOnTimeEventsOnly start\n");
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimeEvent()));
	timer->setInterval(1);
	timer->setSingleShot(false);
	timer->start();
	
	while(m_timeEventCounter < c_testIPCServiceCountLimit && !isError())
	{
		processEvents();
	}
	delete timer;
	fprintf(stdout, "B - runOnTimeEventsOnly complete\n");
}

//-------------------------------------------------------------------------------------------
// e_sendEventsToClientWithNoResponse = 2
//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::runServiceForTimeClientEventsOnly(IPCProgBService *service)
{
	fprintf(stdout, "B - runServiceForTimeClientEventsOnly start\n");
	while(service->timeEventCounter() < c_testIPCServiceCountLimit && !service->isError() && !isError())
	{
		processEvents();
	}
	service->stop();
	fprintf(stdout, "B - runServiceForTimeClientEventsOnly complete\n");
}

//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::onTimeResponseEvent()
{
	int count = 0;
	tfloat64 val, ret;
	common::TimeStamp tS = common::TimeStamp::now();
	val = static_cast<tfloat64>(tS);
	ret = m_pInterface->onResponse(val, count);
	fprintf(stdout, "B - onTimeResponseEvent, val=%.8f, count=%d\n", ret, count);
	if(isEqual(val, ret))
	{
		m_timeEventCounter = count;
	}
	else
	{
		printError("onTimeResponseEvent", "Value is not expected result");
	}
}

//-------------------------------------------------------------------------------------------
// e_handleServiceEventsOnlyWithResponse = 3
//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::runOnTimeEventsWithResponse()
{
	fprintf(stdout, "B - runOnTimeEventsWithResponse start\n");
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimeResponseEvent()));
	timer->setInterval(1);
	timer->setSingleShot(false);
	timer->start();
	
	while(m_timeEventCounter < c_testIPCServiceCountLimit && !isError())
	{
		processEvents();
	}
	delete timer;
	fprintf(stdout, "B - runOnTimeEventsWithResponse complete\n");
}

//-------------------------------------------------------------------------------------------
// e_sendEventsToClientWithResponse = 4
//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::runServiceForResponseClientEventsOnly(IPCProgBService *service)
{
	fprintf(stdout, "B - runServiceForResponseClientEventsOnly start\n");
	while(service->timeEventCounter() < c_testIPCServiceCountLimit && !service->isError() && !isError())
	{
		processEvents();
	}
	fprintf(stdout, "B - runServiceForResponseClientEventsOnly complete\n");
}

//-------------------------------------------------------------------------------------------
// e_sendAndReceiveEventsWithNoResponse = 5
//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::runSendAndReceiveEventsWithNoResponse(IPCProgBService *service)
{
	fprintf(stdout, "B - runSendAndReceiveEventsWithNoResponse start\n");
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimeEvent()));
	timer->setInterval(1);
	timer->setSingleShot(false);
	timer->start();

	while((m_timeEventCounter < c_testIPCServiceCountLimit || service->timeEventCounter() < c_testIPCServiceCountLimit) && 
		!service->isError() && !isError())
	{
		processEvents();
	}
	fprintf(stdout, "B - runSendAndReceiveEventsWithNoResponse complete\n");
}

//-------------------------------------------------------------------------------------------
// e_sendAndReceiveEventsWithResponse = 6
//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::runSendAndReceiveEventsWithResponse(IPCProgBService *service)
{
	fprintf(stdout, "B - runSendAndReceiveEventsWithResponse start\n");
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimeResponseEvent()));
	timer->setInterval(1);
	timer->setSingleShot(false);
	timer->start();

	while((m_timeEventCounter < c_testIPCServiceCountLimit || service->timeEventCounter() < c_testIPCServiceCountLimit) && 
		!service->isError() && !isError())
	{
		processEvents();
	}
	fprintf(stdout, "B - runSendAndReceiveEventsWithResponse complete\n");
}

//-------------------------------------------------------------------------------------------

bool IPCServiceTestProgB::isService() const
{
	bool res;
	
	switch(m_testNo)
	{
		case 2:
		case 4:
		case 5:
		case 6:
			res = true;
			break;
		case 1:
		case 3:
		default:
			res = false;
			break;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool IPCServiceTestProgB::isClientInterface() const
{
	bool res;
	
	switch(m_testNo)
	{
		case 1:
		case 3:
		case 5:
		case 6:
			res = true;
			break;
		case 2:
		case 4:
		default:
			res = false;
			break;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool IPCServiceTestProgB::startServiceAsRequired(IPCProgBService **pService)
{
	bool res = false;
	
	if(isService())
	{
		IPCProgBService *service = new IPCProgBService(IPC_SERVICE_PROGB_NAME, this);
		if(service->start())
		{
			fprintf(stdout, "B - service startedt\n");
			*pService = service;
			res = true;
		}
		else
		{
			printError("startServiceAsRequired", "Failed to start service");
			delete service;
		}
	}
	else
	{
		*pService = 0;
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::stopService(IPCProgBService *service)
{
	if(service != 0)
	{
		fprintf(stdout, "B - service stopped\n");
		service->stop();
		delete service;
	}
}

//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::waitForClientInterface()
{
	if(isClientInterface())
	{
		fprintf(stdout, "B - waiting on client interface\n");
		QString path = pathToUNIXSocket(IPC_SERVICE_TEST_NAME);
		while(!common::DiskOps::exist(path))
		{
			QThread::msleep(100);
		}
		m_pInterface = new ProgBInterface();
		fprintf(stdout, "B - client interface defined\n");
	}
}

//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::stopClientInterface()
{
	if(m_pInterface != 0)
	{
		delete m_pInterface;
		m_pInterface = 0;
	}
}

//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::onRunTest()
{
	IPCProgBService *service;

	fprintf(stdout, "B - onRunTest start\n");
	if(startServiceAsRequired(&service))
	{
		waitForClientInterface();
		
		if(m_testNo == 1)
		{
			// e_handleServiceEventsOnlyWithNoResponse = 1
			runOnTimeEventsOnly();
		}
		else if(m_testNo == 2)
		{
			// e_sendEventsToClientWithNoResponse = 2
			runServiceForTimeClientEventsOnly(service);
		}
		else if(m_testNo == 3)
		{
			// e_handleServiceEventsOnlyWithResponse = 3
			runOnTimeEventsWithResponse();
		}
		else if(m_testNo == 4)
		{
			// e_sendEventsToClientWithResponse = 4
			runServiceForResponseClientEventsOnly(service);
		}
		else if(m_testNo == 5)
		{
			// e_sendAndReceiveEventsWithNoResponse = 5
			runSendAndReceiveEventsWithNoResponse(service);
		}
		else if(m_testNo == 6)
		{
			// e_sendAndReceiveEventsWithResponse = 6
			runSendAndReceiveEventsWithResponse(service);
		}
		else
		{
			QString err = QString("Unknown test number %1").arg(m_testNo);
			printError("onRunTest", err.toUtf8().constData());
		}
		
		stopClientInterface();
	}
	else
	{
		printError("onRunTest", "Failed to start service");
	}
	quit();
	fprintf(stdout, "B - onRunTest complete\n");
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------

using namespace orcus;

//-------------------------------------------------------------------------------------------

void cleanEnv()
{
	QString path = pathToUNIXSocket(IPC_SERVICE_PROGB_NAME);
	if(common::DiskOps::exist(path))
	{
		common::DiskOps::remove(path);
	}
}

//-------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	int res;
	
	fprintf(stdout, "B - process started\n");
#if defined(OMEGA_MAC_STORE)
	QCoreApplication::setOrganizationName("Stuart MacLean");
	QCoreApplication::setOrganizationDomain("www.blackomega.co.uk");
	QCoreApplication::setApplicationName("Black Omega Test");
#else
	QCoreApplication::setOrganizationName("Tiger-Eye-Test");
	QCoreApplication::setOrganizationDomain("www.blackomega.co.uk");
	QCoreApplication::setApplicationName("BlackOmega2Test");
#endif
	if(argc < 2)
	{
		fprintf(stdout, "B - test number not given in args\n");
		return -1;
	}

	cleanEnv();

	int testNo = atoi(argv[1]);
	IPCServiceTestProgB prog(testNo, argc, argv);
	prog.exec();
	res = prog.isError() ? -1 : 0;
	fprintf(stdout, "B - process finished\n");
	
	cleanEnv();
	
	return res;
}

//-------------------------------------------------------------------------------------------
