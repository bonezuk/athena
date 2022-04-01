#include "playerapp/audiodaemon/inc/OmegaPLBusInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaPLBusInterface::OmegaPLBusInterface(QObject *parent) : OmegaPlaylistInterface(parent),
	IPCInterfaceBase(QString::fromUtf8(OMEGAPLAYLISTMANAGER_SERVICE_NAME))
{}

//-------------------------------------------------------------------------------------------

OmegaPLBusInterface::~OmegaPLBusInterface()
{}

//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaPLBusInterface::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------
// { "function": "playbackTime", "timestamp": 12.3456 }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::playbackTime(quint64 tS)
{
	QVariantMap rpcMap;
	common::TimeStamp tStamp(tS);
	rpcMap.insert("timestamp", QVariant(static_cast<tfloat64>(tStamp)));
	sendRPCCall("playbackTime", rpcMap);
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioStart", "name": "audio_filename.mp3" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioStart(const QString& name)
{
	QVariantMap rpcMap;
	rpcMap.insert("name", QVariant(name));
	sendRPCCall("onAudioStart", rpcMap);
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioPlay" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioPlay()
{
	sendRPCCall("onAudioPlay");
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioPause" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioPause()
{
	sendRPCCall("onAudioPause");
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioStop" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioStop()
{
	sendRPCCall("onAudioStop");
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioBuffer", "percent": 12.3456 }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioBuffer(tfloat32 percent)
{
	QVariantMap rpcMap;
	rpcMap.insert("percent", QVariant(percent));
	sendRPCCall("onAudioBuffer", rpcMap);
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioReadyForNext" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioReadyForNext()
{
	sendRPCCall("onAudioReadyForNext");
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioNoNext" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioNoNext()
{
	sendRPCCall("onAudioNoNext");
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioCrossfade" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioCrossfade()
{
	sendRPCCall("onAudioCrossfade");
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
