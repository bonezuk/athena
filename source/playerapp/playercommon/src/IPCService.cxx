#include "playerapp/playercommon/inc/IPCService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

IPCService::IPCService(const char *serviceName, QObject *parent) : QObject(parent),
	m_serviceName(QString::fromLatin1(serviceName)),
	m_pServiceThread(0)
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
	
	stop();
	
	m_pServiceThread = new IPCServiceThread(m_serviceName);
	QObject::connect(m_pServiceThread, SIGNAL(onProcessRPC(QByteArray)), this, SLOT(processRPC(QByteArray)), Qt::BlockingQueuedConnection);
	if(m_pServiceThread->startService())
	{
		res = true;
	}
	else
	{
		printError("start", "Failed to start RPC playlist service");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void IPCService::stop()
{
	if(m_pServiceThread != 0)
	{
		QObject::disconnect(m_pServiceThread, SIGNAL(onProcessRPC(QByteArray)), this, SLOT(processRPC(QByteArray)));
		m_pServiceThread->stopService();
		delete m_pServiceThread;
		m_pServiceThread = 0;
	}
}
		
//-------------------------------------------------------------------------------------------

void IPCService::processRPC(QByteArray rpcArray)
{
	common::Log::g_Log << "processRPC - a" << common::c_endl;

	QJsonParseError jsonErr;
	QJsonDocument doc = QJsonDocument::fromJson(rpcArray, &jsonErr);
	if(!doc.isNull())
	{
		common::Log::g_Log << QString::fromUtf8(rpcArray) << common::c_endl;
		handleRPCJson(doc);
	}
	else
	{
		QString err = QString("Error parsing expected JSON object. %1").arg(jsonErr.errorString());
		printError("processRPC", err.toUtf8().constData());
	}

	common::Log::g_Log << "processRPC - b" << common::c_endl;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
