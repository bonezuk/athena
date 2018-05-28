#include "engine/monkeyomega/inc/MonkeyCodec.h"

#include "monkey/All.h"
#if defined(ORCUS_WIN32)
#include "monkey/MACDll.h"
#else
#include "monkey/MACLib.h"
#endif

#include "engine/inc/FormatType.h"

#if defined(OMEGA_MAC_STORE)
#include "common/inc/SBService.h"
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace monkeyomega
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(CodecFactory,Codec, \
                            MonkeyCodecFactory,MonkeyCodec, \
                            "ape",false)

//-------------------------------------------------------------------------------------------

MonkeyCodec::MonkeyCodec(QObject *parent) : InterleavedCodec(e_codecAPE,parent),
	m_pMonkey(new MonkeyIF),
	m_sampleRate(44100),
	m_noChannels(2),
	m_bitsPerSample(0),
	m_bytesPerSample(0),
	m_buffer(0),
	m_name()
{}

//-------------------------------------------------------------------------------------------

MonkeyCodec::~MonkeyCodec()
{}

//-------------------------------------------------------------------------------------------

void MonkeyCodec::printError(const tchar *strR,const tchar *strE) const
{
	common::Log::g_Log << "MonkeyCodec::" << strR << " - " << strE << "." << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool MonkeyCodec::open(const QString& name)
{
	bool res = false;
	
	close();

	m_name = name;
	
#if defined(OMEGA_MAC_STORE)
	common::SBBookmarkPtr sbBookmark = common::SBBookmark::get();
	if(sbBookmark->has(name,true))
	{
		sbBookmark->checkOut(name,true);
	}
#endif

	if(m_pMonkey->create(name))
	{
        int nAlign = m_pMonkey->getInfo(APE::APE_INFO_BLOCK_ALIGN);
		if(nAlign > 0)
		{
			m_buffer = new char [c_BufferLength * nAlign];
			if(m_buffer!=0)
			{
                m_sampleRate = m_pMonkey->getInfo(APE::APE_INFO_SAMPLE_RATE);
				if(m_sampleRate > 0)
				{
                    m_noChannels = m_pMonkey->getInfo(APE::APE_INFO_CHANNELS);
					if(m_noChannels>0 && m_noChannels<=8)
					{
                        m_bitsPerSample = m_pMonkey->getInfo(APE::APE_INFO_BITS_PER_SAMPLE);
						if(m_bitsPerSample>0 && m_bitsPerSample<=32)
						{
                            m_bytesPerSample = m_pMonkey->getInfo(APE::APE_INFO_BYTES_PER_SAMPLE);
							if(m_bytesPerSample>0 && m_bytesPerSample<=4)
							{
								m_time = 0;
								res = true;
							}
						}
					}
				}
			}
		}
	}
	if(!res)
	{
		close();
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void MonkeyCodec::close()
{
	if(m_buffer!=0)
	{
		delete [] m_buffer;
		m_buffer = 0;
	}
	m_pMonkey->destroy();
	
#if defined(OMEGA_MAC_STORE)
	common::SBBookmarkPtr sbBookmark = common::SBBookmark::get();
	sbBookmark->checkIn(m_name,true);
#endif
}

//-------------------------------------------------------------------------------------------

bool MonkeyCodec::decodeNextPacket(int& outLen)
{
	outLen = 0;
	return (m_pMonkey->getData(m_buffer,c_BufferLength,&m_outLen)==ERROR_SUCCESS && m_outLen>0) ? true : false;
}

//-------------------------------------------------------------------------------------------

char *MonkeyCodec::getPacketBuffer()
{
	return m_buffer;
}

//-------------------------------------------------------------------------------------------

int MonkeyCodec::bytesPerSample()
{
	return m_bytesPerSample;
}

//-------------------------------------------------------------------------------------------

sample_t MonkeyCodec::readSample(char *buffer)
{
	return static_cast<sample_t>(readSampleLittleEndian(buffer,m_bitsPerSample));
}

//-------------------------------------------------------------------------------------------

bool MonkeyCodec::isSeek() const
{
	return true;
}

//-------------------------------------------------------------------------------------------

bool MonkeyCodec::seek(const common::TimeStamp& v)
{
	bool res = false;
	int nBlockOffset = ::floor(static_cast<tfloat64>(v) * static_cast<tfloat64>(m_sampleRate));
	
	if(m_pMonkey->seek(nBlockOffset))
	{
		m_state = 0;
		m_time = static_cast<tfloat64>(nBlockOffset) / static_cast<tfloat64>(m_sampleRate);
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool MonkeyCodec::isComplete() const
{
	return false;
}

//-------------------------------------------------------------------------------------------

bool MonkeyCodec::isRemote() const
{
	return false;
}

//-------------------------------------------------------------------------------------------

bool MonkeyCodec::isBuffered(tfloat32& percent)
{
	return false;
}

//-------------------------------------------------------------------------------------------

tint MonkeyCodec::bitrate() const
{
    return (!m_pMonkey.isNull()) ? (m_pMonkey->getInfo(APE::APE_DECOMPRESS_AVERAGE_BITRATE,0,0) * 1000) : 0;
}

//-------------------------------------------------------------------------------------------

tint MonkeyCodec::frequency() const
{
	return m_sampleRate;
}

//-------------------------------------------------------------------------------------------

tint MonkeyCodec::noChannels() const
{
	return m_noChannels;
}

//-------------------------------------------------------------------------------------------

common::TimeStamp MonkeyCodec::length() const
{
	tfloat64 t = static_cast<tfloat64>(m_pMonkey->getInfo(APE::APE_INFO_LENGTH_MS,0,0)) / 1000.0;
	common::TimeStamp tLen(t);
	return tLen;
}

//-------------------------------------------------------------------------------------------
} // namespace monkeyomega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
