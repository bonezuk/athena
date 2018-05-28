//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_AUDIOIO_AUDIOHARDWAREBUFFERWASAPI_H
#define __ORCUS_AUDIOIO_AUDIOHARDWAREBUFFERWASAPI_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/WasAPILayerIF.h"
#include "audioio/inc/AbstractAudioHardwareBuffer.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT AudioHardwareBufferWasAPI : public AbstractAudioHardwareBuffer
{
	public:
		AudioHardwareBufferWasAPI(WAVEFORMATEX *pFormat,tbyte *pBuffer,tint numberOfFrames);
		virtual ~AudioHardwareBufferWasAPI();
		
		virtual tbyte *buffer(tint bufferIdx);
		
		virtual tint sampleSize(tint bufferIdx);
		
		virtual tint numberOfChannelsInBuffer(tint bufferIdx);
		
		virtual tint bufferLength();
		
		virtual tint numberOfBuffers();
		
	protected:
	
		WAVEFORMATEX *m_pFormat;
		tbyte *m_pBuffer;
		tint m_numberOfFrames;
};

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

