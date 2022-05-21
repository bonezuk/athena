//-------------------------------------------------------------------------------------------
#if defined(OMEGA_IOS)
//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_AUDIOIO_AOCOREAUDIOIOS_H
#define __OMEGA_AUDIOIO_AOCOREAUDIOIOS_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOBase.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT AOCoreAudioIOS : public AOBase
{
	public:
		Q_OBJECT
		
	public:
		AOCoreAudioIOS(QObject *parent = 0);
		virtual ~AOCoreAudioIOS();
		
	protected:
		
		virtual void printError(const tchar *strR,const tchar *strE) const;
		
		virtual engine::AData *allocateData(tint len,tint inChannel,tint outChannel);
		
		virtual bool openAudio();
		virtual void closeAudio();
		virtual bool startAudioDevice();
		virtual void stopAudioDevice();
		virtual bool isAudio() const;

		virtual void processMessages();
		
		virtual void writeToAudioOutputBufferFromPartData(AbstractAudioHardwareBuffer *pBuffer,
                                                          const engine::RData *data,
                                                          tint partNumber,
                                                          tint inChannelIndex,
                                                          tint bufferIndex,
                                                          tint outChannelIndex,
                                                          tint inputSampleIndex,
                                                          tint outputSampleIndex,
                                                          tint amount);

		virtual QSharedPointer<AOQueryDevice::Device> copyDeviceInformation(const AOQueryDevice::Device& iDevice);

};

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
