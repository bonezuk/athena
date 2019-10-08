//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_CONTROLLER_H
#define __ORCUS_NETWORK_CONTROLLER_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Socket.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
//-------------------------------------------------------------------------------------------

class ControllerThread;
class TCPConnectionSocket;

//-------------------------------------------------------------------------------------------

class ControllerEvent : public QEvent
{
	public:
	
		typedef enum
		{
			e_newService = QEvent::User + 1000,
			e_getService,
			e_deleteService
		} ControllerEventType;
		
	public:
		ControllerEvent(ControllerEventType t);
		
		Qt::HANDLE threadId();
		
		const QString& name() const;
		void name(const QString& n);
		
		QSharedPointer<Service> service() const;
		void service(QSharedPointer<Service>& s);
		
	protected:
		
		Qt::HANDLE m_threadId;
		QString m_name;
		QSharedPointer<Service> m_service;
};

//-------------------------------------------------------------------------------------------

class ControllerWaitCondition
{
	public:
		ControllerWaitCondition();
		
		void get();
		void wait();
		void wake();
		
		QSharedPointer<Service> service() const;
		void service(QSharedPointer<Service>& s);		
		
	protected:
	
		QMutex m_mutex;
		QWaitCondition m_condition;
		QSharedPointer<Service> m_service;
};

//-------------------------------------------------------------------------------------------

class NETWORK_EXPORT Controller : public QObject
{
	public:
		Q_OBJECT
		
	public:
		friend class ControllerThread;
		friend class Socket;
		friend class Service;
		friend class TCPConnectionSocket;
		
		typedef QSharedPointer<Controller> ControllerSPtr;
		
	public:
		Controller(tint id,QObject *parent = 0);
		virtual ~Controller();
		
		static QSharedPointer<Controller> instance(tint id = 1);
		static void end(tint id = 1);
		
		tint id() const;
		
		QSharedPointer<Service> newService(const tchar *name);
		QSharedPointer<Service> newService(const QString& name);
		
		QSharedPointer<Service> getService(const tchar *name);
		QSharedPointer<Service> getService(const QString& name);
		
		void deleteService(QSharedPointer<Service>& s);
		
	protected:
		
		static QMutex m_mutex;
		static QMap<tint,QSharedPointer<Controller> > m_controlMap;
		
		tint m_id;
		ControllerThread *m_thread;
		QTimer *m_processTimer;
		QMap<tint,QSharedPointer<Service> > m_serviceMap;
		QSet<QSharedPointer<Socket> > m_socketSet;
		QMap<QSharedPointer<Service>, QSharedPointer<Socket> > m_socketServiceMap;
		QMap<Qt::HANDLE,ControllerWaitCondition *> m_waitConditionMap;
		
		void printError(const tchar *strR,const tchar *strE) const;
		
		bool start();
		void stop();
		
		virtual bool event(QEvent *e);
		
		ControllerWaitCondition *getCondition();
		ControllerWaitCondition *getCondition(Qt::HANDLE id);
		void freeConditions();
		
		QSharedPointer<Service> onNewService(const QString& name);
		QSharedPointer<Service> onGetService(const QString& name);
		void onDeleteService(QSharedPointer<Service>& s);
		
		void addSocket(QSharedPointer<Service>& svr,QSharedPointer<Socket>& s);
		void delSocket(QSharedPointer<Service>& svr,QSharedPointer<Socket>& s);
		void delSocket(QSharedPointer<Service>& svr,Socket *s);
		void delSocketService(QSharedPointer<Service>& svr, QSharedPointer<Socket>& s);
		
		void doRead(QSharedPointer<Service>& s);
		void doWrite(QSharedPointer<Service>& s);
		
	protected slots:
	
		void onProcess();

	signals:

		void socketError(QSharedPointer<Service>& s);
};

//-------------------------------------------------------------------------------------------

class ControllerThread : public QThread
{
	public:
		Q_OBJECT
		
		friend class Controller;
		
	public:
		ControllerThread(QObject *parent = 0);
		virtual ~ControllerThread();
		
		virtual bool ignite(tint id);
		
		QSharedPointer<Controller> controller();
		
	protected:
	
		volatile tint m_id;
		QMutex m_mutex;
		QWaitCondition m_condition;
		
		QSharedPointer<Controller> m_controller;
		
		virtual void run();
};

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

