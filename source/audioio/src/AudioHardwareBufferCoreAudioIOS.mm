//-------------------------------------------------------------------------------------------
#if defined(OMEGA_IOS)
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AudioHardwareBufferCoreAudioIOS.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

AudioHardwareBufferCoreAudioIOS::AudioHardwareBufferCoreAudioIOS() : AbstractAudioHardwareBuffer()
{}

//-------------------------------------------------------------------------------------------

AudioHardwareBufferCoreAudioIOS::~AudioHardwareBufferCoreAudioIOS()
{}

//-------------------------------------------------------------------------------------------

tbyte *AudioHardwareBufferCoreAudioIOS::buffer(tint bufferIdx)
{
	return 0;
}

//-------------------------------------------------------------------------------------------

tint AudioHardwareBufferCoreAudioIOS::sampleSize(tint bufferIdx)
{
	return 0;
}

//-------------------------------------------------------------------------------------------

tint AudioHardwareBufferCoreAudioIOS::numberOfChannelsInBuffer(tint bufferIdx)
{
	return 0;
}

//-------------------------------------------------------------------------------------------

tint AudioHardwareBufferCoreAudioIOS::bufferLength()
{
	return 0;
}

//-------------------------------------------------------------------------------------------

tint AudioHardwareBufferCoreAudioIOS::numberOfBuffers()
{
	return 0;
}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
