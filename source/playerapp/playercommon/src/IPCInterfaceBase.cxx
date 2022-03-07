#include "playerapp/playercommon/inc/IPCInterfaceBase.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

IPCInterfaceBase::IPCInterfaceBase(const QString& serviceName) : m_serviceName(serviceName),
	m_pIPCComms()
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

void IPCInterfaceBase::sendRPCCall(const QString& funcName)
{
	QVariantMap rpcMap;
	sendRPCCall(funcName, rpcMap);
}

//-------------------------------------------------------------------------------------------

void IPCInterfaceBase::sendRPCCall(const QString& funcName, QVariantMap& rpcMap)
{
	QSharedPointer<IPCSocketComms> pComms;
	
	pComms = getIPCComms();
	if(!pComms.isNull())
	{
		rpcMap.insert(QString::fromUtf8(OMEGA_IPC_FUNCTION_NAME), QVariant(funcName));
		QJsonObject rpcObject = QJsonObject::fromVariantMap(rpcMap);
		QJsonDocument rpcDocument(rpcObject);
	
		if(rpcDocument.isObject())
		{
			int res = pComms->write(rpcDocument.toJson(QJsonDocument::Compact));
			if(res <= 0)
			{
				if(res < 0)
				{
					printError("sendRPCCall", "Error writing RPC JSON request");
				}
				pComms->close();
				m_pIPCComms.clear();
			}
		}
		else
		{
			QString err = QString("Failed to create RPC JSON request for function '%1'").arg(funcName);
			printError("sendRPCCall", err.toUtf8().constData());
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
