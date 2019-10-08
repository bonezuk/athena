#include "network/inc/Controller.h"
#include "network/inc/Service.h"

//#define ORCUS_NETWORK_CONTROLLER_DB 1
#if defined(ORCUS_NETWORK_CONTROLLER_DB)
#define DB(x) x
#else
#define DB(x)
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
//-------------------------------------------------------------------------------------------
// ControllerEvent
//-------------------------------------------------------------------------------------------

ControllerEvent::ControllerEvent(ControllerEventType t) : QEvent(static_cast<QEvent::Type>(t)),
	m_threadId(QThread::currentThreadId()),
	m_name(),
	m_service()
{}

//-------------------------------------------------------------------------------------------

Qt::HANDLE ControllerEvent::threadId()
{
	return m_threadId;
}

//-------------------------------------------------------------------------------------------

const QString& ControllerEvent::name() const
{
	return m_name;
}

//-------------------------------------------------------------------------------------------

void ControllerEvent::name(const QString& n)
{
	m_name = n;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<Service> ControllerEvent::service() const
{
	QSharedPointer<Service> pService(m_service);
	return pService;
}

//-------------------------------------------------------------------------------------------

void ControllerEvent::service(QSharedPointer<Service>& s)
{
	m_service = s;
}

//-------------------------------------------------------------------------------------------
// ControllerWaitCondition
//-------------------------------------------------------------------------------------------

ControllerWaitCondition::ControllerWaitCondition() : m_mutex(),
	m_condition(),
	m_service()
{}

//-------------------------------------------------------------------------------------------

void ControllerWaitCondition::get()
{
	m_mutex.lock();
}

//-------------------------------------------------------------------------------------------

void ControllerWaitCondition::wait()
{
	m_condition.wait(&m_mutex);
	m_mutex.unlock();
}

//-------------------------------------------------------------------------------------------

void ControllerWaitCondition::wake()
{
	m_mutex.lock();
	m_condition.wakeAll();
	m_mutex.unlock();
}

//-------------------------------------------------------------------------------------------

QSharedPointer<Service> ControllerWaitCondition::service() const
{
	QSharedPointer<Service> pService(m_service);
	return pService;
}

//-------------------------------------------------------------------------------------------

void ControllerWaitCondition::service(QSharedPointer<Service>& s)
{
	m_service = s;
}

//-------------------------------------------------------------------------------------------
// ControllerThread
//-------------------------------------------------------------------------------------------

ControllerThread::ControllerThread(QObject *parent) : QThread(parent),
	m_id(1),
	m_mutex(),
	m_condition(),
	m_controller()
{}

//-------------------------------------------------------------------------------------------

ControllerThread::~ControllerThread()
{}

//-------------------------------------------------------------------------------------------

bool ControllerThread::ignite(tint id)
{
	m_id = id;
	m_mutex.lock();
	start();
	m_condition.wait(&m_mutex);
	m_mutex.unlock();
    return (m_controller.data()!=0) ? true : false;
}

//-------------------------------------------------------------------------------------------

Controller::ControllerSPtr ControllerThread::controller()
{
	return m_controller;
}

//-------------------------------------------------------------------------------------------

void ControllerThread::run()
{	
	m_mutex.lock();
	m_mutex.unlock();
	
	Controller::ControllerSPtr ctrl(new Controller(static_cast<tint>(m_id)));
	
	m_controller = ctrl;
	m_controller->m_thread = this;
	if(m_controller->start())
	{
		m_condition.wakeAll();
		exec();
		m_controller->stop();
	}
	else
	{
		Controller::ControllerSPtr eCtrl;
		m_controller->stop();
		m_controller = eCtrl;
		m_condition.wakeAll();
	}
}

//-------------------------------------------------------------------------------------------
// Controller
//-------------------------------------------------------------------------------------------

QMutex Controller::m_mutex;
QMap<tint,QSharedPointer<Controller> > Controller::m_controlMap;

//-------------------------------------------------------------------------------------------

Controller::Controller(tint id,QObject *parent) : QObject(parent),
	m_id(id),
	m_thread(0),
	m_processTimer(0),
	m_serviceMap(),
	m_socketSet()
{}

//-------------------------------------------------------------------------------------------

Controller::~Controller()
{}

//-------------------------------------------------------------------------------------------

void Controller::printError(const tchar *strR,const tchar *strE) const
{
	common::Log::g_Log << "Controller::" << strR << " - " << strE << "." << common::c_endl;
}

//-------------------------------------------------------------------------------------------

Controller::ControllerSPtr Controller::instance(tint id)
{
	QMap<tint,QSharedPointer<Controller> >::iterator ppI;
	QSharedPointer<Controller> ctrl;
	
	m_mutex.lock();
	ppI = m_controlMap.find(id);
	if(ppI==m_controlMap.end())
	{
		ControllerThread *cThread = new ControllerThread;
		if(cThread->ignite(id))
		{
			ctrl = cThread->controller();
			m_controlMap.insert(id,ctrl);
		}
		else
		{
			delete cThread;
		}
	}
	else
	{
		ctrl = ppI.value();
	}
	m_mutex.unlock();
	
	return ctrl;
}

//-------------------------------------------------------------------------------------------

void Controller::end(tint id)
{
	QMap<tint,QSharedPointer<Controller> >::iterator ppI;
	
	m_mutex.lock();
	ppI = m_controlMap.find(id);
	if(ppI!=m_controlMap.end())
	{
		m_mutex.unlock();

		ControllerThread *cThread = ppI.value()->m_thread;
		
		if(cThread!=0)
		{
			cThread->quit();
			cThread->wait();
			delete cThread;
		}

		m_mutex.lock();
		m_controlMap.erase(ppI);
		m_mutex.unlock();
	}
	else
	{
		m_mutex.unlock();
	}
}

//-------------------------------------------------------------------------------------------

bool Controller::start()
{
	m_processTimer = new QTimer(this);
	QObject::connect(m_processTimer,SIGNAL(timeout()),this,SLOT(onProcess()));
	// Timer set to have regular processing but not to overload network layer.
	m_processTimer->start(50);
	return true;
}

//-------------------------------------------------------------------------------------------

void Controller::stop()
{
	QMap<tint,QSharedPointer<Service> >::iterator ppI;
	QSet<QSharedPointer<Service> >::iterator ppJ;
	
	while(ppI=m_serviceMap.begin(), ppI!=m_serviceMap.end())
	{
		QSharedPointer<Service> s = ppI.value();
		m_serviceMap.erase(ppI);
		s->stop();
	}
	
	while(ppJ=m_socketSet.begin(), ppJ!=m_socketSet.end())
	{
		QSharedPointer<Socket> sk = *ppJ;
		m_socketSet.erase(ppJ);
	}
	
	m_processTimer->stop();
	QObject::disconnect(m_processTimer,SIGNAL(timeout()),this,SLOT(onProcess()));
	delete m_processTimer;
	m_processTimer = 0;
	
	freeConditions();
}

//-------------------------------------------------------------------------------------------

void Controller::onProcess()
{
	tint nfds = -1;
	fd_set readS,writeS;
	QSet<QSharedPointer<Socket> > readSet,writeSet;
	QVector<QSharedPointer<Socket> > rSelect,wSelect;
	QSet<QSharedPointer<Socket> >::iterator ppI;
	QVector<QSharedPointer<Socket> >::iterator ppJ;
	QMap<tint,QSharedPointer<Service> >::iterator ppK;
	bool writeFlag = true;
	
	FD_ZERO(&readS);
	FD_ZERO(&writeS);
	
	for(ppI=m_socketSet.begin();ppI!=m_socketSet.end();++ppI)
	{
		QSharedPointer<Socket> s = *ppI;
		const tuint32& state = s->state();
		
		if(s->socket()!=c_invalidSocket)
		{
			tint fd = static_cast<tint>(s->socket());
		
			if(s->canRead())
			{
				if(state & Socket::c_socketStateRead)
				{
					readSet.insert(s);
				}
				else
				{
					nfds = (nfds < fd) ? fd : nfds;
					FD_SET(fd,&readS);
					rSelect.append(s);
				}
			}
		
			if(s->canWrite())
			{
				if(state & Socket::c_socketStateWrite)
				{
					writeSet.insert(s);
				}
				else
				{
					nfds = (nfds < fd) ? fd : nfds;
					FD_SET(fd,&writeS);
					wSelect.append(s);
				}
			}
		}
	}
	nfds++;
	
	if(nfds > 0)
	{
		tint res;
		timeval t;
		
		t.tv_sec = 0;
		t.tv_usec = 0;
		
		res = ::select(nfds,&readS,&writeS,0,&t);
		if(res > 0)
		{
			for(ppJ=rSelect.begin();ppJ!=rSelect.end();++ppJ)
			{
				QSharedPointer<Socket> s = *ppJ;
				
				if(FD_ISSET(s->socket(),&readS))
				{
					s->setForRead();
					readSet.insert(s);
				}
			}
			
			for(ppJ=wSelect.begin();ppJ!=wSelect.end();++ppJ)
			{
				QSharedPointer<Socket> s = *ppJ;
				
				if(FD_ISSET(s->socket(),&writeS))
				{
					s->setForWrite();
					writeSet.insert(s);
				}
			}
		}
	}
	
	for(ppI=readSet.begin();ppI!=readSet.end();++ppI)
	{
		QSharedPointer<Socket> s = *ppI;
		if(!s->doRead())
		{
			emit socketError(s);
		}
	}
	
	for(ppK=m_serviceMap.begin();ppK!=m_serviceMap.end();++ppK)
	{
		if(!(ppK.value()->process()))
		{
			writeFlag = false;
		}
	}

	if(writeFlag)
	{
		for(ppI=writeSet.begin();ppI!=writeSet.end();++ppI)
		{
			QSharedPointer<Socket> s = *ppI;
			
			if(m_socketSet.find(s)!=m_socketSet.end())
			{
				if(!s->doWrite())
				{
					emit socketError(s);
				}
			}
		}
	}

	for(ppK=m_serviceMap.begin();ppK!=m_serviceMap.end();++ppK)
	{
		ppK.value()->processComplete();
	}
}

//-------------------------------------------------------------------------------------------

void Controller::doRead(QSharedPointer<Service>& s)
{
	tint nfds = -1;
	fd_set readS;
	QSet<QSharedPointer<Socket> > readSet;
	QVector<QSharedPointer<Socket> > rSelect;
	QSet<QSharedPointer<Socket> >::iterator ppI;
	QVector<QSharedPointer<Socket> >::iterator ppJ;
	QMap<QSharedPointer<Service>, QSharedPointer<Socket> >::iterator ppK;

	FD_ZERO(&readS);
	
	for(ppK=m_socketServiceMap.find(s);ppK!=m_socketServiceMap.end() && ppK.key()==s;++ppK)
	{
		QSharedPointer<Socket> s = ppK.value();
		const tuint32& state = s->state();
		
		if(s->socket()!=c_invalidSocket && s->canRead())
		{
			tint fd = static_cast<tint>(s->socket());
			
			if(state & Socket::c_socketStateRead)
			{
				readSet.insert(s);
			}
			else
			{
				nfds = (nfds < fd) ? fd : nfds;
				FD_SET(fd,&readS);
				rSelect.append(s);
			}
		}
	}
	nfds++;
	
	if(nfds > 0)
	{
		tint res;
		timeval t;
		
		t.tv_sec = 0;
		t.tv_usec = 0;
		
		res = ::select(nfds,&readS,0,0,&t);
		if(res > 0)
		{
			for(ppJ=rSelect.begin();ppJ!=rSelect.end();++ppJ)
			{
				QSharedPointer<Socket> s = *ppJ;
				
				if(FD_ISSET(s->socket(),&readS))
				{
					s->setForRead();
					readSet.insert(s);
				}
			}
		}
	}
	
	for(ppI=readSet.begin();ppI!=readSet.end();++ppI)
	{
		QSharedPointer<Socket> s = *ppI;
		if(!s->doRead())
		{
			emit socketError(s);
		}
	}
}

//-------------------------------------------------------------------------------------------

void Controller::doWrite(QSharedPointer<Service>& s)
{
	tint nfds = -1;
	fd_set writeS;
	QSet<QSharedPointer<Socket> > writeSet;
	QVector<QSharedPointer<Socket> > wSelect;
	QSet<QSharedPointer<Socket> >::iterator ppI;
	QVector<QSharedPointer<Socket> >::iterator ppJ;
	QMap<QSharedPointer<Service>,QSharedPointer<Socket> >::iterator ppK;
	
	FD_ZERO(&writeS);
	
	for(ppK=m_socketServiceMap.find(s);ppK!=m_socketServiceMap.end() && ppK.key()==s;++ppK)
	{
		QSharedPointer<Socket> s = ppK.value();
		const tuint32& state = s->state();
		
		if(s->socket()!=c_invalidSocket && s->canWrite())
		{
			tint fd = static_cast<tint>(s->socket());
			
			if(state & Socket::c_socketStateWrite)
			{
				writeSet.insert(s);
			}
			else
			{
				nfds = (nfds < fd) ? fd : nfds;
				FD_SET(fd,&writeS);
				wSelect.append(s);
			}
		}
	}
	nfds++;
	
	if(nfds > 0)
	{
		tint res;
		timeval t;
		
		t.tv_sec = 0;
		t.tv_usec = 0;
		
		res = ::select(nfds,0,&writeS,0,&t);
		if(res > 0)
		{
			for(ppJ=wSelect.begin();ppJ!=wSelect.end();++ppJ)
			{
				QSharedPointer<Socket> s = *ppJ;
				
				if(FD_ISSET(s->socket(),&writeS))
				{
					s->setForWrite();
					writeSet.insert(s);
				}
			}
		}
	}
	
	for(ppI=writeSet.begin();ppI!=writeSet.end();++ppI)
	{
		QSharedPointer<Socket> s = *ppI;
		if(!s->doWrite())
		{
			emit socketError(s);
		}
	}
}

//-------------------------------------------------------------------------------------------

ControllerWaitCondition *Controller::getCondition()
{
	ControllerWaitCondition *c = getCondition(QThread::currentThreadId());
	c->get();
	return c;
}

//-------------------------------------------------------------------------------------------

ControllerWaitCondition *Controller::getCondition(Qt::HANDLE id)
{
	ControllerWaitCondition *c;
	QMap<Qt::HANDLE,ControllerWaitCondition *>::iterator ppI;
	
	m_mutex.lock();
	ppI = m_waitConditionMap.find(id);
	if(ppI!=m_waitConditionMap.end())
	{
		c = ppI.value();
	}
	else
	{
		c = new ControllerWaitCondition;
		m_waitConditionMap.insert(id,c);
	}
	m_mutex.unlock();
	return c;
}

//-------------------------------------------------------------------------------------------

void Controller::freeConditions()
{
	QMap<Qt::HANDLE,ControllerWaitCondition *>::iterator ppI;
	
	m_mutex.lock();
	while(ppI = m_waitConditionMap.begin(), ppI!=m_waitConditionMap.end())
	{
		ControllerWaitCondition *c = ppI.value();
		m_waitConditionMap.erase(ppI);
		delete c;
	}
	m_mutex.unlock();
}

//-------------------------------------------------------------------------------------------

QSharedPointer<Service> Controller::newService(const tchar *name)
{
	return newService(QString::fromLatin1(name));
}

//-------------------------------------------------------------------------------------------

QSharedPointer<Service> Controller::newService(const QString& name)
{
	ControllerEvent *e = new ControllerEvent(ControllerEvent::e_newService);
	ControllerWaitCondition *c = getCondition();
	QSharedPointer<Service> svr;
	
	e->name(name);
	QCoreApplication::postEvent(this,e);
	c->wait();
	svr = c->service();
	return svr;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<Service> Controller::getService(const tchar *name)
{
	return getService(QString::fromLatin1(name));
}

//-------------------------------------------------------------------------------------------

QSharedPointer<Service> Controller::getService(const QString& name)
{
	ControllerEvent *e = new ControllerEvent(ControllerEvent::e_getService);
	ControllerWaitCondition *c = getCondition();
	QSharedPointer<Service> svr;
	
	e->name(name);
	QCoreApplication::postEvent(this,e);
	c->wait();
	svr = c->service();
	return svr;
}

//-------------------------------------------------------------------------------------------

void Controller::deleteService(QSharedPointer<Service> s)
{
	ControllerEvent *e = new ControllerEvent(ControllerEvent::e_deleteService);
	ControllerWaitCondition *c = getCondition();
	
	e->service(s);
	QCoreApplication::postEvent(this,e);
	c->wait();
}

//-------------------------------------------------------------------------------------------

bool Controller::event(QEvent *e)
{
	if(e!=0 && static_cast<ControllerEvent::ControllerEventType>(e->type())>=ControllerEvent::e_newService)
	{
		ControllerEvent::ControllerEventType t = static_cast<ControllerEvent::ControllerEventType>(e->type());
		ControllerWaitCondition *c;
		
		switch(t)
		{
			case ControllerEvent::e_newService:
				{
					ControllerEvent *cEvent = reinterpret_cast<ControllerEvent *>(e);
					QSharedPointer<Service> s;
					s = onNewService(cEvent->name());
					c = getCondition(cEvent->threadId());
					c->service(s);
					c->wake();
					e->accept();
				}
				break;
			
			case ControllerEvent::e_getService:
				{
					ControllerEvent *cEvent = reinterpret_cast<ControllerEvent *>(e);
					QSharedPointer<Service> s;
					s = onGetService(cEvent->name());
					c = getCondition(cEvent->threadId());
					c->service(s);
					c->wake();
					e->accept();
				}
				break;
			
			case ControllerEvent::e_deleteService:
				{
					ControllerEvent *cEvent = reinterpret_cast<ControllerEvent *>(e);
					onDeleteService(cEvent->service());
					c = getCondition(cEvent->threadId());
					c->wake();
					e->accept();
				}
				break;
				
			default:
				return QObject::event(e);
				break;
		}
		return true;
	}
	else
	{
		return QObject::event(e);
	}
}

//-------------------------------------------------------------------------------------------

QSharedPointer<Service> Controller::onNewService(const QString& name)
{
	QSharedPointer<Service> svr = ServiceFactory::createUnmanaged(name,this);
	QMap<tint,QSharedPointer<Service> >::iterator ppI;
	QString err;
	
	if(svr!=0)
	{
		if(svr->start())
		{
			svr->m_name = name;
			m_serviceMap.insert(svr->id(),svr);
		}
		else
		{
			err = "Failed to start service '" + name + "'";
			printError("onNewService",err.toLatin1().constData());
			delete svr;
			svr = 0;
		}
	}
	else
	{
		err = "Failed to create service '" + name + "'";
		printError("onNewService",err.toLatin1().constData());
	}
	return svr;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<Service> Controller::onGetService(const QString& name)
{
	QSharedPointer<Service> svr = 0;
	QMap<tint,QSharedPointer<Service> >::iterator ppI;
	
	for(ppI=m_serviceMap.begin();svr==0 && ppI!=m_serviceMap.end();++ppI)
	{
		if(ppI.value()->name()==name)
		{
			svr = ppI.value();
		}
	}
	if(svr==0)
	{
		svr = onNewService(name);
	}
	return svr;
}

//-------------------------------------------------------------------------------------------

void Controller::onDeleteService(QSharedPointer<Service>& s)
{
	if(s!=0)
	{
		QMap<tint,QSharedPointer<Service> >::iterator ppI = m_serviceMap.find(s->id());
		
		s->stop();
		
		if(ppI!=m_serviceMap.end())
		{
			m_serviceMap.erase(ppI);
		}
	}
}

//-------------------------------------------------------------------------------------------

void Controller::addSocket(QSharedPointer<Service>& svr, QSharedPointer<Socket>& s)
{
	if(s!=0)
	{
		m_socketSet.insert(s);
		m_socketServiceMap.insertMulti(svr,s);
	}
}

//-------------------------------------------------------------------------------------------

void Controller::delSocketService(QSharedPointer<Service>& svr, QSharedPointer<Socket>& s)
{
	QMap<QSharedPointer<Service>, QSharedPointer<Socket> >::iterator ppJ = m_socketServiceMap.find(svr);
	
	while(ppJ!=m_socketServiceMap.end() && ppJ.key()==svr)
	{
		if(ppJ.value()==s)
		{
			ppJ = m_socketServiceMap.erase(ppJ);
			break;
		}
		else
		{
			++ppJ;
		}
	}
}

//-------------------------------------------------------------------------------------------

void Controller::delSocket(QSharedPointer<Service>& svr, QSharedPointer<Socket>& s)
{
	if(!s.isNull())
	{
		QSet<QSharedPointer<Socket> >::iterator ppI;
		
		delSocketService(svr, s);
		
		ppI = m_socketSet.find(s);
		if(ppI!=m_socketSet.end())
		{
			m_socketSet.erase(ppI);
		}		
	}
}

//-------------------------------------------------------------------------------------------

void Controller::delSocket(QSharedPointer<Service>& svr,Socket *s)
{
	if(s != 0)
	{
		QSet<QSharedPointer<Socket> >::iterator ppI;
		
		for(ppI = m_socketSet.begin(); ppI != m_socketSet.end(); ++ppI)
		{
			QSharedPointer<Socket>& pSocket = *ppI;
			if(pSocket.data() == s)
			{
				delSocketService(svr, pSocket);
				pSocket.erase(ppI);
				break;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
