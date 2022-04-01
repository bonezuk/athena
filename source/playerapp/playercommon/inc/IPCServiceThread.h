//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_IPCSERVICETHREAD_H
#define __PLAYERAPP_PLAYERCOMMON_IPCSERVICETHREAD_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"
#include "playerapp/playercommon/inc/OmegaPiBusServiceNames.h"
#include "playerapp/playercommon/inc/IPCSocketComms.h"
#include "playerapp/playercommon/inc/EmbeddedEnv.h"

#include <QCoreApplication>
#include <QThread>
#include <QMutex>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT IPCServiceThread : public QThread
{
	Q_OBJECT
	public:
		IPCServiceThread(const QString& serviceName, QObject *parent = 0);
		virtual ~IPCServiceThread();
		
		virtual bool startService();
		virtual void stopService();
		
		virtual void postResponse(const QByteArray& arr);
		
	protected:
		QString m_serviceName;
		QSharedPointer<IPCSocketComms> m_pComms;
		volatile bool m_running;
		QMutex m_responseMutex;
		QList<QByteArray> m_responseList;
		
		virtual void printError(const char *strR, const char *strE) const;
		virtual void run() override;
		virtual void writeResponse();
		
	signals:
		void onProcessRPC(QByteArray);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
