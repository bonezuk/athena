#include "playerapp/playlistmanager/inc/WebEventBusInterface.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

WebEventBusInterface::WebEventBusInterface() : IPCInterfaceBase(QString::fromUtf8(OMEGAWEBEVENT_SERVICE_NAME))
{}

//-------------------------------------------------------------------------------------------

WebEventBusInterface::~WebEventBusInterface()
{}

//-------------------------------------------------------------------------------------------

void WebEventBusInterface::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "WebEventBusInterface::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void WebEventBusInterface::sendEvent(const QJsonDocument& doc)
{
	QSharedPointer<IPCSocketComms> pComms;
	
	pComms = getIPCComms();
	if(!pComms.isNull())
	{
		int res = pComms->write(doc.toJson(QJsonDocument::Compact));
		if(res <= 0)
		{
			if(res < 0)
			{
				printError("sendEvent", "Error writing JSON event to IPC socket");
			}
			pComms->close();
			m_pIPCComms.clear();
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
