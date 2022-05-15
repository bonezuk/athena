//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_IPCSERVICE_H
#define __PLAYERAPP_PLAYERCOMMON_IPCSERVICE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCServiceThread.h"

#include <QJsonDocument>

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT IPCService : public QObject
{
	Q_OBJECT
	
	public:
		IPCService(const char *serviceName, QObject *parent = 0);
		virtual ~IPCService();
		
		virtual bool start();
		virtual void stop();
		
	protected:
		QString m_serviceName;
		QSharedPointer<IPCServiceHandler> m_pServiceHandler;
		
		virtual void printError(const char *strR, const char *strE) const;
		
		virtual void handleRPCJson(const QJsonDocument& doc) = 0;
		
	private slots:
		void processRPC(QByteArray rpcArray);
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
