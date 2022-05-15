#include "playerapp/audiodaemon/inc/OmegaPLBusInterface.h"

//-------------------------------------------------------------------------------------------
namespace omega
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
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::playbackTime - a\n", (tuint64)(QThread::currentThreadId()));
	QVariantMap rpcMap;
	common::TimeStamp tStamp(tS);
	rpcMap.insert("timestamp", QVariant(static_cast<tfloat64>(tStamp)));
	sendRPCCall("playbackTime", rpcMap);
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::playbackTime - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioStart", "name": "audio_filename.mp3" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioStart(const QString& name)
{
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioStart - a\n", (tuint64)(QThread::currentThreadId()));
	QVariantMap rpcMap;
	rpcMap.insert("name", QVariant(name));
	sendRPCCall("onAudioStart", rpcMap);
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioStart - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioPlay" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioPlay()
{
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioPlay - a\n", (tuint64)(QThread::currentThreadId()));
	sendRPCCall("onAudioPlay");
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioPlay - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioPause" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioPause()
{
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioPause - a\n", (tuint64)(QThread::currentThreadId()));
	sendRPCCall("onAudioPause");
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioPause - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioStop" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioStop()
{
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioStop - a\n", (tuint64)(QThread::currentThreadId()));
	sendRPCCall("onAudioStop");
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioStop - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioBuffer", "percent": 12.3456 }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioBuffer(tfloat32 percent)
{
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioBuffer - a\n", (tuint64)(QThread::currentThreadId()));
	QVariantMap rpcMap;
	rpcMap.insert("percent", QVariant(percent));
	sendRPCCall("onAudioBuffer", rpcMap);
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioBuffer - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioReadyForNext" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioReadyForNext()
{
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioReadyForNext - a\n", (tuint64)(QThread::currentThreadId()));
	sendRPCCall("onAudioReadyForNext");
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioReadyForNext - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioNoNext" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioNoNext()
{
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioNoNext - a\n", (tuint64)(QThread::currentThreadId()));
	sendRPCCall("onAudioNoNext");
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioNoNext - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
// { "function": "onAudioCrossfade" }
//-------------------------------------------------------------------------------------------

void OmegaPLBusInterface::onAudioCrossfade()
{
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioCrossfade - a\n", (tuint64)(QThread::currentThreadId()));
	sendRPCCall("onAudioCrossfade");
	common::Log::g_Log.print("(%d) - OmegaPLBusInterface::onAudioCrossfade - b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
