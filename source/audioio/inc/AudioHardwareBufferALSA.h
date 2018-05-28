//-------------------------------------------------------------------------------------------
#if defined(OMEGA_LINUX)
//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_AUDIOIO_AUDIOHARDWAREBUFFERALSA_H
#define __ORCUS_AUDIOIO_AUDIOHARDWAREBUFFERALSA_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOQueryALSA.h"
#include "audioio/inc/AbstractAudioHardwareBuffer.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AudioHardwareBufferALSA : public AbstractAudioHardwareBuffer
{
	public:
		AudioHardwareBufferALSA(tint formatType,tint noFrames,tint noChannels);
		virtual ~AudioHardwareBufferALSA();
		
		virtual tbyte *buffer(tint bufferIdx);
		
		virtual tint sampleSize(tint bufferIdx);
		
		virtual tint numberOfChannelsInBuffer(tint bufferIdx);
		
		virtual tint bufferLength();
		
		virtual tint numberOfBuffers();
	
		virtual tint numberOfBytesInBuffer();
	
	protected:
	
		tint m_formatType;
		tint m_noFrames;
		tint m_noChannels;
		tbyte *m_buffer;
};

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
