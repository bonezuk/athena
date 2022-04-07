#include "playerapp/playercommon/inc/IPCInterfaceBase.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

IPCInterfaceBase::IPCInterfaceBase(const QString& serviceName) : m_serviceName(serviceName),
	m_pIPCComms(),
	m_isTimeout(false)
{}

//-------------------------------------------------------------------------------------------

IPCInterfaceBase::~IPCInterfaceBase()
{
	if(!m_pIPCComms.isNull())
	{
		m_pIPCComms->close();
		m_pIPCComms.clear();
	}
}

//-------------------------------------------------------------------------------------------

void IPCInterfaceBase::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "IPCInterfaceBase::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<IPCSocketComms> IPCInterfaceBase::getIPCComms()
{
	if(m_pIPCComms.isNull())
	{
		QString socketPath = pathToUNIXSocket(m_serviceName);
		if(common::DiskOps::exist(socketPath))
		{
			QSharedPointer<IPCSocketComms> pComms(new IPCSocketComms(IPCSocketComms::e_Client));
			if(pComms->open(socketPath))
			{
				pComms->setNoTimeout(m_isTimeout);
				m_pIPCComms = pComms;
			}
			else
			{
				QString err = QString("Error opening expected socket '%1'").arg(socketPath);
				printError("getIPCComms", err.toUtf8().constData());
			}
		}
	}
	return m_pIPCComms;
}

//-------------------------------------------------------------------------------------------

void IPCInterfaceBase::setNoTimeout(bool isTimeout)
{
	m_isTimeout = isTimeout;
	if(!m_pIPCComms.isNull())
	{
		m_pIPCComms->setNoTimeout(isTimeout);
	}
}

//-------------------------------------------------------------------------------------------

bool IPCInterfaceBase::sendRPCCall(const QString& funcName)
{
	QVariantMap rpcMap;
	return sendRPCCall(funcName, rpcMap);
}

//-------------------------------------------------------------------------------------------

bool IPCInterfaceBase::sendRPCCall(const QString& funcName, QVariantMap& rpcMap)
{
	QSharedPointer<IPCSocketComms> pComms;
	bool isSent = false;
	
	pComms = getIPCComms();
	if(!pComms.isNull())
	{
		rpcMap.insert(QString::fromUtf8(OMEGA_IPC_FUNCTION_NAME), QVariant(funcName));
		QJsonObject rpcObject = QJsonObject::fromVariantMap(rpcMap);
		QJsonDocument rpcDocument(rpcObject);
	
		if(rpcDocument.isObject())
		{
			QByteArray arr = rpcDocument.toJson(QJsonDocument::Compact);
			
			common::Log::g_Log << "client - "<< QString::fromUtf8(arr) << common::c_endl;
			
			int res = pComms->write(arr);
			
			if(res <= 0)
			{
				if(res < 0)
				{
					printError("sendRPCCall", "Error writing RPC JSON request");
				}
				pComms->close();
				m_pIPCComms.clear();
			}
			else
			{
				isSent = true;
			}
		}
		else
		{
			QString err = QString("Failed to create RPC JSON request for function '%1'").arg(funcName);
			printError("sendRPCCall", err.toUtf8().constData());
		}
	}
	else
	{
		printError("sendRPCCall", "No IPC Comms socker");
	}
	return isSent;
}

//-------------------------------------------------------------------------------------------

QJsonDocument IPCInterfaceBase::receiveJsonReply()
{
	QJsonDocument doc;
	QSharedPointer<IPCSocketComms> pComms;
	
	pComms = getIPCComms();
	if(!pComms.isNull())
	{
		int res;
		QByteArray arr;
		
		res = pComms->read(arr);
		if(res > 0)
		{
			QJsonParseError jError;
			
			doc = QJsonDocument::fromJson(arr, &jError);
			if(doc.isNull())
			{
				QString err = QString("Error parsing Json data. %1").arg(jError.errorString());
				printError("receiveJsonReply", err.toUtf8().constData());
			}
		}
		else if(!res)
		{
			printError("receiveJsonReply", "Timeout waiting to receive Json data");
		}
		else
		{
			printError("receiveJsonReply", "Error reading IPC data");
		}
	}
	else
	{
		printError("receiveJsonReply", "Cannot get IPC socket interface");
	}
	return doc;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
