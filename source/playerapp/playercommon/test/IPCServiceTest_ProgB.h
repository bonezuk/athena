//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_IPCSERVICETEST_PROGB_H
#define __PLAYERAPP_PLAYERCOMMON_IPCSERVICETEST_PROGB_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/test/IPCServiceTestCommon.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class ProgBInterface : public IPCInterfaceBase
{
	public:
		ProgBInterface();
		virtual ~ProgBInterface();
		
		virtual bool isError() const;
		virtual void onTime(tfloat64 val);
		
	protected:
		mutable bool m_isError;
		
		virtual void printError(const tchar *strR, const tchar *strE) const;
};

//-------------------------------------------------------------------------------------------

class IPCProgBService : public IPCService
{
	Q_OBJECT
	public:
		IPCProgBService(const char *serviceName, QObject *parent = 0);
		virtual ~IPCProgBService();
		
		bool isError() const;
		int timeEventCounter() const;
		
	protected:
		mutable bool m_isError;
		int m_timeEventCounter;
		
		virtual void printError(const char *strR, const char *strE) const;		
		virtual void handleRPCJson(const QJsonDocument& doc);
};

//-------------------------------------------------------------------------------------------

class IPCServiceTestProgB : public QCoreApplication
{
	Q_OBJECT
	public:
		IPCServiceTestProgB(int testNo, int argc, char **argv);
		virtual ~IPCServiceTestProgB();
		
		virtual bool isError() const;
		
	private:
		int m_testNo;
		mutable bool m_isError;
		int m_timeEventCounter;
		ProgBInterface *m_pInterface;
		
		virtual void printError(const tchar *strR, const tchar *strE) const;
		virtual void runOnTimeEventsOnly();
		virtual void runServiceForTimeClientEventsOnly();
		
	private slots:
		virtual void onRunTest();
		virtual void onTimeEvent();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
