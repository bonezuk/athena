//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_AUDIOIO_ABSTRACTAUDIOHARDWAREBUFFER_H
#define __ORCUS_AUDIOIO_ABSTRACTAUDIOHARDWAREBUFFER_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/FormatDescription.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT AbstractAudioHardwareBuffer
{
	public:
		AbstractAudioHardwareBuffer();
		virtual ~AbstractAudioHardwareBuffer();
		
		virtual tbyte *buffer(tint bufferIdx) = 0;
		
		virtual tint sampleSize(tint bufferIdx) = 0;
		
		virtual tint numberOfChannelsInBuffer(tint bufferIdx) = 0;
		
		virtual tint bufferLength() = 0;
		
		virtual tint numberOfBuffers() = 0;
};

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------