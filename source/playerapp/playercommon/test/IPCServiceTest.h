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

class IPCService_QtTestApplication : public QCoreApplication
{
	Q_OBJECT
	public:
		typedef enum
		{
			e_handleServiceEventsOnlyWithNoResponse = 1
		} TestType;
		
	public:
		IPCService_QtTestApplication(TestType testNo, const QString& pathExec, int argc, char **argv);
		
		bool isError() const;
	private:
		TestType m_testNo;
		mutable bool m_isError;
		QString m_pathToTestExec;
		
		virtual void printError(const tchar *strR, const tchar *strE) const;
		virtual QString pathToTestProgramB();
		virtual void runProcessTest(IPCTestService *service);
		virtual void handleServiceEventsOnlyWithNoResponse(IPCTestService *service);
	private slots:
		virtual void onRunTest();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
