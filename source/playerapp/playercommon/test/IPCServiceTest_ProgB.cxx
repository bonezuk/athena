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
// IPCServiceTestProgB
//-------------------------------------------------------------------------------------------

IPCServiceTestProgB::IPCServiceTestProgB(int testNo, int argc,char **argv) : QCoreApplication(argc, argv),
	m_testNo(testNo),
	m_isError(false),
	m_timeEventCounter(0),
	m_pInterface(new ProgBInterface())
{
	QTimer::singleShot(1, this, SLOT(onRunTest()));
}

//-------------------------------------------------------------------------------------------

IPCServiceTestProgB::~IPCServiceTestProgB()
{
	delete m_pInterface;
	m_pInterface = 0;
}

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

void IPCServiceTestProgB::onRunTest()
{
	fprintf(stdout, "B - onRunTest start\n");
	if(m_testNo == 1)
	{
		runOnTimeEventsOnly();
	}
	else if(m_testNo == 2)
	{
		runServiceForTimeClientEventsOnly();
	}
	else
	{
		QString err = QString("Unknown test number %1").arg(m_testNo);
		printError("onInit", err.toUtf8().constData());
	}
	quit();
	fprintf(stdout, "B - onRunTest complete\n");
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
	fprintf(stdout, "B - runOnTimeEventsOnly complete\n");
}

//-------------------------------------------------------------------------------------------

void IPCServiceTestProgB::runServiceForTimeClientEventsOnly()
{
	fprintf(stdout, "B - runServiceForTimeClientEventsOnly start\n");
	IPCProgBService *service = new IPCProgBService(IPC_SERVICE_PROGB_NAME, this);
	if(service->start())
	{
		while(service->timeEventCounter() < c_testIPCServiceCountLimit && !service->isError() && !isError())
		{
			processEvents();
		}
		service->stop();
	}
	else
	{
		printError("runServiceForTimeClientEventsOnly", "Failed to start service");
	}
	delete service;
	fprintf(stdout, "B - runServiceForTimeClientEventsOnly complete\n");
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
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

	int testNo = atoi(argv[1]);
	orcus::IPCServiceTestProgB prog(testNo, argc, argv);
	prog.exec();
	res = prog.isError() ? -1 : 0;
	fprintf(stdout, "B - process finished\n");
	return res;
}

//-------------------------------------------------------------------------------------------
