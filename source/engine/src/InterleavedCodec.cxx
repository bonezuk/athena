#include "engine/inc/InterleavedCodec.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace engine
{
//-------------------------------------------------------------------------------------------

InterleavedCodec::InterleavedCodec(Codec::CodecType type,QObject *parent) : Codec(type,parent),
	m_state(-1),
	m_outLen(0),
	m_outOffset(0),
	m_pBuffer(0),
	m_time(0)
{}

//-------------------------------------------------------------------------------------------

InterleavedCodec::~InterleavedCodec()
{}

//-------------------------------------------------------------------------------------------

bool InterleavedCodec::init()
{
	m_initFlag = true;
	m_state = 0;
	return true;
}

//-------------------------------------------------------------------------------------------

bool InterleavedCodec::next(AData& data)
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
						if(decodeNextPacket(m_outLen))
						{
							m_pBuffer = getPacketBuffer();
							m_state = 1;
							m_outOffset = 0;
						}
						else
						{
							m_state = -1;
							res = false;
						}
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
						if(amount > 0)
						{
							readDecodedData(buffer,i,amount);
							
							m_outOffset += amount;
							i += amount;
							m_time += static_cast<tfloat64>(amount) / static_cast<tfloat64>(frequency());
						}
						if(m_outOffset >= m_outLen)
						{
							m_state = 0;
						}
					}
					break;

				default:
					res = false;
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

void InterleavedCodec::readDecodedData(sample_t *output,tint sampleOffset,tint amount)
{
	tint noChs = noChannels();
	tint aTotal = amount * noChs;
	tint offset = sampleOffset * noChs;
	
	for(int j=0;j<aTotal;j++)
	{
		output[offset + j] = readSample(m_pBuffer);
		m_pBuffer += bytesPerSample();
	}
}

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace omega
//-------------------------------------------------------------------------------------------
