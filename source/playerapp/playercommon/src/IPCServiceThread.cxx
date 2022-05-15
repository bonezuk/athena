#include "playerapp/playercommon/inc/IPCServiceThread.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------
// IPCServiceThread
//-------------------------------------------------------------------------------------------

IPCServiceThread::IPCServiceThread(const QString& name, QObject *parent) : QThread(parent),
	m_serviceName(name),
	m_semaphore(0),
	m_handler()
{}

//-------------------------------------------------------------------------------------------

IPCServiceThread::~IPCServiceThread()
{}

//-------------------------------------------------------------------------------------------

void IPCServiceThread::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "IPCServiceThread::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<IPCServiceHandler> IPCServiceThread::handler()
{
	return m_handler;
}

//-------------------------------------------------------------------------------------------

bool IPCServiceThread::ignite()
{
	common::Log::g_Log.print("(%d) - IPCServiceThread::ignite - a\n", (tuint64)(QThread::currentThreadId()));
	start();
	m_semaphore.acquire(1);
	common::Log::g_Log.print("(%d) - IPCServiceThread::ignite - b\n", (tuint64)(QThread::currentThreadId()));
	return (!m_handler.isNull()) ? true : false;
}

//-------------------------------------------------------------------------------------------

void IPCServiceThread::run()
{
	common::Log::g_Log.print("(%d) - IPCServiceThread::run - a\n", (tuint64)(QThread::currentThreadId()));
	QSharedPointer<IPCServiceHandler> handler(new IPCServiceHandler(m_serviceName));
	if(handler->startService())
	{
		m_handler = handler;
	}
	else
	{
		QString err = QString("Failed to start service '%1'").arg(m_serviceName);
		printError("run", err.toUtf8().constData());
	}

	QThread::yieldCurrentThread();
	m_semaphore.release(1);
	exec();	
	
	m_handler->stopService();
	m_handler.clear();
	common::Log::g_Log.print("(%d) - IPCServiceThread::run - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
// IPCServiceHandler
//-------------------------------------------------------------------------------------------

QMutex IPCServiceHandler::m_handlersMutex;
QMap<QString, IPCServiceThread *> IPCServiceHandler::m_handlers;

//-------------------------------------------------------------------------------------------

IPCServiceHandler::IPCServiceHandler(const QString& name, QObject *parent) : QObject(parent),
	m_serviceName(name),
	m_timer(0),
	m_pComms(),
	m_semaphore(0),
	m_responseArray(),
	m_isResponse(false)
{}

//-------------------------------------------------------------------------------------------

IPCServiceHandler::~IPCServiceHandler()
{}

//-------------------------------------------------------------------------------------------

void IPCServiceHandler::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "IPCServiceHandler::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<IPCServiceHandler> IPCServiceHandler::instance(const QString& name)
{
	QMap<QString, IPCServiceThread *>::iterator ppI;
	QSharedPointer<IPCServiceHandler> handler;
	
	common::Log::g_Log.print("(%d) - IPCServiceHandler::instance - a\n", (tuint64)(QThread::currentThreadId()));
	m_handlersMutex.lock();
	ppI = m_handlers.find(name);
	if(ppI != m_handlers.end())
	{
		handler = ppI.value()->handler();
	}
	m_handlersMutex.unlock();
	
	if(handler.isNull())
	{
		IPCServiceThread *thread = new IPCServiceThread(name);
		if(thread->ignite())
		{
			handler = thread->handler();
			m_handlersMutex.lock();
			m_handlers.insert(name, thread);
			m_handlersMutex.unlock();
		}
		else
		{
			delete thread;
		}
	}
	common::Log::g_Log.print("(%d) - IPCServiceHandler::instance - b\n", (tuint64)(QThread::currentThreadId()));
	return handler;
}

//-------------------------------------------------------------------------------------------

void IPCServiceHandler::release()
{
	IPCServiceThread *thread = 0;
	QMap<QString, IPCServiceThread *>::iterator ppI;
	
	common::Log::g_Log.print("(%d) - IPCServiceHandler::release - a\n", (tuint64)(QThread::currentThreadId()));
	m_handlersMutex.lock();
	ppI = m_handlers.find(m_serviceName);
	if(ppI != m_handlers.end())
	{
		thread = ppI.value();
		m_handlers.erase(ppI);
	}
	else
	{
		QString err = QString("Thread not found for service '%1'").arg(m_serviceName);
		printError("release", err.toUtf8().constData());
	}
	m_handlersMutex.unlock();
	
	if(thread != 0)
	{
		thread->quit();
		thread->wait();
		delete thread;
	}
	common::Log::g_Log.print("(%d) - IPCServiceHandler::release - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

bool IPCServiceHandler::startService()
{
	QString socketPath = pathToUNIXSocket(m_serviceName);
	QSharedPointer<IPCSocketComms> pComms(new IPCSocketComms(IPCSocketComms::e_Server));
	bool res = false;
	
	common::Log::g_Log.print("(%d) - IPCServiceHandler::startService - a\n", (tuint64)(QThread::currentThreadId()));
	if(pComms->open(socketPath))
	{
		common::TimeStamp timeOut(0.2);
		pComms->setReadTimeout(timeOut);
		timeOut = 1.0;
		pComms->setWriteTimeout(timeOut);
		m_pComms = pComms;
		
		m_timer = new QTimer(this);
		QObject::connect(m_timer, &QTimer::timeout, this, &IPCServiceHandler::onProcess);
		m_timer->start();
		
		res = true;
	}
	else
	{
		QString err = QString("Failed to create UNIX socket '%1'").arg(socketPath);
		printError("startService", err.toUtf8().constData());
	}
	common::Log::g_Log.print("(%d) - IPCServiceHandler::startService - b\n", (tuint64)(QThread::currentThreadId()));
	return res;	
}

//-------------------------------------------------------------------------------------------

void IPCServiceHandler::stopService()
{
	common::Log::g_Log.print("(%d) - IPCServiceHandler::stopService - a\n", (tuint64)(QThread::currentThreadId()));
	if(m_timer != 0)
	{
		m_timer->stop();
		delete m_timer;
	}
	
	if(!m_pComms.isNull())
	{
		m_pComms->close();
		m_pComms.clear();
	}
	common::Log::g_Log.print("(%d) - IPCServiceHandler::stopService - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void IPCServiceHandler::onProcess()
{
	QByteArray inArr;
	int res;
	
	common::Log::g_Log.print("(%d) - IPCServiceHandler::onProcess - a\n", (tuint64)(QThread::currentThreadId()));
	res = m_pComms->read(inArr);
	if(res > 0)
	{
		common::Log::g_Log << "server - " << QString::fromUtf8(inArr) << common::c_endl;
		
		m_mutex.lock();
		m_responseArray.clear();
		m_mutex.unlock();
		
		m_isResponse = false;
		emit onProcessRPC(inArr);
		writeResponse();
	}
	else if(res < 0)
	{
		printError("run", "Error reading from socket");
	}
	common::Log::g_Log.print("(%d) - IPCServiceHandler::onProcess - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void IPCServiceHandler::postResponse(const QByteArray& arr)
{
	common::Log::g_Log.print("(%d) - IPCServiceHandler::postResponse - a\n", (tuint64)(QThread::currentThreadId()));
	m_mutex.lock();
	if(!arr.isNull())
	{
		m_responseArray = arr;
	}
	else
	{
		m_responseArray.clear();
	}
	m_isResponse = true;
	m_mutex.unlock();
	m_semaphore.release(1);
	common::Log::g_Log.print("(%d) - IPCServiceHandler::postResponse - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void IPCServiceHandler::wakeupIfNoResponse()
{
	common::Log::g_Log.print("(%d) - IPCServiceHandler::wakeupIfNoResponse - a\n", (tuint64)(QThread::currentThreadId()));
	if(!m_isResponse)
	{
		m_isResponse = true;
		m_semaphore.release(1);
	}
	common::Log::g_Log.print("(%d) - IPCServiceHandler::wakeupIfNoResponse - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void IPCServiceHandler::writeResponse()
{
	common::Log::g_Log.print("(%d) - IPCServiceHandler::writeResponse - a\n", (tuint64)(QThread::currentThreadId()));
	m_semaphore.acquire(1);
	
	m_mutex.lock();
	if(!m_responseArray.isNull())
	{
		int res = m_pComms->write(m_responseArray);
		if(res != m_responseArray.size())
		{
			printError("writeResponse", "Error writing response to socket");
		}
	}
	m_responseArray.clear();
	m_mutex.unlock();
	common::Log::g_Log.print("(%d) - IPCServiceHandler::writeResponse - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
