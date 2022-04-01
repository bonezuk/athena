#include "playerapp/playlistmanager/inc/OmegaAudioBusInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaAudioBusInterface::OmegaAudioBusInterface(QObject *parent)  : OmegaAudioInterface(parent),
	IPCInterfaceBase(QString::fromUtf8(OMEGAAUDIOINTERFACE_SERVICE_NAME))
{}

//-------------------------------------------------------------------------------------------

OmegaAudioBusInterface::~OmegaAudioBusInterface()
{}

//-------------------------------------------------------------------------------------------

void OmegaAudioBusInterface::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaAudioBusInterface::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------
// { "function": "playFile", "fileName": "/Music/file_to_play.wav", "isNext": true }
//-------------------------------------------------------------------------------------------

void OmegaAudioBusInterface::playFile(const QString& fileName, bool isNext)
{
	QVariantMap rpcMap;
	rpcMap.insert(QString::fromUtf8("fileName"), QVariant(fileName));
	rpcMap.insert(QString::fromUtf8("isNext"), QVariant(isNext));
	sendRPCCall("playFile", rpcMap);
}

//-------------------------------------------------------------------------------------------
// // { "function": "playFileWithTime", "fileName": "/Music/file_to_play.wav", "isNext": true, start: 30.01, length: 40.5}
//-------------------------------------------------------------------------------------------

void OmegaAudioBusInterface::playFileWithTime(const QString& fileName, const common::TimeStamp& start,const common::TimeStamp& length, bool isNext)
{
	QVariantMap rpcMap;
	rpcMap.insert(QString::fromUtf8("fileName"), QVariant(fileName));
	rpcMap.insert(QString::fromUtf8("isNext"), QVariant(isNext));
	rpcMap.insert(QString::fromUtf8("start"), QVariant(static_cast<tfloat64>(start)));
	rpcMap.insert(QString::fromUtf8("length"), QVariant(static_cast<tfloat64>(length)));
	sendRPCCall("playFileWithTime", rpcMap);
}

//-------------------------------------------------------------------------------------------
// { "function": "play" }
//-------------------------------------------------------------------------------------------

void OmegaAudioBusInterface::play()
{
	sendRPCCall("play");
}

//-------------------------------------------------------------------------------------------
// { "function": "pause" }
//-------------------------------------------------------------------------------------------

void OmegaAudioBusInterface::pause()
{
	sendRPCCall("pause");
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
