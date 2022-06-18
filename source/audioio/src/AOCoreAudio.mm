//-------------------------------------------------------------------------------------------
#if defined(OMEGA_MACOSX)
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOCoreAudio.h"
#include "audioio/inc/AOQueryCoreAudio.h"
#include "engine/inc/FormatTypeFromFloat.h"
#include "audioio/inc/AudioHardwareBufferCoreAudio.h"
#include <QSet>
#include <QVector>
#include <QPair>
#include <QCoreApplication>

#import <CoreFoundation/CoreFoundation.h>
#import <Cocoa/Cocoa.h>

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(AOFactory,AOBase, \
                            AOCoreAudioFactory,AOCoreAudio, \
                            "coreaudio",false)

//-------------------------------------------------------------------------------------------

AOCoreAudio::AOCoreAudio(QObject *parent) : AOBase(parent),
	m_outputUnit(0),
	m_flagInit(false),
	m_flagStart(false),
	m_audioStreamMutex(),
	m_audioStreamCondition(),
    m_originalSampleRate(-1),
	m_isIntegerMode(false),
	m_hasExclusiveMode(false),
	m_hasMixingBeenDisabled(false),
	m_pSampleConverter(),
	m_oldStreamID(kAudioObjectUnknown),
	m_oldStreamDescription(0),
	m_integerDeviceID(kAudioObjectUnknown),
    m_pIntegerDeviceIOProcID(0)
{}

//-------------------------------------------------------------------------------------------

AOCoreAudio::~AOCoreAudio()
{}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::printError(const tchar *strR,const tchar *strE) const
{
	common::Log::g_Log << "AOCoreAudio::" << strR << " - " << strE << "." << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::printErrorOS(const tchar *strR,const tchar *strE,OSStatus err) const
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
	common::Log::g_Log << "AOCoreAudio::" << strR << " - " << eStr.toUtf8().constData();
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::init()
{
    bool res = AOBase::init();
	CoreAudioIF::instance("coreaudio");
	updateExclusiveModeOnDevices();
	addListenerDevices();
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::reset()
{
    QSharedPointer<AOQueryCoreAudio::DeviceCoreAudio> pCoreDevice = getCurrentCoreAudioDevice();
	if(!pCoreDevice.isNull())
	{
		resetSampleRateToOriginalWhenClosing(pCoreDevice->deviceID());
	}	

	removeListenerDevices();
	AOBase::reset();
	CoreAudioIF::release();
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::isAudio() const
{
	return m_flagInit;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<AOQueryCoreAudio::DeviceCoreAudio> AOCoreAudio::getCurrentCoreAudioDevice()
{
	QSharedPointer<AOQueryCoreAudio::DeviceCoreAudio> pCoreDevice;
	QSharedPointer<AOQueryDevice::Device> pDevice = getCurrentDevice();
	if(!pDevice.isNull())
	{
		pCoreDevice = pDevice.dynamicCast<AOQueryCoreAudio::DeviceCoreAudio>();
	}
	return pCoreDevice;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::openAudio()
{
	bool res = false;
	
	closeAudio();
	m_frequency = m_codec->frequency();

	m_isIntegerMode = isExclusive();

    QSharedPointer<AOQueryCoreAudio::DeviceCoreAudio> pCoreDevice = getCurrentCoreAudioDevice();
	if(pCoreDevice.isNull())
	{
		printError("openAudio","Could not find audio device");
		return false;		
	}
	
	if(m_isIntegerMode)
	{
		res = openIntegerAudio(pCoreDevice);
		if(!res)
		{
			printError("openAudio","Could not open audio in integer mode");
			closeAudio();
		}
	}
    
    if(!res)
    {
	    m_isIntegerMode = false;
        res = openAudioCoreAudio(pCoreDevice);
		if(!res)
		{
			printError("openAudio","Could not open audio using CoreAudio");
			closeAudio();		
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::openAudioCoreAudio(QSharedPointer<AOQueryCoreAudio::DeviceCoreAudio>& pCoreDevice)
{
	AudioDeviceID devID;
	OSStatus err;
	bool res = false;

	devID = pCoreDevice->deviceID();
	if(devID!=kAudioDeviceUnknown)
	{
		AudioComponent comp;
		AudioComponentDescription desc;
	
		desc.componentType = kAudioUnitType_Output;
		desc.componentSubType = kAudioUnitSubType_HALOutput;
		desc.componentManufacturer = kAudioUnitManufacturer_Apple;
		desc.componentFlags = 0;
		desc.componentFlagsMask = 0;
	
		comp = AudioComponentFindNext(0,&desc);
		if(comp!=0)
		{
			err = AudioComponentInstanceNew(comp,&m_outputUnit);
			if(err==noErr)
			{
				UInt32 enableIO = 1;
				
				err = AudioUnitSetProperty(m_outputUnit,kAudioOutputUnitProperty_EnableIO,kAudioUnitScope_Output,0,&enableIO,sizeof(enableIO));
				if(err!=noErr)
				{
					printErrorOS("openAudio","Error enabling output audio I/O",err);
					return false;
				}
				
				err = AudioUnitSetProperty(m_outputUnit,kAudioOutputUnitProperty_CurrentDevice,kAudioUnitScope_Global,0,&devID,sizeof(devID));
				if(err==noErr)
				{
					if(getDeviceFrequency()>0)
					{
						setSampleRateWhileOpeningCoreDevice(devID);
						
						initCyclicBuffer();
						
						m_noChannels = pCoreDevice->noChannels();
						::memset(&m_streamFormat,0,sizeof(AudioStreamBasicDescription));
						m_streamFormat.mSampleRate = static_cast<Float64>(m_frequency);
						m_streamFormat.mFormatID = kAudioFormatLinearPCM;
						m_streamFormat.mFormatFlags = kAudioFormatFlagsNativeFloatPacked;
						m_streamFormat.mFramesPerPacket = 1;
						m_streamFormat.mBitsPerChannel = sizeof(tfloat32) * 8;
						m_streamFormat.mBytesPerPacket = static_cast<UInt32>(sizeof(tfloat32)) * m_noChannels;
						m_streamFormat.mBytesPerFrame = static_cast<UInt32>(sizeof(tfloat32)) * m_noChannels;
						m_streamFormat.mChannelsPerFrame = static_cast<UInt32>(m_noChannels);
						
						err = AudioUnitSetProperty(m_outputUnit,kAudioUnitProperty_StreamFormat,kAudioUnitScope_Input,0,&m_streamFormat,sizeof(AudioStreamBasicDescription));
						if(err==noErr)
						{
							::memset(&m_renderCallback,0,sizeof(AURenderCallbackStruct));
							m_renderCallback.inputProc = AOCoreAudio::callbackIOProc;
							m_renderCallback.inputProcRefCon = reinterpret_cast<void *>(this);
							
							err = AudioUnitSetProperty(m_outputUnit,kAudioUnitProperty_SetRenderCallback,kAudioUnitScope_Input,0,&m_renderCallback,sizeof(m_renderCallback));
							if(err==noErr)
							{
								addListenerJackConnection(devID);
								
								err = AudioUnitInitialize(m_outputUnit);
								if(err==noErr)
								{
									m_flagInit = true;
									res = true;
								}
								else
								{
									AudioUnitUninitialize(m_outputUnit);
									printErrorOS("openAudio","Failed to initialize audio output device",err);
								}
							}
							else
							{
								printErrorOS("openAudio","Error setting audio render callback",err);
							}
						}
						else
						{
							printErrorOS("openAudio","Failed to set audio format on output device",err);
						}
					}
					else
					{
						printError("openAudio","Failed to find supported audio frequency");
					}
				}
				else
				{
					printErrorOS("openAudio","Failed to set audio output device to HAL audio unit",err);
				}
			}
			else
			{
				printErrorOS("openAudio","Failed to get instance of output HAL component",err);
			}
		}
		else
		{
			printError("openAudio","Failed to find ouput HAL component");
		}
	}
	else
	{
		printError("openAudio","Failed to find output device");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::setSampleRateWhileOpeningCoreDevice(AudioDeviceID devID)
{
	tint freq = getFrequency();
	tint oldRate = setSampleRateIfPossible(devID,freq);

	if(oldRate > 0)
	{
		if(m_originalSampleRate < 0)
		{
			m_originalSampleRate = oldRate;
		}
		else if(m_originalSampleRate == freq)
		{
			m_originalSampleRate = -1;
		}
	}
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::resetSampleRateToOriginalWhenClosing(AudioDeviceID devID)
{
	if(m_originalSampleRate > 0)
	{
		if(setSampleRateIfPossible(devID,m_originalSampleRate) > 0)
		{
			m_originalSampleRate = -1;
		}
	}
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::setDeviceID(tint idIndex)
{
	QSharedPointer<AOQueryCoreAudio::DeviceCoreAudio> pCoreDevice = getCurrentCoreAudioDevice();
	if(!pCoreDevice.isNull())
	{
		resetSampleRateToOriginalWhenClosing(pCoreDevice->deviceID());
		m_originalSampleRate = -1;
	}
	AOBase::setDeviceID(idIndex);
}

//-------------------------------------------------------------------------------------------

int AOCoreAudio::getDeviceFrequency()
{
	int iFreq = m_frequency;
	int oFreq = -1;

	if(m_deviceIdx>=0 && m_deviceIdx<m_deviceInfo->noDevices())
	{
		const AOQueryDevice::Device& dev = m_deviceInfo->device(m_deviceIdx);
		
		if(!dev.isFrequencySupported(iFreq))
		{
			int diff;
			const QSet<int>& fSet = dev.frequencies();
            QSet<int>::const_iterator ppI;
			bool found = false;
			
			for(ppI=fSet.begin();ppI!=fSet.end();ppI++)
			{
				int cFreq = *ppI;
				if(cFreq > iFreq)
				{
					diff = cFreq - iFreq;
					if(found)
					{
						if(diff < (oFreq - iFreq))
						{
							oFreq = cFreq;
						}
					}
					else
					{
						oFreq = cFreq;
						found = true;
					}
				}
			}
			if(!found)
			{
				for(ppI=fSet.begin();ppI!=fSet.end();ppI++)
				{
					int cFreq = *ppI;
					if(cFreq < iFreq)
					{
						diff = iFreq - cFreq;
						if(found)
						{
							if(diff < (iFreq - oFreq))
							{
								oFreq = cFreq;
							}
						}
						else
						{
							oFreq = cFreq;
							found = true;
						}
					}
				}
			}
			
			if(found)
			{
				initResampler(iFreq,oFreq);
				m_frequency = oFreq;
			}
			else
			{
				oFreq = -1;
			}
		}
		else
		{
			oFreq = m_frequency;
		}
	}
	return oFreq;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::closeAudio()
{
	OSStatus err;
	
	if(m_isIntegerMode)
	{
		closeIntegerAudio();
	}
	else
	{
		if(m_outputUnit!=0)
		{
			stopAudioDevice();
		
			if(m_flagInit)
			{
				const AOQueryCoreAudio::DeviceCoreAudio& dev = dynamic_cast<const AOQueryCoreAudio::DeviceCoreAudio &>(m_deviceInfo->device(m_deviceIdx));
				
				err = AudioUnitUninitialize(m_outputUnit);
				if(err!=noErr)
				{
					printErrorOS("closeAudio","Error uninitializing audio output unit",err);
				}
				
				removeListenerJackConnection(dev.deviceID());
				m_flagInit = false;
			}
			
			AudioComponentInstanceDispose(m_outputUnit);
			m_outputUnit = 0;
		}
	}
	closeResampler();
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::startAudioDevice()
{
	bool res = false;

	if(m_isIntegerMode)
	{
        res = startIntegerAudio();
	}
	else
	{
		OSStatus err;
	
		if(m_flagInit)
		{
			err = AudioOutputUnitStart(m_outputUnit);
			if(err==noErr)
			{
				m_flagStart = true;
				res = true;
			}
			else
			{
				printErrorOS("startAudioDevice","Error starting audio output",err);
			}
		}
		else
		{
			printError("startAudioDevice","Audio output unit has not been initialized");
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::stopAudioDevice()
{
	if(m_isIntegerMode)
	{
		stopIntegerAudio();
	}
	else
	{
		if(m_flagStart)
		{
			OSStatus err;
		
			err = AudioOutputUnitStop(m_outputUnit);
			if(err!=noErr)
			{
				printErrorOS("stopAudioDevice","Error stopping audio output",err);
			}
			m_flagStart = false;
		}
	}
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::processMessages()
{
	if(m_stopTimeFlag)
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
		m_stopTimeFlag = false;
	}
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::onStopProcess()
{
	stopCodec();
}

//-------------------------------------------------------------------------------------------

engine::AData *AOCoreAudio::allocateData(tint len,tint inChannel,tint outChannel)
{
    engine::RData *d = new engine::RData(len,inChannel,outChannel);
	return reinterpret_cast<engine::AData *>(d);
}

//-------------------------------------------------------------------------------------------

volatile bool AOCoreAudio::m_outputLatencyCalcFlag = false;
tint AOCoreAudio::m_outputLatencyBufferSize = 0;
volatile common::TimeStamp AOCoreAudio::m_outputLatencyTimeCore;

//-------------------------------------------------------------------------------------------
	
void AOCoreAudio::calcAudioLatency()
{
	UInt32 count,bufferSize = 0,propSize = 0;
	AudioDeviceID devId = kAudioDeviceUnknown;
	OSStatus err;

	AudioObjectPropertyAddress theAddress = { kAudioHardwarePropertyDefaultOutputDevice,
                                              kAudioObjectPropertyScopeGlobal,
                                              kAudioObjectPropertyElementMain };

	propSize = sizeof(devId);
	err = AudioObjectGetPropertyData(kAudioObjectSystemObject,&theAddress,0,0,&propSize,&devId);
	if(err==noErr)
	{
		AudioStreamBasicDescription streamFormat;
		
		::memset(&streamFormat,0,sizeof(AudioStreamBasicDescription));
		streamFormat.mSampleRate = 44100.0;
		streamFormat.mFormatID = kAudioFormatLinearPCM;
		streamFormat.mFormatFlags = kAudioFormatFlagsNativeFloatPacked;
		streamFormat.mFramesPerPacket = 1;
		streamFormat.mBitsPerChannel = sizeof(tfloat32) * 8;
		streamFormat.mBytesPerPacket = sizeof(tfloat32) * 2;
		streamFormat.mBytesPerFrame  = sizeof(tfloat32) * 2;
		streamFormat.mChannelsPerFrame = 2;

		AudioObjectPropertyAddress propertyAddress;
		propertyAddress.mSelector = kAudioDevicePropertyStreamFormat;
		propertyAddress.mScope = kAudioDevicePropertyScopeOutput;    
        propertyAddress.mElement = kAudioObjectPropertyElementMain;

		err = AudioObjectSetPropertyData(devId,&propertyAddress,0,0,sizeof(streamFormat),&streamFormat);
		if(err==noErr)
		{
			count = sizeof(bufferSize);
			
            AudioObjectPropertyAddress bufferAddress = { kAudioDevicePropertyBufferSize, kAudioDevicePropertyScopeOutput, kAudioObjectPropertyElementMain};
			
			err = AudioObjectGetPropertyData(devId,&bufferAddress,0,0,&count,&bufferSize);
			if(err==noErr)
			{
				AudioDeviceIOProcID theIOProcID = 0;
			
				m_outputLatencyBufferSize = static_cast<tint>(bufferSize / streamFormat.mBytesPerFrame);
				
				err = AudioDeviceCreateIOProcID(devId,AOCoreAudio::callbackLatencyProc,0,&theIOProcID);
				if(err==kAudioHardwareNoError)
				{
					err = AudioDeviceStart(devId,AOCoreAudio::callbackLatencyProc);
					if(err==kAudioHardwareNoError)
					{
						while(!m_outputLatencyCalcFlag)
						{
							usleep(100);
						}
						
						AudioDeviceStop(devId,AOCoreAudio::callbackLatencyProc);
					}
					AudioDeviceDestroyIOProcID(devId,theIOProcID);
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudio::callbackLatencyProc(AudioDeviceID id, \
                                          const AudioTimeStamp *inNow, \
                                          const AudioBufferList *inInputData, \
                                          const AudioTimeStamp *inInputTime, \
                                          AudioBufferList *outOutputData, \
                                          const AudioTimeStamp *inOutputTime, \
                                          void *inClientData)
{
	tint j,k,noChs;
	common::TimeStamp sT,oT;
	
	sT.nano64(AudioConvertHostTimeToNanos(inNow->mHostTime));
	oT.nano64(AudioConvertHostTimeToNanos(inOutputTime->mHostTime));
	if(oT > sT)
	{
		m_outputLatencyTimeCore.set(oT - sT);
	}
	else
	{
		common::TimeStamp blankT;
		m_outputLatencyTimeCore.set(blankT);
	}
	m_outputLatencyCalcFlag = true;

	for(tint i = 0; i < static_cast<int>(outOutputData->mNumberBuffers); i++)
	{
		tint noChs = static_cast<int>(outOutputData->mBuffers[i].mNumberChannels);
		::memset(outOutputData->mBuffers[i].mData, 0 , m_outputLatencyBufferSize * noChs * sizeof(tfloat32));
	}
	return kAudioHardwareNoError;
}

//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudio::corePropertyChangeProc(AudioObjectID inObjectID,UInt32 inNumberAddresses,const AudioObjectPropertyAddress inAddresses[],void *inClientData)
{
	AOCoreAudio *audioIO = reinterpret_cast<AOCoreAudio *>(inClientData);
	return audioIO->corePropertyChangeProcImpl(inObjectID,inNumberAddresses,inAddresses);
}

//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudio::corePropertyChangeProcImpl(AudioObjectID inObjectID,UInt32 inNumberAddresses,const AudioObjectPropertyAddress inAddresses[])
{
	UInt32 i;
	
	for(i=0;i<inNumberAddresses;i++)
	{
		switch(inAddresses[i].mSelector)
		{
			case kAudioHardwarePropertyDevices:
                {
                    AudioEvent *e = new AudioEvent(AudioEvent::e_audioDeviceChangeEvent);
                    QCoreApplication::postEvent(this,e);
                }
				break;
				
			case kAudioDevicePropertyJackIsConnected:
				pause();
				break;

			default:
				break;
		}
	}
	return noErr;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::addListenerDevices()
{
	AudioObjectPropertyAddress propAddr;
	OSStatus err;
	
	if(!setupPropertyRunLoop())
	{
		printError("addListenerDevices","Error setting run loop correctly");
	}
	
	propAddr.mSelector = kAudioHardwarePropertyDevices;
	propAddr.mScope = kAudioObjectPropertyScopeGlobal;
    propAddr.mElement = kAudioObjectPropertyElementMain;
	
	err = AudioObjectAddPropertyListener(kAudioObjectSystemObject,&propAddr,AOCoreAudio::corePropertyChangeProc,this);
	if(err!=noErr)
	{
		printError("addListenerDevice","Error adding device hardware listener");
	}
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::removeListenerDevices()
{
	AudioObjectPropertyAddress propAddr;
	OSStatus err;
	
	propAddr.mSelector = kAudioHardwarePropertyDevices;
	propAddr.mScope = kAudioObjectPropertyScopeGlobal;
    propAddr.mElement = kAudioObjectPropertyElementMain;
	
	err = AudioObjectRemovePropertyListener(kAudioObjectSystemObject,&propAddr,AOCoreAudio::corePropertyChangeProc,this);
	if(err!=noErr)
	{
		printError("addListenerDevice","Error removing device hardware listener");
	}
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::addListenerJackConnection(AudioDeviceID devID)
{
	AudioObjectPropertyAddress propAddr;
	OSStatus err;

	propAddr.mSelector = kAudioDevicePropertyJackIsConnected;
	propAddr.mScope = kAudioDevicePropertyScopeOutput;
    propAddr.mElement = kAudioObjectPropertyElementMain;
	if(AudioObjectHasProperty(devID,&propAddr))
	{
		err = AudioObjectAddPropertyListener(devID,&propAddr,AOCoreAudio::corePropertyChangeProc,this);
		if(err!=noErr)
		{
			printError("addListenerDevice","Error adding headphone jack sensor");
		}
	}
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::removeListenerJackConnection(AudioDeviceID devID)
{
	AudioObjectPropertyAddress propAddr;
	OSStatus err;

	propAddr.mSelector = kAudioDevicePropertyJackIsConnected;
	propAddr.mScope = kAudioDevicePropertyScopeOutput;
    propAddr.mElement = kAudioObjectPropertyElementMain;
	if(AudioObjectHasProperty(devID,&propAddr))
	{
		err = AudioObjectRemovePropertyListener(devID,&propAddr,AOCoreAudio::corePropertyChangeProc,this);
		if(err!=noErr)
		{
			printError("addListenerDevice","Error removing headphone jack sensor");
		}
	}
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::audioDeviceChange()
{
	tint i,nDefaultIndex = -1;
	AOQueryCoreAudio *nDeviceInfo;
	QString defaultDeviceID,defaultDeviceName;
	bool res = false,found = false,pauseFlag = false;
	
	nDeviceInfo = new AOQueryCoreAudio;
		
	m_deviceInfoMutex.lock();
	if(nDeviceInfo->queryNames())
	{
		QSettings settings;
			
		for(i=0;i<nDeviceInfo->noDevices();i++)
		{
			nDeviceInfo->queryDevice(i);
		}
			
#if defined(OMEGA_DEBUG)
		nDeviceInfo->print();
#endif
			
		settings.beginGroup("audio");
		if(settings.contains(QString::fromLatin1("defaultDeviceID")))
		{
			defaultDeviceID = settings.value(QString::fromLatin1("defaultDeviceID"),QVariant(QString())).toString();
			if(!defaultDeviceID.isEmpty())
			{
				for(i=0;i<nDeviceInfo->noDevices() && !found;i++)
				{
					if(defaultDeviceID==nDeviceInfo->device(i).id())
					{
						nDefaultIndex = i;
						found = true;
					}
				}
			}
		}
		if(!found)
		{
			if(settings.contains(QString::fromLatin1("defaultDeviceName")))
			{
				defaultDeviceName = settings.value(QString::fromLatin1("defaultDeviceName"),QVariant(QString())).toString();
				if(!defaultDeviceName.isEmpty())
				{
					for(i=0;i<nDeviceInfo->noDevices() && !found;i++)
					{
						nDefaultIndex = i;
						found = true;
					}
				}
			}
		}
		if(!found)
		{
			nDefaultIndex = nDeviceInfo->defaultDeviceIndex();
		}
		settings.endGroup();
			
		if(m_deviceInfo!=0)
		{
			if((m_deviceIdx>=0 && m_deviceIdx<m_deviceInfo->noDevices()) && (nDefaultIndex>=0 && nDefaultIndex<nDeviceInfo->noDevices()))
			{
				QString nDeviceID,cDeviceID;
					
				cDeviceID = m_deviceInfo->device(m_deviceIdx).id();
				nDeviceID = nDeviceInfo->device(nDefaultIndex).id();
				if(cDeviceID!=nDeviceID)
				{
					pauseFlag = true;
				}
				res = true;
			}
		}
	}
		
	if(res)
	{
		if(m_deviceInfo!=0)
		{
			delete m_deviceInfo;
		}
		m_deviceInfo = nDeviceInfo;
		m_deviceIdx = nDefaultIndex;
		updateExclusiveModeOnDevices();
		QString devName = m_deviceInfo->device(m_deviceIdx).name();
		m_audioChannelMap.load(devName);
	}
	else
	{
		delete nDeviceInfo;
	}
	m_deviceInfoMutex.unlock();
		
	if(res)
	{
		resetPlayback();
		if(pauseFlag)
		{
			pausePlayback(true);
		}
	}
}

//-------------------------------------------------------------------------------------------
// IOProc refactor
//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudio::callbackIOProc(void *inRegCon, \
                                     AudioUnitRenderActionFlags *ioActionFlags, \
		                             const AudioTimeStamp *inTimeStamp, \
		                             UInt32 inBusNumber, \
		                             UInt32 inNumberFrames, \
		                             AudioBufferList *ioData)
{
	AOCoreAudio *ao = reinterpret_cast<AOCoreAudio *>(inRegCon);
	return ao->callbackIOProcI(ioActionFlags,inTimeStamp,inBusNumber,inNumberFrames,ioData);
}

//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudio::callbackIOProcI(AudioUnitRenderActionFlags *ioActionFlags, \
		                              const AudioTimeStamp *inTimeStamp, \
		                              UInt32 inBusNumber, \
		                              UInt32 inNumberFrames, \
		                              AudioBufferList *ioData)
{
	IOTimeStamp systemTime;
	AudioHardwareBufferCoreAudio buffer(ioData,inNumberFrames,sizeof(tfloat32));
	systemTime = createIOTimeStamp(inTimeStamp);
	writeToAudioIOCallback(&buffer,systemTime);
	return noErr;
}

//-------------------------------------------------------------------------------------------

IOTimeStamp AOCoreAudio::createIOTimeStamp(const AudioTimeStamp *sysTime) const
{
    common::TimeStamp tS;
	tS.nano64(AudioConvertHostTimeToNanos(sysTime->mHostTime));
	return IOTimeStamp(true,tS);
}

//-------------------------------------------------------------------------------------------
// HAL Integer
//-------------------------------------------------------------------------------------------

bool AOCoreAudio::setupPropertyRunLoop()
{
	bool res;
	OSStatus err;
	CFRunLoopRef runLoop = 0;
    AudioObjectPropertyAddress property = { kAudioHardwarePropertyRunLoop, kAudioObjectPropertyScopeGlobal,kAudioObjectPropertyElementMain };
	
	err = CoreAudioIF::instance()->AudioObjectSetPropertyData(kAudioObjectSystemObject,&property,0,0,sizeof(CFRunLoopRef),
		reinterpret_cast<const void *>(&runLoop));
	if(err==noErr)
	{
		res = true;
	}
	else
	{
        printErrorOS("setupPropertyRunLoop","Error setting run loop",err);
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

pid_t AOCoreAudio::getCurrentProcessID() const
{
	return getpid();
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::useExclusiveModeIfAvailable(AudioDeviceID devId)
{
	bool res = false;
    AudioObjectPropertyAddress property = { kAudioDevicePropertyHogMode, kAudioDevicePropertyScopeOutput, kAudioObjectPropertyElementMain };
	
    if(CoreAudioIF::instance()->AudioObjectHasProperty(devId,&property))
	{
		OSStatus err;
		Boolean settableFlag = false;
		
		err = CoreAudioIF::instance()->AudioObjectIsPropertySettable(devId,&property,&settableFlag);
		if(err==noErr)
		{
			if(settableFlag)
			{
				pid_t processID, cProcessID;
				UInt32 propSize = sizeof(pid_t);
				
				err = CoreAudioIF::instance()->AudioObjectGetPropertyData(devId,&property,0,0,&propSize,reinterpret_cast<void *>(&processID));
				if(err==noErr)
				{
					cProcessID = getCurrentProcessID();
					
					if(processID==-1)
					{
						err = CoreAudioIF::instance()->AudioObjectSetPropertyData(devId,&property,0,0,propSize,reinterpret_cast<const void *>(&cProcessID));
						if(err==noErr)
						{
							res = true;
						}
						else
						{
							printErrorOS("useExclusiveModeIfAvailable","Failed to set exclusive mode for audio device",err);
							res = false;
						}
					}
					else
					{
						res = (processID==cProcessID);
					}
				}
				else
				{
					printErrorOS("useExclusiveModeIfAvailable","Failed to get exclusive mode process information",err);
					res = false;
				}
			}
			else
			{
				res = true;
			}
		}
		else
		{
			printErrorOS("useExclusiveModeIfAvailable","Error querying if exclusive mode can be set",err);
			res = false;
		}
	}
	else
	{
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::releaseExclusiveMode(AudioDeviceID devID)
{
    AudioObjectPropertyAddress property = { kAudioDevicePropertyHogMode, kAudioDevicePropertyScopeOutput, kAudioObjectPropertyElementMain };
	
    if(CoreAudioIF::instance()->AudioObjectHasProperty(devID,&property))
	{
		OSStatus err;
		Boolean settableFlag = false;
		
        err = CoreAudioIF::instance()->AudioObjectIsPropertySettable(devID,&property,&settableFlag);
		if(err==noErr && settableFlag)
		{
			pid_t processID, cProcessID;
			UInt32 propSize = sizeof(pid_t);
			
            err = CoreAudioIF::instance()->AudioObjectGetPropertyData(devID,&property,0,0,&propSize,reinterpret_cast<void *>(&processID));
			if(err==noErr)
			{
				cProcessID = getCurrentProcessID();
				if(processID==cProcessID)
				{
					cProcessID = -1;
                    CoreAudioIF::instance()->AudioObjectSetPropertyData(devID,&property,0,0,propSize,reinterpret_cast<const void *>(&cProcessID));
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::disableMixingIfPossible(AudioDeviceID devID)
{
    AudioObjectPropertyAddress property = { kAudioDevicePropertySupportsMixing, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain };
	bool res = false;
	
	if(CoreAudioIF::instance()->AudioObjectHasProperty(devID,&property))
	{
		OSStatus err;
		Boolean writeable = false;
		
		err = CoreAudioIF::instance()->AudioObjectIsPropertySettable(devID,&property,&writeable);
		if(err==noErr)
		{
			if(writeable)
			{
				UInt32 paramSize = 0;
			
				err = CoreAudioIF::instance()->AudioObjectGetPropertyDataSize(devID,&property,0,0,&paramSize);
				if(err==noErr)
				{
					UInt32 mix = 0;
					
					err = CoreAudioIF::instance()->AudioObjectGetPropertyData(devID,&property,0,0,&paramSize,&mix);
					if(err==noErr)
					{
						if(mix)
						{
							mix = 0;
							err = CoreAudioIF::instance()->AudioObjectSetPropertyData(devID,&property,0,0,paramSize,&mix);
							if(err==noErr)
							{
								res = true;
							}
							else
							{
								printErrorOS("disableMixingIfPossible","Error disabling mixing property on audio device",err);
							}
						}
					}
					else
					{
						printErrorOS("disableMixingIfPossible","Error getting mixing property",err);
					}
				}
				else
				{
					printErrorOS("disableMixingIfPossible","Error getting mixing property size",err);
				}
			}
		}
		else
		{
			printErrorOS("disableMixingIfPossible","Error getting if mixing property is writeable",err);
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::reEnableMixing(AudioDeviceID devID)
{
    AudioObjectPropertyAddress property = { kAudioDevicePropertySupportsMixing, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain };
	
	if(CoreAudioIF::instance()->AudioObjectHasProperty(devID,&property))
	{
		OSStatus err;
		Boolean writeable = false;
		
		err = CoreAudioIF::instance()->AudioObjectIsPropertySettable(devID,&property,&writeable);
		if(err==noErr)
		{
			if(writeable)
			{
				UInt32 paramSize = 0;
				
				err = CoreAudioIF::instance()->AudioObjectGetPropertyDataSize(devID,&property,0,0,&paramSize);
				if(err==noErr)
				{
					UInt32 mix = 1;
					
					err = CoreAudioIF::instance()->AudioObjectSetPropertyData(devID,&property,0,0,paramSize,&mix);
					if(err!=noErr)
					{
						printErrorOS("reEnableMixing","Failed to re-enable mixing on audio device",err);
					}
				}
				else
				{
					printErrorOS("reEnableMixing","Error getting mixing property size",err);
				}
			}
		}
		else
		{
			printErrorOS("reEnableMixing","Error getting if mixing property is writeable",err);
		}
	}
}

//-------------------------------------------------------------------------------------------

QVector<AudioStreamID> AOCoreAudio::getAudioStreamsForDevice(AudioDeviceID devID)
{
	OSStatus err;
	UInt32 paramSize = 0;
	QVector<AudioStreamID> streamIDs;
    AudioObjectPropertyAddress property = { kAudioDevicePropertyStreams, kAudioDevicePropertyScopeOutput, kAudioObjectPropertyElementMain };
	
	err = CoreAudioIF::instance()->AudioObjectGetPropertyDataSize(devID,&property,0,0,&paramSize);
	if(err==noErr)
	{
		tint noStreams = static_cast<tint>(paramSize) / sizeof(AudioStreamID);
		
		if(noStreams > 0)
		{
			AudioStreamID *pStreams = new AudioStreamID [noStreams];
			
			err = CoreAudioIF::instance()->AudioObjectGetPropertyData(devID,&property,0,0,&paramSize,pStreams);
			if(err==noErr)
			{
				for(tint i=0;i<noStreams;i++)
				{
					streamIDs.append(pStreams[i]);
				}
			}
			else
			{
				printErrorOS("getAudioStreamsForDevice","Error getting audio streams for device",err);
			}
			delete [] pStreams;
		}
		else
		{
			printError("getAudioStreamsForDevice","No audio streams available for device");
		}
	}
	else
	{
		printErrorOS("getAudioStreamsForDevice","Error getting number of audio streams for device",err);
	}
	return streamIDs;
}

//-------------------------------------------------------------------------------------------

QVector<AudioStreamRangedDescription> AOCoreAudio::getAudioStreamDescriptions(AudioStreamID streamID)
{
	OSStatus err;
	UInt32 paramSize = 0;
	QVector<AudioStreamRangedDescription> streams;
	AudioObjectPropertyAddress property = { kAudioStreamPropertyAvailablePhysicalFormats, kAudioObjectPropertyScopeGlobal, 0 };
	
	err = CoreAudioIF::instance()->AudioObjectGetPropertyDataSize(streamID,&property,0,0,&paramSize);
	if(err==noErr)
	{
		tint noFormats = static_cast<tint>(paramSize) / sizeof(AudioStreamRangedDescription);
		if(noFormats>0)
		{
			AudioStreamRangedDescription *descriptions = new AudioStreamRangedDescription [noFormats];
			
			err = CoreAudioIF::instance()->AudioObjectGetPropertyData(streamID,&property,0,0,&paramSize,descriptions);
			if(err==noErr)
			{
				for(tint i=0;i<noFormats;i++)
				{
					streams.append(descriptions[i]);
				}
			}
			else
			{
                printErrorOS("getAudioStreamDescriptions","Failed to get formats for stream ID",err);
			}
			
			delete [] descriptions;
		}
		else
		{
			printError("getAudioStreamDescriptions","No formats available for given stream");
		}
	}
	else
	{
		printErrorOS("getAudioStreamDescriptions","Error getting number of physical stream formats",err);
	}
	return streams;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::isFormatDataTypeCorrisponding(const AudioStreamBasicDescription& format,const FormatDescription& desc)
{
	bool res = false;
	
	if(format.mFormatID==kAudioFormatLinearPCM)
	{
		if((format.mFormatFlags & kAudioFormatFlagIsFloat) && desc.typeOfData()==FormatDescription::e_DataFloatSingle)
		{
			res = true;
		}
		else if((format.mFormatFlags & kAudioFormatFlagIsSignedInteger) && desc.typeOfData()==FormatDescription::e_DataSignedInteger)
		{
			res = true;
		}
		else if(!((format.mFormatFlags & kAudioFormatFlagIsFloat) || (format.mFormatFlags & kAudioFormatFlagIsSignedInteger)) && desc.typeOfData()==FormatDescription::e_DataUnsignedInteger)
		{
			res = true;
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

QVector<int> AOCoreAudio::findFrequenciesFromRange(const AudioStreamRangedDescription& range,bool useRange)
{
	QVector<int> frequencies;
	
	if(useRange)
	{
		m_deviceInfoMutex.lock();
		if(m_deviceIdx>=0 && m_deviceIdx<getDeviceInfo()->noDevices())
		{
			const QSet<int>& freqs = getDeviceInfo()->device(m_deviceIdx).frequencies();
			
            for(QSet<int>::const_iterator ppI=freqs.constBegin();ppI!=freqs.constEnd();ppI++)
			{
				tfloat64 f = static_cast<int>(*ppI);
				
				if(f>=range.mSampleRateRange.mMinimum && f<=range.mSampleRateRange.mMaximum)
				{
					frequencies.append(*ppI);
				}
			}
		}
		m_deviceInfoMutex.unlock();
	}
	else
	{
		frequencies.append(range.mFormat.mSampleRate);
	}
	return frequencies;
}

//-------------------------------------------------------------------------------------------

QVector<FormatDescription> AOCoreAudio::formatDescriptionsFromRanged(const AudioStreamRangedDescription& range,bool useRange)
{
	QVector<FormatDescription> descriptions;
	QVector<int> frequencies = findFrequenciesFromRange(range,useRange);
	
	for(QVector<int>::iterator ppI=frequencies.begin();ppI!=frequencies.end();ppI++)
	{
		FormatDescription desc;
		
		if(range.mFormat.mFormatID==kAudioFormatLinearPCM)
		{
			if(range.mFormat.mFormatFlags & kAudioFormatFlagIsFloat)
			{
				desc.setTypeOfData(FormatDescription::e_DataFloatSingle);
			}
			else if(range.mFormat.mFormatFlags & kAudioFormatFlagIsSignedInteger)
			{
				desc.setTypeOfData(FormatDescription::e_DataSignedInteger);
			}
			else
			{
				desc.setTypeOfData(FormatDescription::e_DataUnsignedInteger);
			}
			
			if(desc.setNumberOfBits(range.mFormat.mBitsPerChannel))
			{
				if(desc.setNumberOfChannels(range.mFormat.mChannelsPerFrame))
				{
					if(desc.setFrequency(*ppI))
					{
						if(isFormatDataTypeCorrisponding(range.mFormat,desc))
						{
							descriptions.append(desc);
						}
					}
				}
			}
		}
	}
	return descriptions;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::areDescriptionsEquivalent(const AudioStreamBasicDescription& format,const FormatDescription& desc)
{
	bool res = false;
	
	if(format.mFormatID==kAudioFormatLinearPCM)
	{
		if(isFormatDataTypeCorrisponding(format,desc))
		{
			if(desc.bits()==format.mBitsPerChannel && desc.channels()==format.mChannelsPerFrame && desc.frequency()==static_cast<tint>(format.mSampleRate))
			{
				res = true;
			}
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

FormatsSupported AOCoreAudio::getSupportedFormatsForStreams(const QVector<AudioStreamID>& streamIDs)
{
	FormatsSupported supported;

    for(QVector<AudioStreamID>::const_iterator ppI=streamIDs.constBegin();ppI!=streamIDs.constEnd();ppI++)
	{
		AudioStreamID ID = *ppI;
		QVector<AudioStreamRangedDescription> streamList;
		
        streamList = getAudioStreamDescriptions(ID);
        for(QVector<AudioStreamRangedDescription>::const_iterator ppJ=streamList.constBegin();ppJ!=streamList.constEnd();ppJ++)
		{
			const AudioStreamRangedDescription& range = *ppJ;
			
            if(range.mFormat.mFormatFlags & kAudioFormatFlagIsNonMixable)
			{
				QVector<FormatDescription> descs = formatDescriptionsFromRanged(range,false);
				
				for(QVector<FormatDescription>::iterator ppK=descs.begin();ppK!=descs.end();ppK++)
				{
					supported.add(*ppK);
				}
			}
		}
	}
	return supported;
}

//-------------------------------------------------------------------------------------------

QPair<AudioStreamID,AudioStreamBasicDescription *> AOCoreAudio::findClosestStream(const FormatDescription& sourceDesc,const QVector<AudioStreamID>& streamIDs)
{
	FormatsSupported supported;
	FormatDescription closestDesc;
	QPair<AudioStreamID,AudioStreamBasicDescription *> streamID(kAudioObjectUnknown,0);
	
	supported = getSupportedFormatsForStreams(streamIDs);
	
	if(FormatDescriptionUtils::findClosestFormatType(sourceDesc,supported,closestDesc))
	{
		for(QVector<AudioStreamID>::const_iterator ppI=streamIDs.constBegin();ppI!=streamIDs.constEnd();ppI++)
		{
			AudioStreamID ID = *ppI;
			QVector<AudioStreamRangedDescription> streamList;
			
			streamList = getAudioStreamDescriptions(ID);
			for(QVector<AudioStreamRangedDescription>::const_iterator ppJ=streamList.constBegin();ppJ!=streamList.constEnd();ppJ++)
			{
				const AudioStreamRangedDescription& range = *ppJ;
				
				if(areDescriptionsEquivalent(range.mFormat,closestDesc) && (range.mFormat.mFormatFlags & kAudioFormatFlagIsNonMixable))
				{
					streamID.first = ID;
					streamID.second = new AudioStreamBasicDescription;
					memcpy(streamID.second,&range.mFormat,sizeof(AudioStreamBasicDescription));
				}
			}
		}
	}
	return streamID;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::isConvertionAlignedHigh(const AudioStreamBasicDescription& format) const
{
	bool alignHigh = false;
	tint packedBytesPerFrame = (format.mBitsPerChannel >> 3) * format.mChannelsPerFrame;
	
	if(packedBytesPerFrame < format.mBytesPerFrame)
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

QSharedPointer<SampleConverter> AOCoreAudio::sampleConverterFromDescription(const AudioStreamBasicDescription& format)
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

bool AOCoreAudio::formatFromStreamDescription(const AudioStreamBasicDescription& format,FormatDescription& desc)
{
	bool res = false;
	
	if(format.mFormatID==kAudioFormatLinearPCM)
	{
		FormatDescription::DataType type;
	
		if(format.mFormatFlags & kAudioFormatFlagIsFloat)
		{
			type = FormatDescription::e_DataFloatSingle;
		}
		else
		{
			if(format.mFormatFlags & kAudioFormatFlagIsSignedInteger)
			{
				type = FormatDescription::e_DataSignedInteger;
			}
			else
			{
				type = FormatDescription::e_DataUnsignedInteger;
			}
		}
		
		if(desc.setTypeOfData(type))
		{
			if(desc.setNumberOfBits(format.mBitsPerChannel))
			{
				if(desc.setNumberOfChannels(format.mChannelsPerFrame))
				{
					if(desc.setFrequency(format.mSampleRate))
					{
						res = true;	
					}
				}
			}
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

tint AOCoreAudio::audioStreamWaitTimeout() const
{
	return 500;
}

//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudio::audioStreamChangeListener(AudioObjectID inObjectID,UInt32 inNumberAddresses,const AudioObjectPropertyAddress inAddress[],void *inClientData)
{
	OSStatus err = noErr;
	AOCoreAudio *pAudio = reinterpret_cast<AOCoreAudio *>(inClientData);
	
	if(pAudio!=0)
	{
		err = pAudio->audioStreamChangeListenerImpl(inObjectID,inNumberAddresses,inAddress);
	}
	return err;
}

//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudio::audioStreamChangeListenerImpl(AudioObjectID inObjectID,UInt32 inNumberAddresses,const AudioObjectPropertyAddress inAddress[])
{
	for(UInt32 i=0;i<inNumberAddresses;i++)
	{
		if(inAddress[i].mSelector == kAudioStreamPropertyPhysicalFormat)
		{
            m_audioStreamCondition.wakeAll();
		}
	}
	return noErr;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::setAudioStream(AudioStreamID streamID,const AudioStreamBasicDescription& format)
{
	FormatDescription desc;
	bool res = false;
	
	if(formatFromStreamDescription(format,desc))
	{
		OSStatus err;
        AudioObjectPropertyAddress property = { kAudioStreamPropertyPhysicalFormat, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain };
		
        err = CoreAudioIF::instance()->AudioObjectAddPropertyListener(streamID,&property,AOCoreAudio::audioStreamChangeListener,reinterpret_cast<void *>(this));
		if(err==noErr)
		{
			m_audioStreamMutex.lock();
		
            err = CoreAudioIF::instance()->AudioObjectSetPropertyData(streamID,&property,0,0,sizeof(AudioStreamBasicDescription),&format);
			if(err==noErr)
			{
				AudioStreamBasicDescription actualFormat;
				
				for(tint i=0;i<9 && !res;i++)
				{
					UInt32 paramSize = sizeof(AudioStreamBasicDescription);
				
					if(i > 0)
					{
						m_audioStreamCondition.wait(&m_audioStreamMutex,audioStreamWaitTimeout());
					}
                    err = CoreAudioIF::instance()->AudioObjectGetPropertyData(streamID,&property,0,0,&paramSize,&actualFormat);
					if(err==noErr)
					{
						FormatDescription actualDesc;
						
						if(formatFromStreamDescription(actualFormat,actualDesc))
						{
							if(desc==actualDesc)
							{
								res = true;
							}
						}
					}
					else
					{
						printErrorOS("setAudioStream","Error getting physical format for device",err);
					}
				}
				
			}
			else
			{
				printErrorOS("setAudioStream","Could not set the stream format",err);
			}
			
			m_audioStreamMutex.unlock();
			
			err = CoreAudioIF::instance()->AudioObjectRemovePropertyListener(streamID,&property,AOCoreAudio::audioStreamChangeListener,reinterpret_cast<void *>(this));
			if(err!=noErr)
			{
				printErrorOS("setAudioStream","Failed to remove property listener",err);
				res = false;
			}
		}
		else
		{
			printErrorOS("setAudioStream","Failed to attach property listener",err);
		}
	}
	else
	{
		printError("setAudioStream","Incompatible audio format");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

AudioStreamBasicDescription *AOCoreAudio::saveAudioDescriptionForStream(AudioStreamID streamID)
{
	OSStatus err;
	UInt32 paramSize = sizeof(AudioStreamBasicDescription);
    AudioObjectPropertyAddress property = { kAudioStreamPropertyPhysicalFormat, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain };
	AudioStreamBasicDescription *format = new AudioStreamBasicDescription;
	
	memset(format,0,sizeof(AudioStreamBasicDescription));
	
	err = CoreAudioIF::instance()->AudioObjectGetPropertyData(streamID,&property,0,0,&paramSize,format);
	if(err!=noErr)
	{
		printErrorOS("saveAudioDescriptionForStream","Error getting current audio stream description",err);
		delete format;
		format = 0;
	}
	return format;
}

//-------------------------------------------------------------------------------------------

tint AOCoreAudio::setSampleRateIfPossible(AudioDeviceID devID,int sampleRate)
{
	tint oldRate = -1;
	OSStatus err;
	Boolean settable = false;
    AudioObjectPropertyAddress property = { kAudioDevicePropertyNominalSampleRate, kAudioDevicePropertyScopeOutput, kAudioObjectPropertyElementMain };
	
	if(CoreAudioIF::instance()->AudioObjectHasProperty(devID,&property))
	{
		err = CoreAudioIF::instance()->AudioObjectIsPropertySettable(devID,&property,&settable);
		if(err==noErr)
		{
			if(settable)
			{
				Float64 oRate;
				UInt32 paramSize = sizeof(Float64);
			
				err = CoreAudioIF::instance()->AudioObjectGetPropertyData(devID,&property,0,0,&paramSize,&oRate);
				if(err==noErr)
				{
					oldRate = static_cast<tint>(oRate);
				
					if(oldRate!=sampleRate)
					{
						Float64 sRate = static_cast<tint>(sampleRate);
    	                err = CoreAudioIF::instance()->AudioObjectSetPropertyData(devID,&property,0,0,paramSize,reinterpret_cast<const void *>(&sRate));
						if(err!=noErr)
						{
							QString eMsg;
							eMsg = "Failed to set sample rate to " + QString::number(sampleRate);
							printErrorOS("setSampleRateIfPossible",eMsg.toUtf8().constData(),err);
						}
					}
					else
					{
						oldRate = -1;
					}
				}
				else
				{
					printErrorOS("setSampleRateIfPossible","Failed to get sample rate from device",err);
				}
			}
		}
		else
		{
			printErrorOS("setSampleRateIfPossible","Error getting if sample rate is settable for device",err);
		}
	}
	return oldRate;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<AOQueryDevice::Device> AOCoreAudio::copyDeviceInformation(const AOQueryDevice::Device& iDevice)
{
	QSharedPointer<AOQueryDevice::Device> pDevice(new AOQueryCoreAudio::DeviceCoreAudio(iDevice));
	return pDevice;
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::openIntegerAudio(QSharedPointer<AOQueryCoreAudio::DeviceCoreAudio>& pDevice)
{
	QVector<AudioStreamID> streamList;
	bool res = false;

	if(isDeviceAlive(pDevice))
	{
		m_integerDeviceID = pDevice->deviceID();

		m_hasExclusiveMode = useExclusiveModeIfAvailable(pDevice->deviceID());
		m_hasMixingBeenDisabled = disableMixingIfPossible(pDevice->deviceID());
	
    	streamList = getAudioStreamsForDevice(pDevice->deviceID());
		if(!streamList.isEmpty())
		{
			QPair<AudioStreamID,AudioStreamBasicDescription *> closestStream;
			FormatDescription sourceDescription = getSourceDescription(pDevice->noChannels());
			
			closestStream = findClosestStream(sourceDescription,streamList);
			if(closestStream.first!=kAudioObjectUnknown && closestStream.second!=0)
			{
				m_pSampleConverter = sampleConverterFromDescription(*closestStream.second);
				if(!m_pSampleConverter.isNull() && m_pSampleConverter->isSupported())
				{
					int oFreq = closestStream.second->mSampleRate;
					
					// All configuration data is now obtained, so open the device.
					
					setSampleRateWhileOpeningCoreDevice(pDevice->deviceID());
	
					if(getFrequency() != oFreq)
					{
						int iFreq = getFrequency();
						setFrequency(oFreq);
						initResampler(iFreq,getFrequency());
					}
					
					initCyclicBuffer();
				
					// Why is the old stream ID being set from the closest found stream ID?
					m_oldStreamID = closestStream.first;
					m_oldStreamDescription = saveAudioDescriptionForStream(closestStream.first);
					
					if(setAudioStream(closestStream.first,*(closestStream.second)))
					{
						OSStatus err;
					
						m_pIntegerDeviceIOProcID = new AudioDeviceIOProcID;
						err = CoreAudioIF::instance()->AudioDeviceCreateIOProcID(pDevice->deviceID(),AOCoreAudio::IOProcInteger,reinterpret_cast<void *>(this),m_pIntegerDeviceIOProcID);
						if(err==noErr)
						{
							addListenerJackConnection(pDevice->deviceID());
							m_flagInit = true;
							res = true;
						}
						else
						{
							printErrorOS("openIntegerAudio","Failed to create I/O playback callback",err);
							delete m_pIntegerDeviceIOProcID;
							m_pIntegerDeviceIOProcID = 0;
						}
					}
					else
					{
						printError("openIntegerAudio","Failed to setup physical format on audio stream");
					}
				}
				else
				{
					printError("openIntegerAudio","Closest audio format not supported");
				}
			}
			else
			{
				printError("openIntegerAudio","Failed to find format for device");
			}
		}
		else
		{
			printError("openIntegerAudio","Failed to get list of audio streams for device");
		}
	}
	else
	{
		printError("openIntegerAudio","Device is not alive");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::closeIntegerAudio()
{
	OSStatus err;

	if(m_integerDeviceID!=kAudioObjectUnknown)
	{
		removeListenerJackConnection(m_integerDeviceID);
	
		if(m_pIntegerDeviceIOProcID!=0)
		{
			err = CoreAudioIF::instance()->AudioDeviceDestroyIOProcID(m_integerDeviceID,*m_pIntegerDeviceIOProcID);
			if(err!=noErr)
			{
				printErrorOS("closeIntegerAudio","Error removing I/O playback callback",err);
			}
			delete m_pIntegerDeviceIOProcID;
			m_pIntegerDeviceIOProcID = 0;	
		}

		if(m_oldStreamID!=kAudioObjectUnknown && m_oldStreamDescription!=0)
		{
			if(!setAudioStream(m_oldStreamID,*m_oldStreamDescription))
			{
				printError("closeIntegerAudio","Error restoring original audio stream");
			}
			delete m_oldStreamDescription;
			m_oldStreamDescription = 0;
		}

		m_pSampleConverter.clear();
	
		if(m_hasMixingBeenDisabled)
		{
			reEnableMixing(m_integerDeviceID);
			m_hasMixingBeenDisabled = false;
		}
		
		if(m_hasExclusiveMode)
		{
			releaseExclusiveMode(m_integerDeviceID);
			m_hasExclusiveMode = false;
		}
	
		m_integerDeviceID = kAudioObjectUnknown;
		m_flagInit = false;
	}
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::startIntegerAudio()
{
	bool res = false;

	if(m_flagInit)
	{
		OSStatus err;
		
		err = CoreAudioIF::instance()->AudioDeviceStart(m_integerDeviceID,*m_pIntegerDeviceIOProcID);
		if(err==noErr)
		{
			m_flagStart = true;
			res = true;
		}
		else
		{
			printErrorOS("startIntegerAudio","Error starting audio integer output",err);
		}
	}
	else
	{
		printError("startIntegerAudio","Audio output device has not been setup");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::stopIntegerAudio()
{
	if(m_flagStart)
	{
		OSStatus err;
		
		err = CoreAudioIF::instance()->AudioDeviceStop(m_integerDeviceID,*m_pIntegerDeviceIOProcID);
		if(err!=noErr)
		{
			printErrorOS("stopIntegerAudio","Error stopping audio integer output",err);
		}
		m_flagStart = false;
	}
}

//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudio::IOProcInteger(AudioDeviceID ioDevice,
                                	const AudioTimeStamp *inNow,
                                    const AudioBufferList *inInputData,
                                    const AudioTimeStamp *inInputTime,
                                    AudioBufferList *outOutputData,
                                    const AudioTimeStamp *inOutputTime,
                                    void *inClientData)
{
	AOCoreAudio *pAudio = reinterpret_cast<AOCoreAudio *>(inClientData);
	return pAudio->IntegerIOProcImpl(ioDevice,inNow,inInputData,inInputTime,outOutputData,inOutputTime);
}

//-------------------------------------------------------------------------------------------

OSStatus AOCoreAudio::IntegerIOProcImpl(AudioDeviceID ioDevice,
                                        const AudioTimeStamp *inNow,
                                        const AudioBufferList *inInputData,
                                        const AudioTimeStamp *inInputTime,
                                        AudioBufferList *outOutputData,
                                        const AudioTimeStamp *inOutputTime)
{
	IOTimeStamp systemTime;
	UInt32 inNumberFrames = outOutputData->mBuffers[0].mDataByteSize / (outOutputData->mBuffers[0].mNumberChannels * m_pSampleConverter->bytesPerSample());
	AudioHardwareBufferCoreAudio buffer(outOutputData,inNumberFrames,m_pSampleConverter->bytesPerSample());
	systemTime = createIOTimeStamp(inOutputTime);
	writeToAudioIOCallback(&buffer,systemTime);
	return noErr;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::writeToAudioOutputBufferFromPartData(AbstractAudioHardwareBuffer *pBuffer,
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
	
	if(m_isIntegerMode)
	{
		tbyte *out = reinterpret_cast<tbyte *>(pBuffer->buffer(bufferIndex));
		out += oIdx * m_pSampleConverter->bytesPerSample();
		
		m_pSampleConverter->setNumberOfInputChannels(noInputChannels);
		m_pSampleConverter->setNumberOfOutputChannels(noOutputChannels);
		m_pSampleConverter->setVolume(m_volume);
		
		m_pSampleConverter->convert(&input[iIdx],out,amount);
	}
	else
	{	
		tfloat32 *out = reinterpret_cast<tfloat32 *>(pBuffer->buffer(bufferIndex));
		tint tAmount = oIdx + (amount * noOutputChannels);
		
		while(oIdx < tAmount)
		{
#if defined(SINGLE_FLOAT_SAMPLE)
			out[oIdx] = input[iIdx] * m_volume;
#else
			out[oIdx] = engine::sampleToFloat32(input[iIdx] * m_volume);
#endif
			iIdx += noInputChannels;
			oIdx += noOutputChannels;
		}
	}
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::canDeviceSupportExclusiveMode(AudioDeviceID devID)
{
	QVector<AudioStreamID> streamList;
	bool res = false;
	
	streamList = getAudioStreamsForDevice(devID);
	if(!streamList.isEmpty())
	{
	    for(QVector<AudioStreamID>::const_iterator ppI=streamList.constBegin();ppI!=streamList.constEnd() && !res;ppI++)
		{
			AudioStreamID ID = *ppI;
			QVector<AudioStreamRangedDescription> streamList;
		
    	    streamList = getAudioStreamDescriptions(ID);
        	for(QVector<AudioStreamRangedDescription>::const_iterator ppJ=streamList.constBegin();ppJ!=streamList.constEnd() && !res;ppJ++)
			{
				const AudioStreamRangedDescription& range = *ppJ;
			
        	    if(range.mFormat.mFormatFlags & kAudioFormatFlagIsNonMixable)
				{
					res = true;
				}
			}
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AOCoreAudio::updateExclusiveModeOnDevices()
{
	m_deviceInfoMutex.lock();
	for(tint i=0;i<m_deviceInfo->noDevices();i++)
	{
		const AOQueryCoreAudio::DeviceCoreAudio& cDevice = dynamic_cast<const AOQueryCoreAudio::DeviceCoreAudio&>(m_deviceInfo->device(i));
		AOQueryCoreAudio::DeviceCoreAudio& coreDevice = const_cast<AOQueryCoreAudio::DeviceCoreAudio&>(cDevice);
		coreDevice.setHasExclusive(canDeviceSupportExclusiveMode(coreDevice.deviceID()));
	}
	m_deviceInfoMutex.unlock();
}

//-------------------------------------------------------------------------------------------

bool AOCoreAudio::isDeviceAlive(QSharedPointer<AOQueryCoreAudio::DeviceCoreAudio>& pDevice)
{
	OSStatus err;
	bool isAlive = false;
	UInt32 paramSize = sizeof(bool);
    AudioObjectPropertyAddress property = { kAudioDevicePropertyDeviceIsAlive, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMain };
	
	err = CoreAudioIF::instance()->AudioObjectGetPropertyData(pDevice->deviceID(),&property,0,0,&paramSize,&isAlive);
	if(err!=noErr)
	{
		printErrorOS("isDeviceAlive","Failed to check whether device is alive",err);
		isAlive = false;
	}
	
	if(!isAlive)
	{
		QString msg = "Audio device '" + pDevice->name() + "' is no longer alive and available";
		printToLog(msg.toUtf8().constData());
	}
	return isAlive;
}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
