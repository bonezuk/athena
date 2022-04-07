//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_IPCSERVICETEST_H
#define __PLAYERAPP_PLAYERCOMMON_IPCSERVICETEST_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/test/IPCServiceTestCommon.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class IPCTestService : public IPCService
{
	Q_OBJECT
	public:
		IPCTestService(const char *serviceName, QObject *parent = 0);
		virtual ~IPCTestService();
		
		bool isError() const;
		int timeEventCounter() const;
		
	protected:
		mutable bool m_isError;
		int m_timeEventCounter;
		
		virtual void printError(const char *strR, const char *strE) const;		
		virtual void handleRPCJson(const QJsonDocument& doc);
};

//-------------------------------------------------------------------------------------------

class IPCProgBInterface : public IPCInterfaceBase
{
	public:
		IPCProgBInterface();
		virtual ~IPCProgBInterface();
		
		virtual bool isError() const;
		virtual void onClientTime(tfloat64 val);
		
	protected:
		mutable bool m_isError;
		
		virtual void printError(const tchar *strR, const tchar *strE) const;

};

//-------------------------------------------------------------------------------------------

class IPCService_QtTestApplication : public QCoreApplication
{
	Q_OBJECT
	public:
		typedef enum
		{
			e_handleServiceEventsOnlyWithNoResponse = 1,
			e_sendEventsToClientWithNoResponse = 2,
		} TestType;
		
	public:
		IPCService_QtTestApplication(TestType testNo, const QString& pathExec, int argc, char **argv);
		virtual ~IPCService_QtTestApplication();
		
		bool isError() const;
	private:
		TestType m_testNo;
		mutable bool m_isError;
		QString m_pathToTestExec;
		IPCProgBInterface *m_pInterface;
		int m_testCount;
		
		virtual void printError(const tchar *strR, const tchar *strE) const;
		virtual QString pathToTestProgramB();
		virtual bool isTestService();
		virtual bool isClientService();
		virtual bool startProgBInterface();
		virtual void stopProgBInterface();
		virtual void runProcessTest(IPCTestService *service);
		virtual void handleServiceEventsOnlyWithNoResponse(IPCTestService *service);
		virtual void sendEventsToClientWithNoResponse();
	private slots:
		virtual void onRunTest();
		virtual void onSendClientTimeEvent();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
