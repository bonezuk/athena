#include "playerapp/webservice/inc/OmegaWebEventService.h"
#include <QJsonValue>
#include <QJsonArray>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaWebEventService::OmegaWebEventService(QObject *parent) : IPCService(OMEGAWEBEVENT_SERVICE_NAME, parent),
	WebEventRegisterInterface(),
	m_broker()
{}

//-------------------------------------------------------------------------------------------

OmegaWebEventService::~OmegaWebEventService()
{}

//-------------------------------------------------------------------------------------------

void OmegaWebEventService::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaWebEventService::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool OmegaWebEventService::registerConnection(network::http::HTTPReceive *receive)
{
	return m_broker.registerConnection(receive);
}

//-------------------------------------------------------------------------------------------
//	{
//		"event": "eventName",
//		"data": {
// 			"data": "aboutTheEvent"
//		}
//	}
//-------------------------------------------------------------------------------------------

void OmegaWebEventService::handleRPCJson(const QJsonDocument& doc)
{
	if(!doc.isNull() && doc.isObject())
	{
		QJsonObject root = doc.object();
		
		if(root.contains("event"))
		{
			QJsonValue e = root.value("event");
			if(e.isString())
			{
				m_broker.postEvent(doc);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
