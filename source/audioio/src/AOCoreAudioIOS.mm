//-------------------------------------------------------------------------------------------
#if defined(OMEGA_IOS)
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOCoreAudioIOS.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

AOCoreAudioIOS::AOCoreAudioIOS(QObject *parent) : AOBase(parent)
{}

//-------------------------------------------------------------------------------------------

AOCoreAudioIOS::~AOCoreAudioIOS()
{}

//-------------------------------------------------------------------------------------------

void AOCoreAudioIOS::printError(const tchar *strR,const tchar *strE) const
{
	common::Log::g_Log << "AOCoreAudioIOS::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

engine::AData *AOCoreAudioIOS::allocateData(tint len,tint inChannel,tint outChannel)
{
	return 0;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioIOS::openAudio()
{
	return false;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioIOS::closeAudio()
{}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioIOS::startAudioDevice()
{
	return false;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioIOS::stopAudioDevice()
{}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioIOS::isAudio() const
{
	return false;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioIOS::processMessages()
{}

//-------------------------------------------------------------------------------------------

void AOCoreAudioIOS::writeToAudioOutputBufferFromPartData(AbstractAudioHardwareBuffer *pBuffer,
                                                          const engine::RData *data,
                                                          tint partNumber,
                                                          tint inChannelIndex,
                                                          tint bufferIndex,
                                                          tint outChannelIndex,
                                                          tint inputSampleIndex,
                                                          tint outputSampleIndex,
                                                          tint amount)
{}

//-------------------------------------------------------------------------------------------

QSharedPointer<AOQueryDevice::Device> AOCoreAudioIOS::copyDeviceInformation(const AOQueryDevice::Device& iDevice)
{
	return QSharedPointer<AOQueryDevice::Device>();
}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
