//-------------------------------------------------------------------------------------------
#if defined(OMEGA_IOS)
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOCoreAudioIOS.h"
#include "audioio/inc/AOCoreAudioSessionIOS.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(AOFactory,AOBase, \
							AOCoreAudioIOSFactory,AOCoreAudioIOS, \
                            "ios",false)

//-------------------------------------------------------------------------------------------

AOCoreAudioIOS::AOCoreAudioIOS(QObject *parent) : AOCoreAudio(parent),
	m_audioOutputUnit(0),
	m_pSampleConverter(),
	m_flagInit(false),
	m_flagStart(false)
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

void AOCoreAudioIOS::printError(const tchar *strR, const tchar *strE, OSStatus err) const
{
	NSError *error = [NSError errorWithDomain:NSOSStatusErrorDomain code:err userInfo:nil];
	QString desc = QString::fromUtf8(error.localizedDescription.UTF8String);
	QString reason = QString::fromUtf8(error.localizedFailureReason.UTF8String);
	QString recovery = QString::fromUtf8(error.localizedRecoverySuggestion.UTF8String);
	QString eStr;
	eStr  = strE;
	eStr += common::c_endl;
	eStr += "\tDescription - " + desc;
	eStr += common::c_endl;
	eStr += "\tReason - " + reason;
	eStr += common::c_endl;
	eStr += "\tSuggestion - " + recovery + common::c_endl;
	common::Log::g_Log << "AOQueryCoreAudio::" << strR << " - " << eStr.toUtf8().constData();
}

//-------------------------------------------------------------------------------------------

engine::AData *AOCoreAudioIOS::allocateData(tint len,tint inChannel,tint outChannel)
{
	engine::RData *d = new engine::RData(len,inChannel,outChannel);
	return reinterpret_cast<engine::AData *>(d);
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioIOS::getStreamDescription(const FormatDescription& desc, AudioStreamBasicDescription *fmt)
{
	tint bytesPerSample = 0;
	
	::memset(fmt, 0, sizeof(AudioStreamBasicDescription));
	
	if(desc.typeOfData() == FormatDescription::e_DataFloatSingle)
	{
		fmt->mFormatFlags = kAudioFormatFlagsNativeFloatPacked;
		bytesPerSample = 4;
	}
	else if(desc.typeOfData() == FormatDescription::e_DataSignedInteger)
	{
		if(desc.bits() == 16)
		{
			fmt->mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
			bytesPerSample = 2;
		}
		else if(desc.bits() == 24)
		{
			fmt->mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
			bytesPerSample = 3;
		}
		else if(desc.bits() == 32)
		{
			fmt->mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
			bytesPerSample = 4;
		}
		else
		{
			QString err = QString("Audio bit depth of %1 bits is not supported").arg(desc.bits());
			printError("getStreamDescription", err.toUtf8().constData());
			return false;
		}
	}
	else
	{
		printError("getStreamDescription", "Unsupported format description");
		return false;
	}
	
	fmt->mSampleRate = static_cast<Float64>(desc.frequency());
	fmt->mFormatID = kAudioFormatLinearPCM;
	fmt->mFramesPerPacket = 1;
	fmt->mBitsPerChannel = bytesPerSample * 8;
	fmt->mBytesPerPacket = bytesPerSample * desc.channels();
	fmt->mBytesPerFrame = bytesPerSample * desc.channels();
	fmt->mChannelsPerFrame = desc.channels();
	
	return true;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioIOS::setPlaybackFrequency()
{
	bool res = false;
	QSharedPointer<AOCoreAudioSessionIOS> pSession = AOCoreAudioSessionIOS::audioInstance();
	
	if(!pSession.isNull())
	{
		int playbackFrequency = pSession->startPlaybackWithFrequency(m_frequency);
		if(playbackFrequency > 0)
		{
			if(playbackFrequency != m_frequency)
			{
				initResampler(m_frequency, playbackFrequency);
				m_frequency = playbackFrequency;
			}
			res = true;
		}
		else
		{
			printError("setPlaybackFrequency", "Failed to get actual playback frequency");
		}
	}
	else
	{
		printError("setPlaybackFrequency", "Failed to get audio session");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioIOS::openAudio()
{
	OSStatus err;
	AudioComponent comp;
	AudioComponentDescription ioUnitDescription;
	QSharedPointer<AOQueryCoreAudioIOS::IOSDevice> pDevice;
	bool res = false;

	closeAudio();
	m_frequency = m_codec->frequency();
	
	if(mach_timebase_info(&m_machTimeInfo) != KERN_SUCCESS)
	{
		printError("openAudio", "Failed to get time information from system");
		return false;
	}

	pDevice = getCurrentCoreAudioIOSDevice();
	if(!pDevice.isNull())
	{
		::memset(&ioUnitDescription, 0, sizeof(AudioComponentDescription));
		ioUnitDescription.componentType = kAudioUnitType_Output;
		ioUnitDescription.componentSubType = kAudioUnitSubType_RemoteIO;
		ioUnitDescription.componentManufacturer = kAudioUnitManufacturer_Apple;
	
		comp = AudioComponentFindNext(0, &ioUnitDescription);
		if(comp != nil)
		{
			FormatDescription closestDescription;
			FormatDescription sourceDescription = getSourceDescription(pDevice->noChannels());

			if(FormatDescriptionUtils::findClosestFormatType(sourceDescription, pDevice->formats(), closestDescription))
			{
				AudioStreamBasicDescription streamFormat;
				
				if(getStreamDescription(closestDescription, &streamFormat))
				{
					err = AudioComponentInstanceNew(comp, &m_audioOutputUnit);
					if(err == noErr)
					{
						UInt32 enableIO = 1;
					
						err = AudioUnitSetProperty(m_audioOutputUnit, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Output, 0, &enableIO, sizeof(enableIO));
						if(err == noErr)
						{
							if(setPlaybackFrequency())
							{
								initCyclicBuffer();
							
								err = AudioUnitSetProperty(m_audioOutputUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &streamFormat, sizeof(AudioStreamBasicDescription));
								if(err == noErr)
								{
									AURenderCallbackStruct renderCallback;

									::memset(&renderCallback,0,sizeof(AURenderCallbackStruct));
									renderCallback.inputProc = iosAudioRender;
									renderCallback.inputProcRefCon = (void *)(this);
								
									err = AudioUnitSetProperty(m_audioOutputUnit, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Global, 0, &renderCallback, sizeof(renderCallback));
									if(err == noErr)
									{
										err = AudioUnitInitialize(m_audioOutputUnit);
										if(err == noErr)
										{
											m_pSampleConverter = sampleConverterFromDescription(streamFormat);
											m_noChannels = closestDescription.channels();
											m_flagInit = true;
											res = true;
										}
										else
										{
											printError("openAudio", "Failed to initialise audio output device", err);
											AudioUnitUninitialize(m_audioOutputUnit);
										}
									}
									else
									{
										printError("openAudio", "Error setting audio render callback", err);
									}
								}
								else
								{
									printError("openAudio", "Error setting audio output format", err);
								}
							}
							else
							{
								printError("openAudio","Failed to find supported audio frequency");
							}

						}
						else
						{
							printError("openAudio", "Error enabling output audio I/O", err);
						}
					}
					else
					{
						printError("openAudio", "Failed to get audio output unit", err);
					}
				}
				else
				{
					printError("openAudio", "Failed to get description for stream");
				}
			}
			else
			{
				printError("openAudio", "Could not find supported format DAC description for codec");
			}
		}
		else
		{
			printError("openAudio", "Failed to find audio output component");
		}
	}
	else
	{
		printError("openAudio", "Failed to get audio device");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioIOS::closeAudio()
{
	OSStatus err;
	
	if(m_audioOutputUnit != 0)
	{
		stopAudioDevice();
		
		if(m_flagInit)
		{
			err = AudioUnitUninitialize(m_audioOutputUnit);
			if(err != noErr)
			{
				printError("closeAudio", "Error uninitializing audio output unit", err);
			}
			m_flagInit = false;
		}
		AudioComponentInstanceDispose(m_audioOutputUnit);
		m_audioOutputUnit = 0;
	}
	closeResampler();
	m_pSampleConverter.clear();
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioIOS::startAudioDevice()
{
	OSStatus err;
	bool res = false;
	
	if(m_flagInit && m_audioOutputUnit != 0)
	{
		err = AudioOutputUnitStart(m_audioOutputUnit);
		if(err == noErr)
		{
			m_flagStart = true;
			res = true;
		}
		else
		{
			printError("startAudioDevice", "Audio output failed to start", err);
		}
	}
	else
	{
		printError("startAudioDevice", "Audio output unit has not been initialized");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioIOS::stopAudioDevice()
{
	if(m_audioOutputUnit != 0 && m_flagStart)
	{
		OSStatus err = AudioOutputUnitStop(m_audioOutputUnit);
		if(err !=noErr)
		{
			printError("stopAudioDevice", "Error stopping audio output", err);
		}
		m_flagStart = false;
	}
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioIOS::isAudio() const
{
	return m_flagInit;;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioIOS::processMessages()
{
	if(getStopTimeFlag())
	{
		if(m_flagStart)
		{
			tint delay;
			common::TimeStamp dT,cT;
			
			cT = common::TimeStamp::reference();
			dT = const_cast<const common::TimeStamp &>(m_stopTimeClock);
			if(dT > cT)
			{
				dT -= cT;
				delay = (dT.secondsTotal() * 1000) + dT.millisecond();
				if(delay>0)
				{
					QTimer::singleShot(delay,this,SLOT(onStopProcess()));
				}
				else
				{
					onStopProcess();
				}
			}
			else
			{
				onStopProcess();
			}
		}
		else
		{
			onStopProcess();
		}
		setStopTimeFlag(false);
	}
}

//-------------------------------------------------------------------------------------------

void AOCoreAudioIOS::onStopProcess()
{
	stopCodec();
}

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
{
	const sample_t *input = data->partDataOutConst(partNumber);

	tint noInputChannels = data->noOutChannels();
	tint noOutputChannels = pBuffer->numberOfChannelsInBuffer(bufferIndex);

	tint iIdx = (inputSampleIndex * noInputChannels) + inChannelIndex;
	tint oIdx = (outputSampleIndex * noOutputChannels) + outChannelIndex;

	tbyte *out = reinterpret_cast<tbyte *>(pBuffer->buffer(bufferIndex));
	out += oIdx * m_pSampleConverter->bytesPerSample();
	
	m_pSampleConverter->setNumberOfInputChannels(noInputChannels);
	m_pSampleConverter->setNumberOfOutputChannels(noOutputChannels);
	m_pSampleConverter->setVolume(m_volume);
	
	m_pSampleConverter->convert(&input[iIdx],out,amount);
}

//-------------------------------------------------------------------------------------------

QSharedPointer<AOQueryDevice::Device> AOCoreAudioIOS::copyDeviceInformation(const AOQueryDevice::Device& iDevice)
{
	QSharedPointer<AOQueryCoreAudioIOS::IOSDevice> dev(new AOQueryCoreAudioIOS::IOSDevice(iDevice));
	QSharedPointer<AOQueryDevice::Device> baseDev = dev.dynamicCast<AOQueryDevice::Device>();
	return baseDev;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<AOQueryCoreAudioIOS::IOSDevice> AOCoreAudioIOS::getCurrentCoreAudioIOSDevice()
{
	QSharedPointer<AOQueryCoreAudioIOS::IOSDevice> pCoreDevice;
	QSharedPointer<AOQueryDevice::Device> pDevice = getCurrentDevice();
	if(!pDevice.isNull())
	{
		pCoreDevice = pDevice.dynamicCast<AOQueryCoreAudioIOS::IOSDevice>();
	}
	return pCoreDevice;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudioIOS::isConvertionAlignedHigh(const AudioStreamBasicDescription& format) const
{
	bool alignHigh = false;
	tint packedBytesPerFrame = (format.mBitsPerChannel >> 3) * format.mChannelsPerFrame;
	
	if(packedBytesPerFrame < static_cast<tint>(format.mBytesPerFrame))
	{
		if(format.mFormatFlags & kAudioFormatFlagIsBigEndian)
		{
			alignHigh = (format.mFormatFlags & kAudioFormatFlagIsAlignedHigh) ? true : false;
		}
		else
		{
			alignHigh = (format.mFormatFlags & kAudioFormatFlagIsAlignedHigh) ? false : true;
		}
	}
	return alignHigh;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<SampleConverter> AOCoreAudioIOS::sampleConverterFromDescription(const AudioStreamBasicDescription& format)
{
	bool isLittleEndian,isAlignHigh;
	QSharedPointer<SampleConverter> pConverter;
	
	isLittleEndian = (format.mFormatFlags & kAudioFormatFlagIsBigEndian) ? false : true;
	isAlignHigh = isConvertionAlignedHigh(format);
	
	if(format.mFormatFlags & kAudioFormatFlagIsFloat)
	{
        QSharedPointer<SampleConverter> nConverter(new SampleConverter(true,isLittleEndian));
		pConverter = nConverter;
	}
	else
	{
		tint noBits = format.mBitsPerChannel;
		tint bytesPerSample = format.mBytesPerFrame / format.mChannelsPerFrame;
		bool isSigned = (format.mFormatFlags & kAudioFormatFlagIsSignedInteger) ? true : false;
        QSharedPointer<SampleConverter> nConverter(new SampleConverter(noBits,bytesPerSample,isLittleEndian,isAlignHigh,isSigned));
		pConverter = nConverter;
	}
	return pConverter;
}

//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudioIOS::iosAudioRender(void *inRegCon, \
                                        AudioUnitRenderActionFlags *ioActionFlags, \
		                                const AudioTimeStamp *inTimeStamp, \
		                                UInt32 inBusNumber, \
		                                UInt32 inNumberFrames, \
		                                AudioBufferList *ioData)
{
	AOCoreAudioIOS *audioIO = reinterpret_cast<AOCoreAudioIOS *>(inRegCon);
	return audioIO->iosAudioRenderImpl(ioActionFlags, inTimeStamp, inBusNumber, inNumberFrames, ioData);
}

//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudioIOS::iosAudioRenderImpl(AudioUnitRenderActionFlags *ioActionFlags, \
		                          const AudioTimeStamp *inTimeStamp, \
		                          UInt32 inBusNumber, \
		                          UInt32 inNumberFrames, \
		                          AudioBufferList *ioData)
{
	IOTimeStamp systemTime;
	AudioHardwareBufferCoreAudioIOS buffer(ioData, inNumberFrames, m_pSampleConverter->bytesPerSample());
	systemTime = createIOTimeStamp(inTimeStamp);
	writeToAudioIOCallback(&buffer, systemTime);
	return noErr;
}

//-------------------------------------------------------------------------------------------

IOTimeStamp AOCoreAudioIOS::createIOTimeStamp(const AudioTimeStamp *sysTime) const
{
    common::TimeStamp tS;
	tfloat64 hTime2nsFactor = static_cast<tfloat64>(m_machTimeInfo.numer) / static_cast<tfloat64>(m_machTimeInfo.denom);
    tuint64 upTime = static_cast<tuint64>(mach_absolute_time() * hTime2nsFactor);
	tS.nano64(upTime);
	return IOTimeStamp(true, tS);
}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
