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
	return res;
}

//-------------------------------------------------------------------------------------------

void IPCService::stop()
{
	if(!m_pServiceHandler.isNull())
	{
		QObject::disconnect(m_pServiceHandler.get(), SIGNAL(onProcessRPC(QByteArray)), this, SLOT(processRPC(QByteArray)));
		m_pServiceHandler->release();
		m_pServiceHandler.clear();
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
		m_pServiceHandler->wakeupIfNoResponse();
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
