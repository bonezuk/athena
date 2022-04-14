//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_IPCSERVICEHANDLER_H
#define __PLAYERAPP_PLAYERCOMMON_IPCSERVICEHANDLER_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"
#include "playerapp/playercommon/inc/OmegaPiBusServiceNames.h"
#include "playerapp/playercommon/inc/IPCSocketComms.h"
#include "playerapp/playercommon/inc/EmbeddedEnv.h"

#include <QCoreApplication>
#include <QThread>
#include <QSemaphore>
#include <QTimer>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class IPCServiceThread;

class PLAYERCOMMON_EXPORT IPCServiceHandler : public QObject
{
	public:
		Q_OBJECT
		friend class IPCServiceThread;
		
	public:
		virtual ~IPCServiceHandler();
		
		static QSharedPointer<IPCServiceHandler> instance(const QString& name);
		virtual void release();
		virtual void postResponse(const QByteArray& arr);
		virtual void wakeupIfNoResponse();
		
	protected:
		static QMutex m_handlersMutex;
		static QMap<QString, IPCServiceThread *> m_handlers;
		
		QString m_serviceName;
		QTimer *m_timer;
		QSharedPointer<IPCSocketComms> m_pComms;
		QMutex m_mutex;
		QSemaphore m_semaphore;
		QByteArray m_responseArray;
		volatile bool m_isResponse;
	
		IPCServiceHandler(const QString& name, QObject *parent = 0);
		
		virtual void printError(const tchar *strR, const tchar *strE) const;
		virtual bool startService();
		virtual void stopService();
		virtual void writeResponse();
		
	protected slots:
		virtual void onProcess();
		
	signals:
		void onProcessRPC(QByteArray);
};

//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT IPCServiceThread : public QThread
{
	Q_OBJECT
	public:
		IPCServiceThread(const QString& name, QObject *parent = 0);
		virtual ~IPCServiceThread();
		
		virtual bool ignite();
		virtual QSharedPointer<IPCServiceHandler> handler();
		
	protected:
		QString m_serviceName;
		QSemaphore m_semaphore;
		QSharedPointer<IPCServiceHandler> m_handler;
		
		virtual void printError(const tchar *strR, const tchar *strE) const;
		virtual void run();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
