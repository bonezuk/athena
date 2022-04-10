#include "playerapp/playercommon/inc/IPCServiceThread.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

IPCServiceThread::IPCServiceThread(const QString& serviceName, QObject *parent) : QThread(parent),
	m_serviceName(serviceName),
	m_pComms(),
	m_running(false),
	m_responseMutex(),
	m_responseList()
{}

//-------------------------------------------------------------------------------------------

IPCServiceThread::~IPCServiceThread()
{}

//-------------------------------------------------------------------------------------------

void IPCServiceThread::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "IPCServiceThread::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool IPCServiceThread::startService()
{
	QString socketPath = pathToUNIXSocket(m_serviceName);
	QSharedPointer<IPCSocketComms> pComms(new IPCSocketComms(IPCSocketComms::e_Server));
	bool res = false;
	
	common::Log::g_Log << "Start service" << common::c_endl;
	if(pComms->open(socketPath))
	{
		common::TimeStamp timeOut(0.2);
		pComms->setReadTimeout(timeOut);
		timeOut = 1.0;
		pComms->setWriteTimeout(timeOut);
		m_pComms = pComms;
		m_running = true;
		start();
		res = true;
	}
	else
	{
		QString err = QString("Failed to create UNIX socket '%1'").arg(socketPath);
		printError("startService", err.toUtf8().constData());
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void IPCServiceThread::stopService()
{
	common::Log::g_Log << "Stop service" << common::c_endl;
	m_running = false;
	while(isRunning())
	{
		QThread::msleep(100);
	}
	
	if(!m_pComms.isNull())
	{
		m_pComms->close();
		m_pComms.clear();
	}
}

//-------------------------------------------------------------------------------------------

void IPCServiceThread::run()
{
	int res;
	QByteArray inArr;
	
	common::Log::g_Log << "Service thread started" << common::c_endl;
	while(m_running)
	{
		res = m_pComms->read(inArr);
		if(res > 0)
		{
			common::Log::g_Log << "server - " << QString::fromUtf8(inArr) << common::c_endl;
			
			emit onProcessRPC(inArr);
			writeResponse();
		}
		else if(res < 0)
		{
			printError("run", "Error reading from socket");
		}
		common::Log::g_Log << "Service tick" << common::c_endl;
	}
}

//-------------------------------------------------------------------------------------------

void IPCServiceThread::postResponse(const QByteArray& arr)
{
	m_responseMutex.lock();
	m_responseList.append(arr);
	m_responseMutex.unlock();
}

//-------------------------------------------------------------------------------------------

void IPCServiceThread::writeResponse()
{
	QList<QByteArray> wList;
	int res;
	
	m_responseMutex.lock();
	while(!m_responseList.isEmpty())
	{
		QByteArray arr = m_responseList.takeFirst();
		wList.append(arr);
	}
	m_responseMutex.unlock();

	while(!wList.isEmpty())
	{
		QByteArray arr = wList.takeFirst();
		res = m_pComms->write(arr);
		if(res != arr.size())
		{
			printError("writeResponse", "Error writing response to socket");
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
