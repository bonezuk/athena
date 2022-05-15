#include "playerapp/webservice/inc/OmegaPLWebInterfaceWS.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

OmegaPLWebInterfaceWS::OmegaPLWebInterfaceWS() : IPCInterfaceBase(QString::fromLatin1(OMEGAPLWEB_SERVICE_NAME)),
	OmegaPLWebInterface()
{}

//-------------------------------------------------------------------------------------------

OmegaPLWebInterfaceWS::~OmegaPLWebInterfaceWS()
{}

//-------------------------------------------------------------------------------------------

void OmegaPLWebInterfaceWS::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaPLWebInterfaceWS::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------
// { "function": "getFullPlaylist" }
//-------------------------------------------------------------------------------------------

QJsonDocument OmegaPLWebInterfaceWS::getFullPlaylist()
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

QJsonDocument OmegaPLWebInterfaceWS::getPlaybackState()
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

void OmegaPLWebInterfaceWS::onPressPlay()
{
	sendRPCCall("onPressPlay");
}

//-------------------------------------------------------------------------------------------
// { "function": "onStartPlaying", "id": 12345 }
//-------------------------------------------------------------------------------------------

void OmegaPLWebInterfaceWS::onStartPlaying(tuint64 id)
{
	QVariantMap rpcMap;
	QString idStr = QString::number(id);
	rpcMap.insert("id", QVariant(idStr));
	sendRPCCall("onStartPlaying", rpcMap);
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
