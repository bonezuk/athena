#include "playerapp/webservice/inc/OmegaPLWebInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaPLWebInterface::OmegaPLWebInterface() : IPCInterfaceBase(QString::fromLatin1(OMEGAPLWEB_SERVICE_NAME))
{}

//-------------------------------------------------------------------------------------------

OmegaPLWebInterface::~OmegaPLWebInterface()
{}

//-------------------------------------------------------------------------------------------

void OmegaPLWebInterface::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaPLWebInterface::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------
// { "function": "getFullPlaylist" }
//-------------------------------------------------------------------------------------------

QJsonDocument OmegaPLWebInterface::getFullPlaylist()
{
	QJsonDocument doc;
	
	if(sendRPCCall("getFullPlaylist"))
	{
		doc = receiveJsonReply();
	}
	return doc;
}

//-------------------------------------------------------------------------------------------
// { "function": "getPlaybackState" }
//-------------------------------------------------------------------------------------------

QJsonDocument OmegaPLWebInterface::getPlaybackState()
{
	QJsonDocument doc;
	
	if(sendRPCCall("getPlaybackState"))
	{
		doc = receiveJsonReply();
	}
	return doc;
}

//-------------------------------------------------------------------------------------------
// { "function": "onPressPlay" }
//-------------------------------------------------------------------------------------------

void OmegaPLWebInterface::onPressPlay()
{
	sendRPCCall("onPressPlay");
}

//-------------------------------------------------------------------------------------------
// { "function": "onStartPlaying", "id": 12345 }
//-------------------------------------------------------------------------------------------

void OmegaPLWebInterface::onStartPlaying(tuint64 id)
{
	QVariantMap rpcMap;
	QString idStr = QString::number(id);
	rpcMap.insert("id", QVariant(idStr));
	sendRPCCall("onStartPlaying", rpcMap);
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
