#include "playerapp/playlistmanager/inc/WebEventBusInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
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

QJsonDocument WebEventBusInterface::createJsonEvent(const QString& eventName, const QJsonObject& data)
{
	QJsonObject eventJ;
	QJsonDocument doc;
	
	QJsonValue eventNameJ(eventName);
	eventJ.insert("event", eventNameJ);
	eventJ.insert("data", data);
	doc.setObject(eventJ);
	return doc;
}

//-------------------------------------------------------------------------------------------
//	{
//		"event": "onPlaybackTime",
//		"data": {
// 			"time": 12.345
//		}
//	}
//-------------------------------------------------------------------------------------------

void WebEventBusInterface::onPlaybackTime(const common::TimeStamp& tS)
{
	QJsonObject dataJ;
	QJsonValue timeData(static_cast<tfloat64>(tS));
	dataJ.insert("time", timeData);
	QJsonDocument doc = createJsonEvent("onPlaybackTime", dataJ);
	sendEvent(doc);
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
