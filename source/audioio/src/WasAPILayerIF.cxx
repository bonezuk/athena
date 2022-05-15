#include "audioio/inc/WasAPILayerIF.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(WasAPIIFFactory,WasAPIIF, \
                            WasAPILayerIFFactory,WasAPILayerIF, \
                            "wasapi",false)

//-------------------------------------------------------------------------------------------

WasAPILayerIF::WasAPILayerIF() : WasAPIIF(),
	m_pEnumerator()
{}

//-------------------------------------------------------------------------------------------

WasAPILayerIF::~WasAPILayerIF()
{}

//-------------------------------------------------------------------------------------------

bool WasAPILayerIF::init()
{
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

	IMMDeviceEnumerator *pEnumerator;
	HRESULT hr;
	bool res = false;
	
	done();
	
	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, reinterpret_cast<void **>(&pEnumerator));
	if(hr==S_OK && pEnumerator!=0)
	{
		IMMDeviceEnumeratorIFSPtr pEnum(new IMMDeviceEnumeratorIF(pEnumerator));
		m_pEnumerator = pEnum;
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void WasAPILayerIF::done()
{
	if(!m_pEnumerator.isNull())
	{
		IMMDeviceEnumeratorIFSPtr pEmpty;
		m_pEnumerator = pEmpty;
	}
}

//-------------------------------------------------------------------------------------------

IMMDeviceCollectionIFSPtr WasAPILayerIF::createDeviceCollectionIF(IMMDeviceCollection *pCollection)
{
	return IMMDeviceCollectionIFSPtr(new IMMDeviceCollectionIF(pCollection));
}

//-------------------------------------------------------------------------------------------

IMMDeviceIFSPtr WasAPILayerIF::createDeviceIF(IMMDevice *pDevice)
{
	return IMMDeviceIFSPtr(new IMMDeviceIF(pDevice));
}

//-------------------------------------------------------------------------------------------

IPropertyStoreIFSPtr WasAPIDeviceLayer::createPropertyStoreIF(IPropertyStore *pPropertyStore)
{
	IPropertyStoreIFSPtr pPropertyStoreIF(new IPropertyStoreIF(pPropertyStore));
	return pPropertyStoreIF;
}

//-------------------------------------------------------------------------------------------

QStringList WasAPILayerIF::enumerateDeviceIds()
{
	QStringList deviceList;
	IMMDeviceCollection *pCollection = 0;
	HRESULT hr;
	
	hr = m_pEnumerator->EnumAudioEndpoints(eRender,DEVICE_STATE_ACTIVE | DEVICE_STATE_UNPLUGGED,&pCollection);
	if(hr==S_OK && pCollection!=0)
	{
		UINT len = 0;
		IMMDeviceCollectionIFSPtr pCollectionIF = createDeviceCollectionIF(pCollection);
		
		hr = pCollectionIF->GetCount(&len);
		if(hr==S_OK)
		{
			for(UINT deviceNo=0;deviceNo<len;deviceNo++)
			{
				IMMDevice *pDevice = 0;
				
				hr = pCollectionIF->Item(deviceNo,&pDevice);
				if(hr==S_OK && pDevice!=0)
				{
					LPWSTR pDeviceName;
					IMMDeviceIFSPtr pDeviceIF = createDeviceIF(pDevice);
					
					hr = pDeviceIF->GetId(&pDeviceName);
					if(hr==S_OK && pDeviceName!=0)
					{
						QString name = QString::fromUtf16(reinterpret_cast<const ushort *>(pDeviceName));
						deviceList.append(name);
						CoTaskMemFree(pDeviceName);
					}
				}
			}
		}
	}
	return deviceList;
}

//-------------------------------------------------------------------------------------------

IMMDeviceEnumeratorIFSPtr WasAPILayerIF::enumerator()
{
	return m_pEnumerator;
}

//-------------------------------------------------------------------------------------------

WasAPIDeviceLayer *WasAPILayerIF::createDeviceInstance()
{
	return new WasAPIDeviceLayer();
}

//-------------------------------------------------------------------------------------------

QSharedPointer<WasAPIDevice> WasAPILayerIF::getDevice(const QString& devID)
{
	WasAPIDeviceSPtr pDevice;

	if(!devID.isEmpty())
	{
		WasAPIDeviceLayer *pNDevice = createDeviceInstance();
		
		if(pNDevice->init(devID))
		{
			WasAPIDeviceSPtr pDev(pNDevice);
			pDevice = pDev;
		}
		else
		{
			delete pNDevice;
		}
	}
	return pDevice;
}

//-------------------------------------------------------------------------------------------
// WasAPIDeviceLayer
//-------------------------------------------------------------------------------------------

WasAPIDeviceLayer::WasAPIDeviceLayer() : WasAPIDevice(),
	m_pDevice(),
	m_pAudioClient()
{
	WasAPIDeviceLayer::blank();
	if(!WasAPIIF::instance().isNull())
	{
		QObject::connect(WasAPIIF::instance().data(),SIGNAL(exclusiveUpdated()),this,SLOT(updateFormats()));
	}
}

//-------------------------------------------------------------------------------------------

WasAPIDeviceLayer::~WasAPIDeviceLayer()
{
	WasAPIDeviceLayer::done();
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::blank()
{
	for(int i=0;i<NUMBER_WASAPI_MAXCHANNELS;i++)
	{
		for(int j=0;j<NUMBER_WASAPI_MAXBITS;j++)
		{
			for(int k=0;k<NUMBER_WASAPI_MAXFREQUENCIES;k++)
			{
				m_formats[i][j][k] = -1;
				m_formatsShared[i][j][k] = -1;
				m_formatsExclusive[i][j][k] = -1;
			}
		}
	}	
}

//-------------------------------------------------------------------------------------------

IMMDeviceEnumeratorIFSPtr WasAPIDeviceLayer::deviceEnumerator()
{
	QSharedPointer<WasAPILayerIF> pLayer = WasAPIIF::instance().dynamicCast<WasAPILayerIF>();
	return pLayer->enumerator();
}

//-------------------------------------------------------------------------------------------

IMMDeviceIFSPtr WasAPIDeviceLayer::createDeviceIF(IMMDevice *pDevice)
{
	IMMDeviceIFSPtr pDeviceIF(new IMMDeviceIF(pDevice));
	return pDeviceIF;
}

//-------------------------------------------------------------------------------------------

IAudioClientIFSPtr WasAPIDeviceLayer::createAudioClientIF(IAudioClient *pAudioClient)
{
	IAudioClientIFSPtr pAudioClientIF(new IAudioClientIF(pAudioClient));
	return pAudioClientIF;
}

//-------------------------------------------------------------------------------------------

bool WasAPIDeviceLayer::init(const QString& devID)
{
	HRESULT hr;
	LPWSTR pDeviceName;
	IMMDevice *pDevice = 0;
	bool res = false;
	
	done();

	if(!devID.isEmpty())
	{
		pDeviceName = reinterpret_cast<LPWSTR>(CoTaskMemAlloc((devID.length() + 1) * sizeof(tuint16)));
		if(pDeviceName!=0)
		{
			memcpy(pDeviceName,devID.utf16(),devID.length() * sizeof(tuint16));
			pDeviceName[devID.length()] = 0;
			
			hr = deviceEnumerator()->GetDevice(pDeviceName,&pDevice);
			if(hr==S_OK && pDevice!=0)
			{
				IAudioClientIFSPtr pAudioClient;

				m_pDevice = createDeviceIF(pDevice);
				pAudioClient = getAudioClient();
				if(!pAudioClient.isNull())
				{
					if(!loadFormats())
					{
						blank();
					}
					queryDeviceFormatCapabilities();
					releaseAudioClient();
					res = true;
				}
			}
			CoTaskMemFree(pDeviceName);
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::done()
{
	if(!m_pDevice.isNull())
	{
		if(!WasAPIIF::instance().dynamicCast<WasAPILayerIF>().isNull())
		{
			saveFormats();
		}
	}
	if(m_pAudioClient.data()!=0)
	{
		m_pAudioClient.clear();
	}
	if(m_pDevice.data()!=0)
	{
		m_pDevice.clear();
	}
}

//-------------------------------------------------------------------------------------------

bool WasAPIDeviceLayer::isExclusive() const
{
	return WasAPIIF::isExclusive();
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::defaultWaveFormat(WAVEFORMATEX& format) const
{
	memset(&format,0,sizeof(WAVEFORMATEX));
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 2;
	format.nSamplesPerSec = 44100;
	format.nAvgBytesPerSec = 44100 * 2 * 2;
	format.nBlockAlign = 4;
	format.wBitsPerSample = 16;
	format.cbSize = 0;
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::setWaveFormat(int noChannels,int noBits,int frequency,WAVEFORMATEX& format) const
{
	int noBytes = noBits >> 3;
	format.nChannels = noChannels;
	format.nSamplesPerSec = frequency;
	format.nAvgBytesPerSec = frequency * noBytes * noChannels;
	format.nBlockAlign = noBytes * noChannels;
	format.wBitsPerSample = noBits;
}

//-------------------------------------------------------------------------------------------

WAVEFORMATEXTENSIBLE *WasAPIDeviceLayer::toWaveExtensible(WAVEFORMATEX *pFormat) const
{
	WAVEFORMATEXTENSIBLE *pExFormat = new WAVEFORMATEXTENSIBLE;
	
	memset(pExFormat,0,sizeof(WAVEFORMATEXTENSIBLE));
	pExFormat->Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	pExFormat->Format.nChannels = pFormat->nChannels;
	pExFormat->Format.nSamplesPerSec = pFormat->nSamplesPerSec;
	pExFormat->Format.nAvgBytesPerSec = pFormat->nAvgBytesPerSec;
	pExFormat->Format.nBlockAlign = pFormat->nBlockAlign;
	pExFormat->Format.wBitsPerSample = pFormat->wBitsPerSample;
	pExFormat->Format.cbSize = 22;
	
	pExFormat->Samples.wValidBitsPerSample = pFormat->wBitsPerSample;
	
	switch(pFormat->nChannels)
	{
		case 1:
			pExFormat->dwChannelMask = KSAUDIO_SPEAKER_MONO;
			break;
		case 3:
			pExFormat->dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER;
			break;
		case 4:
			pExFormat->dwChannelMask = KSAUDIO_SPEAKER_QUAD;
			break;
		case 5:
			pExFormat->dwChannelMask = KSAUDIO_SPEAKER_SURROUND;
			break;
		case 6:
			pExFormat->dwChannelMask = KSAUDIO_SPEAKER_5POINT1;
			break;
		case 7:
			pExFormat->dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT;
			break;
		case 8:
			pExFormat->dwChannelMask = KSAUDIO_SPEAKER_7POINT1;
			break;
		case 2:
		default:
			pExFormat->dwChannelMask = KSAUDIO_SPEAKER_STEREO;
			break;
	}
	
	pExFormat->SubFormat = (pFormat->wFormatTag==WAVE_FORMAT_IEEE_FLOAT) ? KSDATAFORMAT_SUBTYPE_IEEE_FLOAT : KSDATAFORMAT_SUBTYPE_PCM;
	
	return pExFormat;
}

//-------------------------------------------------------------------------------------------

bool WasAPIDeviceLayer::isFormatSupported(WAVEFORMATEX *pFormat)
{
	HRESULT hr;
	
	if(isExclusive())
	{
		hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_EXCLUSIVE,pFormat,0);
		if(hr!=S_OK && pFormat->cbSize==0)
		{
			WAVEFORMATEXTENSIBLE *pExFormat = toWaveExtensible(pFormat);
			hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_EXCLUSIVE,reinterpret_cast<const WAVEFORMATEX *>(pExFormat),0);
			delete pExFormat;
		}
	}
	else
	{
		WAVEFORMATEX *pCloseFormat = 0;
		
		hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED,pFormat,&pCloseFormat);
		if(pCloseFormat!=0)
		{
			CoTaskMemFree(pCloseFormat);
			pCloseFormat = 0;
		}

		if(hr!=S_OK && pFormat->cbSize==0)
		{
			WAVEFORMATEXTENSIBLE *pExFormat = toWaveExtensible(pFormat);
			if(pExFormat!=0)
			{
				hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED,reinterpret_cast<const WAVEFORMATEX *>(pExFormat),&pCloseFormat);
			}
			if(pCloseFormat!=0)
			{
				CoTaskMemFree(pCloseFormat);
				pCloseFormat = 0;
			}
			delete pExFormat;
		}
	}
	return (hr==S_OK);
}

//-------------------------------------------------------------------------------------------

int WasAPIDeviceLayer::getIndexOfBits(const WAVEFORMATEX *pFormat) const
{
	int index;
	
	if(pFormat->wFormatTag==e_formatFloat)
	{
		index = 4;
	}
	else
	{
		index = (pFormat->wBitsPerSample >> 3) - 1;
	}
	return index;
}

//-------------------------------------------------------------------------------------------

int WasAPIDeviceLayer::getIndexOfChannels(const WAVEFORMATEX *pFormat) const
{
	return pFormat->nChannels - 1;
}

//-------------------------------------------------------------------------------------------

int WasAPIDeviceLayer::getIndexOfFrequency(const WAVEFORMATEX *pFormat) const
{
	int index;
	
	switch(pFormat->nSamplesPerSec)
	{
		case 8000:
			index = 0;
			break;
		case 11025:
			index = 1;
			break;
		case 12000:
			index = 2;
			break;
		case 16000:
			index = 3;
			break;
		case 22050:
			index = 4;
			break;
		case 24000:
			index = 5;
			break;
		case 32000:
			index = 6;
			break;
		case 44100:
			index = 7;
			break;
		case 48000:
			index = 8;
			break;
		case 64000:
			index = 9;
			break;
		case 88200:
			index = 10;
			break;
		case 96000:
			index = 11;
			break;
		case 176400:
			index = 12;
			break;
		case 192000:
			index = 13;
			break;
		case 352800:
			index = 14;
			break;
		case 384000:
			index = 15;
			break;
		case 705600:
			index = 16;
			break;
		case 768000:
			index = 17;
			break;
		default:
			index = -1;
			break;
	}
	return index;
}

//-------------------------------------------------------------------------------------------

int WasAPIDeviceLayer::getNumberOfBitsFromIndex(int idx) const
{
	int noBits;
	
	if(idx>=0 && idx<4)
	{
		noBits = (idx + 1) << 3;
	}
	else
	{
		noBits = -1;
	}
	return noBits;
}

//-------------------------------------------------------------------------------------------

int WasAPIDeviceLayer::getNumberOfChannelsFromIndex(int idx) const
{
	return idx + 1;
}

//-------------------------------------------------------------------------------------------

int WasAPIDeviceLayer::getFrequencyFromIndex(int idx) const
{
	int freq;
	
	switch(idx)
	{
		case 0:
			freq = 8000;
			break;
		case 1:
			freq = 11025;
			break;
		case 2:
			freq = 12000;
			break;
		case 3:
			freq = 16000;
			break;
		case 4:
			freq = 22050;
			break;
		case 5:
			freq = 24000;
			break;
		case 6:
			freq = 32000;
			break;
		case 8:
			freq = 48000;
			break;
		case 9:
			freq = 64000;
			break;
		case 10:
			freq = 88200;
			break;
		case 11:
			freq = 96000;
			break;
		case 12:
			freq = 176400;
			break;
		case 13:
			freq = 192000;
			break;
		case 14:
			freq = 352800;
			break;
		case 15:
			freq = 384000;
			break;
		case 16:
			freq = 705600;
			break;
		case 17:
			freq = 768000;
			break;
			
		case 7:
		default:
			freq = 44100;
			break;
	}
	return freq;
}

//-------------------------------------------------------------------------------------------

bool WasAPIDeviceLayer::isFormat(int chIdx,int bitIdx,int freqIdx) const
{
	return (m_formats[chIdx][bitIdx][freqIdx]==1) ? true : false;
}

//-------------------------------------------------------------------------------------------

tint WasAPIDeviceLayer::getNoOfBitsFromWaveFormat(const WAVEFORMATEX *pFormat) const
{
	return pFormat->wBitsPerSample;
}

//-------------------------------------------------------------------------------------------

tint WasAPIDeviceLayer::getNoOfChannelsFromWaveFormat(const WAVEFORMATEX *pFormat) const
{
	return pFormat->nChannels;
}

//-------------------------------------------------------------------------------------------

tint WasAPIDeviceLayer::getFrequencyFromWaveFormat(const WAVEFORMATEX *pFormat) const
{
	return pFormat->nSamplesPerSec;
}

//-------------------------------------------------------------------------------------------

QString WasAPIDeviceLayer::id()
{
	LPWSTR pName = 0;
	QString id;
	
	if(m_pDevice->GetId(&pName)==S_OK && pName!=0)
	{
		id = QString::fromUtf16(reinterpret_cast<const ushort *>(pName));
	}
	if(pName!=0)
	{
		CoTaskMemFree(pName);
	}
	return id;
}

//-------------------------------------------------------------------------------------------

QString WasAPIDeviceLayer::name()
{
	HRESULT hr;
	QString deviceName;
	IPropertyStore *iProperties = 0;
	
	hr = m_pDevice->OpenPropertyStore(STGM_READ,&iProperties);
	if(hr==S_OK && iProperties!=0)
	{
		PROPVARIANT varName;
		IPropertyStoreIFSPtr pPropertyStore = createPropertyStoreIF(iProperties);
		
		PropVariantInit(&varName);
		
		hr = pPropertyStore->GetValue(PKEY_Device_FriendlyName,&varName);
		if(hr==S_OK)
		{
			deviceName = QString::fromUtf16(reinterpret_cast<const ushort *>(varName.pwszVal));
		}
		PropVariantClear(&varName);
	}
	return deviceName;
}

//-------------------------------------------------------------------------------------------

QString WasAPIDeviceLayer::settingsKey(bool exclusive)
{
	QString key = "Formats_" + id() + "_";
	key += (exclusive) ? "E" : "M";
	return key;
}

//-------------------------------------------------------------------------------------------

bool WasAPIDeviceLayer::loadFormats()
{
	bool res = (loadFormats(true) && loadFormats(false));
	updateFormats();
	return res;
}

//-------------------------------------------------------------------------------------------

bool WasAPIDeviceLayer::loadFormats(bool exclusive)
{
	QString key = settingsKey(exclusive);
	QSettings settings;
	bool res = false;
	
	if(!key.isEmpty())
	{
		QByteArray mem;
	
		settings.beginGroup("wasapi");
		if(settings.contains(key))
		{
			mem = settings.value(key).toByteArray();
			if(mem.size()==(NUMBER_WASAPI_MAXCHANNELS * NUMBER_WASAPI_MAXBITS * NUMBER_WASAPI_MAXFREQUENCIES))
			{
				res = true;
			}
		}
		settings.endGroup();
		
		if(res)
		{
			int i,j,k,idx;
			const tbyte *fmts = mem.constData();
			
			for(i=0,idx=0;i<NUMBER_WASAPI_MAXCHANNELS;i++)
			{
				for(j=0;j<NUMBER_WASAPI_MAXBITS;j++)
				{
					for(k=0;k<NUMBER_WASAPI_MAXFREQUENCIES;k++,idx++)
					{
						if(exclusive)
						{
							m_formatsExclusive[i][j][k] = static_cast<tint>(fmts[idx]);
						}
						else
						{
							m_formatsShared[i][j][k] = static_cast<tint>(fmts[idx]);
						}
					}
				}
			}
		}
		else
		{
			blank();
		}
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool WasAPIDeviceLayer::saveFormats()
{
	return (saveFormats(true) && saveFormats(false));
}

//-------------------------------------------------------------------------------------------

bool WasAPIDeviceLayer::saveFormats(bool exclusive)
{
	tint i,j,k,idx;
	tbyte fmts[NUMBER_WASAPI_MAXCHANNELS * NUMBER_WASAPI_MAXBITS * NUMBER_WASAPI_MAXFREQUENCIES];
	bool res = false;
	
	for(i=0,idx=0;i<NUMBER_WASAPI_MAXCHANNELS;i++)
	{
		for(j=0;j<NUMBER_WASAPI_MAXBITS;j++)
		{
			for(k=0;k<NUMBER_WASAPI_MAXFREQUENCIES;k++,idx++)
			{
				if(exclusive)
				{
					fmts[idx] = static_cast<tbyte>(m_formatsExclusive[i][j][k]);
				}
				else
				{
					fmts[idx] = static_cast<tbyte>(m_formatsShared[i][j][k]);
				}
			}
		}
	}
	
	QString key = settingsKey(exclusive);
	
	if(!key.isEmpty())
	{
		QByteArray mem(fmts,NUMBER_WASAPI_MAXCHANNELS * NUMBER_WASAPI_MAXBITS * NUMBER_WASAPI_MAXFREQUENCIES);
		QSettings settings;
		
		settings.beginGroup("wasapi");
		key = settingsKey(exclusive);
		settings.setValue(key,QVariant(mem));
		settings.endGroup();
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::setFrequencyInWaveFormat(int frequency,WAVEFORMATEX *pFormat) const
{
	pFormat->nSamplesPerSec = frequency;
	pFormat->nAvgBytesPerSec = frequency * pFormat->nBlockAlign;
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::setChannelsInWaveFormat(int channel,WAVEFORMATEX *pFormat) const
{
	pFormat->nChannels = channel;
	pFormat->nBlockAlign = (pFormat->wBitsPerSample >> 3) * channel;
	pFormat->nAvgBytesPerSec = pFormat->nSamplesPerSec * pFormat->nBlockAlign;
}

//-------------------------------------------------------------------------------------------

QSet<int> WasAPIDeviceLayer::queryFrequencyCapabilities()
{
	int i,j,k;
	QSet<int> frequencySet;

	for(i=0;i<NUMBER_WASAPI_MAXCHANNELS;i++)
	{
		for(j=0;j<NUMBER_WASAPI_MAXBITS;j++)
		{
			for(k=0;k<NUMBER_WASAPI_MAXFREQUENCIES;k++)
			{
				if(m_formats[i][j][k] > 0)
				{
					int freq = getFrequencyFromIndex(k);
					QSet<int>::iterator ppI = frequencySet.find(freq);
					if(ppI==frequencySet.end())
					{
						frequencySet.insert(freq);
					}
				}
			}
		}
	}
	return frequencySet;
}

//-------------------------------------------------------------------------------------------

QVector<AOQueryDevice::Channel> WasAPIDeviceLayer::queryChannelCapabilities()
{
	QVector<AOQueryDevice::Channel> channelList;
	int i,j,k,maxChs = 0;
	
	for(i=0;i<NUMBER_WASAPI_MAXCHANNELS;i++)
	{
		int noChs = getNumberOfChannelsFromIndex(i);

		for(j=0;j<NUMBER_WASAPI_MAXBITS;j++)
		{
			for(k=0;k<NUMBER_WASAPI_MAXFREQUENCIES;k++)
			{
				if(m_formats[i][j][k] > 0 && noChs > maxChs)
				{
					maxChs = noChs;
				}
			}
		}
	}
	
	for(i=0;i<maxChs;i++)
	{
		AOQueryDevice::Channel channel;
		channel.name() = QString::number(i+1);
		channelList.append(channel);		
	}
	return channelList;
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::queryDeviceFormatCapabilities()
{
	bool exclusiveFlag = isExclusive();
	int i,j,k,l;
	
	for(l=0;l<2;l++)
	{
		WasAPIIF::setExclusive((!l) ? true : false);
		
		for(i=0;i<NUMBER_WASAPI_MAXCHANNELS;i++)
		{
			for(j=0;j<NUMBER_WASAPI_MAXBITS;j++)
			{
				for(k=0;k<NUMBER_WASAPI_MAXFREQUENCIES;k++)
				{
					hasIndexedFormatWithUpdate(j,i,k);
				}
			}
		}
	}
	WasAPIIF::setExclusive(exclusiveFlag);
}

//-------------------------------------------------------------------------------------------

bool WasAPIDeviceLayer::hasIndexedFormatWithUpdate(tint bitIdx,tint chIdx,tint freqIdx)
{
	bool res = false;
	
	if(m_formats[chIdx][bitIdx][freqIdx] < 0)
	{
		WAVEFORMATEX format;
		
		setWaveFormatFromIndex(bitIdx,chIdx,freqIdx,format);
		
		m_formats[chIdx][bitIdx][freqIdx] = (isFormatSupported(&format)) ? 1 : 0;
		if(isExclusive())
		{
			m_formatsExclusive[chIdx][bitIdx][freqIdx] = m_formats[chIdx][bitIdx][freqIdx];
		}
		else
		{
			m_formatsShared[chIdx][bitIdx][freqIdx] = m_formats[chIdx][bitIdx][freqIdx];
		}
	}
	
	if(m_formats[chIdx][bitIdx][freqIdx] > 0)
	{
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::updateFormats()
{
	int i,j,k;
	bool flag = WasAPIIF::isExclusive();
	
	for(i=0;i<NUMBER_WASAPI_MAXCHANNELS;i++)
	{
		for(j=0;j<NUMBER_WASAPI_MAXBITS;j++)
		{
			for(k=0;k<NUMBER_WASAPI_MAXFREQUENCIES;k++)
			{
				m_formats[i][j][k] = (flag) ? m_formatsExclusive[i][j][k] : m_formatsShared[i][j][k];
			}
		}
	}	
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::setWaveFormatFromIndex(tint bitIdx,tint chIdx,tint freqIdx,WAVEFORMATEX& format)
{
	int noChannels = getNumberOfChannelsFromIndex(chIdx);
	int noBits = getNumberOfBitsFromIndex(bitIdx);
	int frequency = getFrequencyFromIndex(freqIdx);
	
	defaultWaveFormat(format);
	if(noBits > 0)
	{
		setWaveFormat(noChannels,noBits,frequency,format);
	}
	else
	{
		format.wFormatTag = e_formatFloat;
		setWaveFormat(noChannels,32,frequency,format);
	}
}

//-------------------------------------------------------------------------------------------

FormatDescription WasAPIDeviceLayer::descriptionFromWaveFormat(const WAVEFORMATEX *pFormat)
{
	FormatDescription desc;

	desc.setBitsIndex(getIndexOfBits(pFormat));
	desc.setChannelsIndex(getIndexOfChannels(pFormat));
	desc.setFrequencyIndex(getIndexOfFrequency(pFormat));
	return desc;
}

//-------------------------------------------------------------------------------------------

WAVEFORMATEX *WasAPIDeviceLayer::descriptionToWaveFormat(const FormatDescription& desc)
{
	WAVEFORMATEX *pFormat = new WAVEFORMATEX;
	setWaveFormatFromIndex(desc.bitsIndex(),desc.channelsIndex(),desc.frequencyIndex(),*pFormat);
	return pFormat;
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::populateFormatsSupported(FormatsSupported& support)
{
	support.clear();

	for(tint i=0;i<NUMBER_WASAPI_MAXCHANNELS;i++)
	{
		for(tint j=0;j<NUMBER_WASAPI_MAXBITS;j++)
		{
			for(tint k=0;k<NUMBER_WASAPI_MAXFREQUENCIES;k++)
			{
				if(hasIndexedFormatWithUpdate(j,i,k))
				{
					FormatDescription desc;
					
					desc.setBitsIndex(j);
					desc.setChannelsIndex(i);
					desc.setFrequencyIndex(k);
					
					support.add(desc);
				}
			}		
		}
	}
}

//-------------------------------------------------------------------------------------------

WAVEFORMATEX *WasAPIDeviceLayer::findClosestFormatType(const WAVEFORMATEX *pFormat)
{
	WAVEFORMATEX *pCloseFormat = 0;
	FormatDescription desc;
	
	if(pFormat!=0)
	{
		desc = descriptionFromWaveFormat(pFormat);
		pCloseFormat = findClosestWaveFormatFromDescription(desc);
	}
	return pCloseFormat;
}

//-------------------------------------------------------------------------------------------

WAVEFORMATEX *WasAPIDeviceLayer::findClosestWaveFormatFromDescription(const FormatDescription& sourceDesc)
{
	WAVEFORMATEX *pCloseFormat = 0;
	FormatsSupported support;
	FormatDescription descClose;
	
	populateFormatsSupported(support);
	if(FormatDescriptionUtils::findClosestFormatType(sourceDesc,support,descClose))
	{
		pCloseFormat = descriptionToWaveFormat(descClose);
	}
	return pCloseFormat;
}

//-------------------------------------------------------------------------------------------

WAVEFORMATEX *WasAPIDeviceLayer::findClosestSupportedFormat(const FormatDescription& sourceDesc)
{
	if(m_pAudioClient.isNull())
	{
		return 0;
	}

	WAVEFORMATEX *pFormat = findClosestWaveFormatFromDescription(sourceDesc);
	
	if(pFormat!=0)
	{
		HRESULT hr;
		
		if(isExclusive())
		{
			hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_EXCLUSIVE,pFormat,0);
			if(hr!=S_OK)
			{
				WAVEFORMATEX *pDelFormat = pFormat;
			
				if(pFormat->cbSize==0)
				{
					WAVEFORMATEXTENSIBLE *pExFormat = toWaveExtensible(pFormat);
					
					hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_EXCLUSIVE,reinterpret_cast<const WAVEFORMATEX *>(pExFormat),0);
					if(hr==S_OK)
					{
						pFormat = reinterpret_cast<WAVEFORMATEX *>(pExFormat);
					}
					else
					{
						delete pExFormat;
						pFormat = 0;						
					}
				}
				else
				{
					pFormat = 0;
				}
				delete pDelFormat;
			}
		}
		else
		{
			WAVEFORMATEX *pCloseFormat = 0;
			
			hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED,pFormat,&pCloseFormat);
			if(pCloseFormat!=0)
			{
				CoTaskMemFree(pCloseFormat);
				pCloseFormat = 0;
			}

			if(hr!=S_OK)
			{
				WAVEFORMATEX *pDelFormat = pFormat;
			
				if(pFormat->cbSize==0)
				{
					WAVEFORMATEXTENSIBLE *pExFormat = toWaveExtensible(pFormat);
				
					hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED,reinterpret_cast<const WAVEFORMATEX *>(pExFormat),&pCloseFormat);
					if(pCloseFormat!=0)
					{
						CoTaskMemFree(pCloseFormat);
						pCloseFormat = 0;
					}					
					
					if(hr==S_OK)
					{
						pFormat = reinterpret_cast<WAVEFORMATEX *>(pExFormat);
					}
					else
					{
						delete pExFormat;
						pFormat = 0;						
					}
				}
				else
				{
					pFormat = 0;
				}
				delete pDelFormat;
			}
		}
	}
	return pFormat;
}

//-------------------------------------------------------------------------------------------

IAudioClientIFSPtr WasAPIDeviceLayer::getAudioClient()
{
	if(m_pAudioClient.isNull() && !m_pDevice.isNull())
	{
		HRESULT hr;
		IAudioClient *pAudioClient;
		const IID IDOF_IAudioClient = __uuidof(IAudioClient);
		
		hr = m_pDevice->Activate(IDOF_IAudioClient,CLSCTX_ALL,0,reinterpret_cast<void **>(&pAudioClient));
		if(hr==S_OK && pAudioClient!=0)
		{
			m_pAudioClient = createAudioClientIF(pAudioClient);
		}
	}
	return m_pAudioClient;
}

//-------------------------------------------------------------------------------------------

void WasAPIDeviceLayer::releaseAudioClient()
{
	if(!m_pAudioClient.isNull())
	{
		m_pAudioClient.clear();
	}
}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
