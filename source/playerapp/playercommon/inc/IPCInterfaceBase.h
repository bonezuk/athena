//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_IPCINTERFACEBASE_H
#define __PLAYERAPP_PLAYERCOMMON_IPCINTERFACEBASE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCSocketComms.h"
#include "playerapp/playercommon/inc/OmegaPiBusServiceNames.h"
#include "playerapp/playercommon/inc/EmbeddedEnv.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT IPCInterfaceBase
{
	public:
		IPCInterfaceBase(const QString& serviceName);
		virtual ~IPCInterfaceBase();
		
	protected:
		QString m_serviceName;
		QSharedPointer<IPCSocketComms> m_pIPCComms;
	
		virtual void printError(const char *strR, const char *strE) const;
		virtual QSharedPointer<IPCSocketComms> getIPCComms();
		virtual void sendRPCCall(const QString& funcName);
		virtual void sendRPCCall(const QString& funcName, QVariantMap& rpcMap);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
