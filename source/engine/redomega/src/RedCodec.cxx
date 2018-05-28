#include "engine/redomega/inc/RedCodec.h"
#include "engine/redomega/inc/ALACCafContainer.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace redomega
{
//-------------------------------------------------------------------------------------------
// RedCodecInitialize
//-------------------------------------------------------------------------------------------

RedCodecInitialize::RedCodecInitialize()
{}

//-------------------------------------------------------------------------------------------

RedCodecInitialize::~RedCodecInitialize()
{}

//-------------------------------------------------------------------------------------------

void RedCodecInitialize::start()
{}

//-------------------------------------------------------------------------------------------

void RedCodecInitialize::end()
{}

//-------------------------------------------------------------------------------------------
// RedCodec
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(CodecFactory,Codec, \
                            RedCodecFactory,RedCodec, \
                            "caf",false)

//-------------------------------------------------------------------------------------------

RedCodec::RedCodec(QObject *parent) : engine::Codec(e_codecALAC,parent),
	m_fileStream(0),
	m_file(0),
	m_container(0),
	m_decoder(0),
	m_outBuffer(0),
	m_outBufferSize(0),
	m_currentFrame(0),
	m_state(-1),
	m_time(),
	m_outOffset(0),
	m_outLen(0)
{}

//-------------------------------------------------------------------------------------------

RedCodec::~RedCodec()
{
	try
	{
		RedCodec::close();
	}
	catch(...) {}
}

//-------------------------------------------------------------------------------------------

void RedCodec::printError(const tchar *strR,const tchar *strE) const
{
	common::Log::g_Log << "RedCodec::" << strR << " - " << strE << "." << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool RedCodec::open(const QString& name)
{
	bool res = false;
	
	close();
	
	m_fileStream = new common::BIOTimeCachedStream;
	
	if(m_fileStream->open(name))
	{
		m_file = new IOFile(m_fileStream);
		m_container = new ALACCafContainer();
		if(m_container->init(m_file))
		{
			const ALACDescription& desc = m_container->description();
			
			m_decoder = new ALACDecoder(m_container);
			if(m_decoder->init())
			{
				m_outBufferSize = desc.framesPerPacket();
				m_outBuffer = new sample_t [m_outBufferSize * m_container->config().numChannels()];
				m_currentFrame = 0;
				m_state = 0;
				m_time = 0.0;
				m_outOffset = 0;
				m_outLen = 0;
				res = true;
			}
			else
			{
				printError("open","Failed to initialize decoder");
			}
		}
		else
		{
			printError("open","Could not find required parts in CAF file");
		}
	}
	else
	{
		printError("open","Failed to open given stream file");
	}
	
	if(!res)
	{
		close();
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void RedCodec::close()
{
	if(m_decoder!=0)
	{
		delete m_decoder;
		m_decoder = 0;
	}
	if(m_container!=0)
	{
		delete m_container;
		m_container = 0;
	}
	if(m_file!=0)
	{
		m_file->close();
		delete m_file;
		m_file = 0;
	}
	if(m_fileStream!=0)
	{
		delete m_fileStream;
		m_fileStream = 0;
	}
	if(m_outBuffer!=0)
	{
		delete m_outBuffer;
		m_outBuffer = 0;
	}
	m_state = -1;
}

//-------------------------------------------------------------------------------------------

bool RedCodec::init()
{
	if(m_fileStream!=0)
	{
		m_fileStream->setBitrate(bitrate());
	}
	return true;
}

//-------------------------------------------------------------------------------------------

bool RedCodec::next(AData& data)
{
	tint i,len;
	sample_t *buffer;
	bool res = true;
	engine::RData& rData = dynamic_cast<engine::RData&>(data);
	
	if(!rData.noParts())
	{
		data.start() = m_time;
	}
	
	if(m_state>=0)
	{
		engine::RData::Part *part = &(rData.nextPart());
		
		buffer = rData.partData(rData.noParts() - 1);
		part->start() = m_time;
		
		i = 0;
		len = rData.rLength();
		
		while(i<len && res)
		{
			switch(m_state)
			{
				case 0:
					{
						const ALACFrameIndex& fIndex = m_container->frameIndex();
						
						if(m_currentFrame < fIndex.list().size())
						{
							if(m_file->seek(m_container->dataOffset() + fIndex.list().at(m_currentFrame).first,File::e_startPosition))
							{
								tint size = fIndex.list().at(m_currentFrame).second;
								common::Array<tubyte,tubyte> arrA;
								
								arrA.SetSize(size);
								if(m_file->read(arrA.GetData(),arrA.GetSize())==size)
								{
									ALACSequence *seq;
									QSharedPointer<common::Array<tubyte,tubyte> > arrB(new common::Array<tubyte,tubyte>());
									
									SequenceMemory::generateArray(arrA.GetData(),arrA.GetSize(),arrB);
									seq = new ALACSequence(arrB);
									m_outLen = m_decoder->decode(seq,m_outBuffer,m_outBufferSize);
									if(m_outLen>=0)
									{
										m_outLen /= m_container->config().numChannels();
										m_state = 1;
										m_currentFrame++;
									}
									else
									{
										res = false;
									}
									delete seq;
								}
								else
								{
									res = false;
								}
							}
							else
							{
								res = false;
							}
						}
						else
						{
							part->end() = m_time;
							part->done() = true;
							m_state = -1;
							res = false;
						}
						m_fileStream->springCleanTheCache();
					}
					break;
					
				case 1:
					{
						tint amount;
						
						amount = len - i;
						if(amount > (m_outLen - m_outOffset))
						{
							amount = m_outLen - m_outOffset;
						}
						if(amount>0)
						{
							tint j,k,idx,noChs = m_container->config().numChannels();
							
							for(j=0,idx=m_outOffset*noChs;j<amount;j++,m_outOffset++,idx+=noChs)
							{
								for(k=0;k<noChs;k++)
								{
									*buffer++ = m_outBuffer[idx + k];
								}
							}
							i += amount;
							m_time += static_cast<tfloat64>(amount) / static_cast<tfloat64>(frequency());
						}
						
						if(m_outOffset>=m_outLen)
						{
							m_outOffset = 0;
							m_state = 0;
						}
					}
					break;
			}
		}
		
		part->length() = i;
		part->end() = m_time;
		part->done() = true;
		data.end() = m_time;
	}
	else
	{
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool RedCodec::isSeek() const
{
	if(m_container!=0 && m_container->frameIndex().list().size()>0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//-------------------------------------------------------------------------------------------

bool RedCodec::seek(const common::TimeStamp& v)
{
	tfloat64 sTime = static_cast<tfloat64>(v);
	bool res = false;
	
	if(m_container!=0)
	{
		tint fSize = m_container->description().framesPerPacket();
		tfloat64 fPerSecond = static_cast<tfloat64>(m_container->config().sampleRate()) / static_cast<tfloat64>(fSize);
		tfloat64 fPosition = fPerSecond * sTime;
		tint fIndex = static_cast<tint>(fPosition);
		const ALACFrameIndex& frameIndex = m_container->frameIndex();
		
		if(fIndex<frameIndex.list().size())
		{
			m_currentFrame = fIndex;
			fPosition = fPosition - ::floor(fPosition);
			m_outOffset = static_cast<tint>(fPosition * static_cast<tfloat64>(m_outBufferSize / m_container->config().numChannels()));
			res = true;
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool RedCodec::isComplete() const
{
	return false;
}

//-------------------------------------------------------------------------------------------

bool RedCodec::isRemote() const
{
	return false;
}

//-------------------------------------------------------------------------------------------

bool RedCodec::isBuffered(tfloat32& percent)
{
	return false;
}

//-------------------------------------------------------------------------------------------

tint RedCodec::bitrate() const
{
	tint rate = 0;
	
	if(m_container!=0)
	{
		rate = m_container->config().avgBitRate();
		if(!rate)
		{
			tfloat64 l = static_cast<tfloat64>(length());
			rate = static_cast<tint>((static_cast<tfloat64>(m_fileStream->size()) * 8.0) / l);
		}
	}
	return rate;
}

//-------------------------------------------------------------------------------------------

tint RedCodec::frequency() const
{
	if(m_container!=0)
	{
		return m_container->config().sampleRate();
	}
	else
	{
		return 44100;
	}
}

//-------------------------------------------------------------------------------------------

tint RedCodec::noChannels() const
{
	if(m_container!=0)
	{
		return m_container->config().numChannels();
	}
	else
	{
		return 2;
	}
}

//-------------------------------------------------------------------------------------------

common::TimeStamp RedCodec::length() const
{
	common::TimeStamp l;
	
	if(m_container!=0)
	{
		l = (static_cast<tfloat64>(m_container->config().sampleRate()) / static_cast<tfloat64>(m_container->description().framesPerPacket())) * static_cast<tfloat64>(m_container->frameIndex().list().size());
	}
	return l;
}

//-------------------------------------------------------------------------------------------
} // namespace redomega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
