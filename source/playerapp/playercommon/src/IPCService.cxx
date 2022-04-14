#include "playerapp/playercommon/inc/IPCService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

IPCService::IPCService(const char *serviceName, QObject *parent) : QObject(parent),
	m_serviceName(QString::fromLatin1(serviceName)),
	m_pServiceHandler()
{}

//-------------------------------------------------------------------------------------------

IPCService::~IPCService()
{
	IPCService::stop();
}

//-------------------------------------------------------------------------------------------

void IPCService::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "IPCSevice::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool IPCService::start()
{
	bool res = false;
	
	common::Log::g_Log.print("(%d) - IPCService::start - a\n", (tuint64)(QThread::currentThreadId()));
	
	stop();
	
	m_pServiceHandler = IPCServiceHandler::instance(m_serviceName);
	if(!m_pServiceHandler.isNull())
	{
		QObject::connect(m_pServiceHandler.get(), SIGNAL(onProcessRPC(QByteArray)), this, SLOT(processRPC(QByteArray)));
		res = true;
	}
	else
	{
		printError("start", "Failed to start RPC playlist service");
	}
	
	common::Log::g_Log.print("(%d) - IPCService::start - b\n", (tuint64)(QThread::currentThreadId()));
	
	return res;
}

//-------------------------------------------------------------------------------------------

void IPCService::stop()
{
	common::Log::g_Log.print("(%d) - IPCService::stop - a\n", (tuint64)(QThread::currentThreadId()));
	
	if(!m_pServiceHandler.isNull())
	{
		QObject::disconnect(m_pServiceHandler.get(), SIGNAL(onProcessRPC(QByteArray)), this, SLOT(processRPC(QByteArray)));
		m_pServiceHandler->release();
		m_pServiceHandler.clear();
	}
	common::Log::g_Log.print("(%d) - IPCService::stop - b\n", (tuint64)(QThread::currentThreadId()));
}
		
//-------------------------------------------------------------------------------------------

void IPCService::processRPC(QByteArray rpcArray)
{
	common::Log::g_Log.print("(%d) - IPCService::processRPC - a\n", (tuint64)(QThread::currentThreadId()));

	QJsonParseError jsonErr;
	QJsonDocument doc = QJsonDocument::fromJson(rpcArray, &jsonErr);
	if(!doc.isNull())
	{
		common::Log::g_Log << QString::fromUtf8(rpcArray) << common::c_endl;
		handleRPCJson(doc);
		m_pServiceHandler->wakeupIfNoResponse();
	}
	else
	{
		QString err = QString("Error parsing expected JSON object. %1").arg(jsonErr.errorString());
		printError("processRPC", err.toUtf8().constData());
	}
	
	common::Log::g_Log.print("(%d) - IPCService::processRPC - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
