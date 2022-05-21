//-------------------------------------------------------------------------------------------
#if defined(OMEGA_IOS)
//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_AUDIOIO_AUDIOHARDWAREBUFFERCOREAUDIO_H
#define __OMEGA_AUDIOIO_AUDIOHARDWAREBUFFERCOREAUDIO_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOQueryCoreAudioIOS.h"
#include "audioio/inc/AbstractAudioHardwareBuffer.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT AudioHardwareBufferCoreAudioIOS : public AbstractAudioHardwareBuffer
{
	public:
		AudioHardwareBufferCoreAudioIOS();
		virtual ~AudioHardwareBufferCoreAudioIOS();
		
		virtual tbyte *buffer(tint bufferIdx);
		
		virtual tint sampleSize(tint bufferIdx);
		
		virtual tint numberOfChannelsInBuffer(tint bufferIdx);
		
		virtual tint bufferLength();
		
		virtual tint numberOfBuffers();
		
	protected:
};

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
