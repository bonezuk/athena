#include "audioio/inc/ASIOData.h"
#include "engine/inc/FormatTypeFromFloat.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

ASIOData::ASIOData() : engine::RData(),
	m_sampleType(ASIOSTFloat32LSB),
	m_asioDataArray(0),
	m_convertFlag(false),
	m_volume(1.0)
{}

//-------------------------------------------------------------------------------------------

ASIOData::ASIOData(tint len,tint inChannel,tint outChannel) : engine::RData(len,inChannel,outChannel),
	m_sampleType(ASIOSTFloat32LSB),
	m_asioDataArray(0),
	m_convertFlag(false),
	m_volume(1.0)
{}

//-------------------------------------------------------------------------------------------

ASIOData::ASIOData(const engine::AData& rhs) : engine::RData(),
	m_sampleType(ASIOSTFloat32LSB),
	m_asioDataArray(0),
	m_convertFlag(false),
	m_volume(1.0)
{
	ASIOData::copy(rhs);
}

//-------------------------------------------------------------------------------------------

ASIOData::~ASIOData()
{
	if(m_asioDataArray!=0)
	{
		tint i;
		
		for(i=0;i<m_noOutChannels;++i)
		{
			::free(m_asioDataArray[i]);
		}
		::free(m_asioDataArray);
		m_asioDataArray = 0;
	}
}

//-------------------------------------------------------------------------------------------

void ASIOData::copy(const engine::AData& rhs)
{
	engine::RData::copy(rhs);
	try
	{
		tint i;
		const ASIOData& d = dynamic_cast<const ASIOData&>(rhs);
		
		asioAllocate();
		for(i=0;i<m_noOutChannels;++i)
		{
			::memcpy(m_asioDataArray[i],d.m_asioDataArray[i],length() * getSampleSize());
		}
		m_convertFlag = d.m_convertFlag;
		m_volume = d.m_volume;
	}
	catch(...) {}
}

//-------------------------------------------------------------------------------------------

void ASIOData::asioAllocate()
{
	tint i;
	
	if(m_asioDataArray!=0)
	{
		for(i=0;i<m_noOutChannels;++i)
		{
			::free(m_asioDataArray[i]);
		}
		::free(m_asioDataArray);
	}
	
	m_asioDataArray = reinterpret_cast<void **>(::malloc(sizeof(void *) * m_noOutChannels));
	if(m_asioDataArray != 0)
	{
		for(i = 0; i < m_noOutChannels; ++i)
		{
			m_asioDataArray[i] = ::malloc(length() * getSampleSize());
		}
	}
}

//-------------------------------------------------------------------------------------------

void *ASIOData::asioData(tint chIdx,tint pIdx)
{
	return const_cast<void *>(asioDataI(chIdx,pIdx));
}

//-------------------------------------------------------------------------------------------

const void *ASIOData::asioDataConst(tint chIdx,tint pIdx) const
{
	return asioDataI(chIdx,pIdx);
}

//-------------------------------------------------------------------------------------------

const void *ASIOData::asioDataI(tint chIdx,tint pIdx) const
{
	if(chIdx<m_noOutChannels && pIdx<noParts())
	{
		const tubyte *mem = reinterpret_cast<const tubyte *>(m_asioDataArray[chIdx]);
		mem = &mem[ m_parts.at(pIdx).offsetConst() * getSampleSize() ];
		return reinterpret_cast<const void *>(mem);
	}
	else
	{
		return 0;
	}
}

//-------------------------------------------------------------------------------------------

void ASIOData::setSampleType(ASIOSampleType t)
{
	m_sampleType = t;
	asioAllocate();
}

//-------------------------------------------------------------------------------------------

tint ASIOData::getSampleSize() const
{
	return ASIODriverService::getSampleSize(m_sampleType);
}

//-------------------------------------------------------------------------------------------

void ASIOData::setVolume(sample_t v)
{
	m_volume = v;
}

//-------------------------------------------------------------------------------------------

void ASIOData::reset()
{
	engine::RData::reset();
	m_convertFlag = false;
}

//-------------------------------------------------------------------------------------------

void ASIOData::convert()
{
	if(!m_convertFlag)
	{
		tint i,j,pLen,amount;
		QVector<tint> amountS(m_noOutChannels,0);
	
		for(i=0;i<noParts();++i)
		{
			sample_t *in = partDataOut(i);
		
			pLen = part(i).length();
			for(j=0;j<m_noOutChannels;++j)
			{
				amount = copyToBuffer(&in[j],pLen,amountS.at(j),j);
				Q_ASSERT(pLen == amount);
				amountS[j] += amount;
			}
		}
		m_convertFlag = true;
	}
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt16LSB(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint16 *out = reinterpret_cast<tint16 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		engine::write16BitsLittleEndianFromSample(x,reinterpret_cast<tbyte *>(&out[j]));
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt16MSB(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint16 *out = reinterpret_cast<tint16 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		engine::write16BitsBigEndianFromSample(x,reinterpret_cast<tbyte *>(&out[j]));
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt24LSB(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tbyte *out = reinterpret_cast<tbyte *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset*3;i<len;i++,j+=3,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		engine::write24BitsLittleEndianFromSample(x,reinterpret_cast<tbyte *>(&out[j]));
	}
	return (j - (oOffset * 3)) / 3;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt24MSB(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tbyte *out = reinterpret_cast<tbyte *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset*3;i<len;i++,j+=3,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		engine::write24BitsBigEndianFromSample(x,reinterpret_cast<tbyte *>(&out[j]));
	}
	return (j - (oOffset * 3)) / 3;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt32LSB(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint32 *out = reinterpret_cast<tint32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		engine::write32BitsLittleEndianFromSample(x,reinterpret_cast<tbyte *>(&out[j]));
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt32MSB(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint32 *out = reinterpret_cast<tint32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		engine::write32BitsBigEndianFromSample(x,reinterpret_cast<tbyte *>(&out[j]));
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferFloat32LSB(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tfloat32 *out = reinterpret_cast<tfloat32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		tfloat32 x = static_cast<tfloat32>(volumeAndClip(*src));
		engine::writeNative32BitsAsLittleEndian(reinterpret_cast<const tbyte *>(&x),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferFloat32MSB(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tfloat32 *out = reinterpret_cast<tfloat32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		tfloat32 x = static_cast<tfloat32>(volumeAndClip(*src));
		engine::writeNative32BitsAsBigEndian(reinterpret_cast<const tbyte *>(&x),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferFloat64LSB(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tfloat64 *out = reinterpret_cast<tfloat64 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		tfloat64 x = static_cast<tfloat64>(volumeAndClip(*src));
		engine::writeNative64BitsAsLittleEndian(reinterpret_cast<const tbyte *>(&x),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferFloat64MSB(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tfloat64 *out = reinterpret_cast<tfloat64 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		tfloat64 x = static_cast<tfloat64>(volumeAndClip(*src));
		engine::writeNative64BitsAsBigEndian(reinterpret_cast<const tbyte *>(&x),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt32LSB16(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint32 *out = reinterpret_cast<tint32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		tint32 y = engine::sampleToInteger(x,16);
		engine::writeNative32BitsAsLittleEndian(reinterpret_cast<const tbyte *>(&y),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt32MSB16(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint32 *out = reinterpret_cast<tint32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		tint32 y = engine::sampleToInteger(x,16);
		engine::writeNative32BitsAsBigEndian(reinterpret_cast<const tbyte *>(&y),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt32LSB18(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint32 *out = reinterpret_cast<tint32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		tint32 y = engine::sampleToInteger(x,18);
		engine::writeNative32BitsAsLittleEndian(reinterpret_cast<const tbyte *>(&y),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt32MSB18(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint32 *out = reinterpret_cast<tint32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		tint32 y = engine::sampleToInteger(x,18);
		engine::writeNative32BitsAsBigEndian(reinterpret_cast<const tbyte *>(&y),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt32LSB20(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint32 *out = reinterpret_cast<tint32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		tint32 y = engine::sampleToInteger(x,20);
		engine::writeNative32BitsAsLittleEndian(reinterpret_cast<const tbyte *>(&y),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt32MSB20(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint32 *out = reinterpret_cast<tint32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		tint32 y = engine::sampleToInteger(x,20);
		engine::writeNative32BitsAsBigEndian(reinterpret_cast<const tbyte *>(&y),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt32LSB24(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint32 *out = reinterpret_cast<tint32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		tint32 y = engine::sampleToInteger(x,24);
		engine::writeNative32BitsAsLittleEndian(reinterpret_cast<const tbyte *>(&y),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}

//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBufferInt32MSB24(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint i,j;
	tint32 *out = reinterpret_cast<tint32 *>(m_asioDataArray[chIndex]);
	
	for(i=0,j=oOffset;i<len;i++,j++,src+=m_noOutChannels)
	{
		sample_t x = volumeAndClip(*src);
		tint32 y = engine::sampleToInteger(x,24);
		engine::writeNative32BitsAsBigEndian(reinterpret_cast<const tbyte *>(&y),reinterpret_cast<tbyte *>(&out[j]),1);
	}
	return j - oOffset;
}


//-------------------------------------------------------------------------------------------

tint ASIOData::copyToBuffer(const sample_t *src,tint len,tint oOffset,tint chIndex)
{
	tint amount;
	
	switch(m_sampleType)
	{
		case ASIOSTInt16LSB:
			amount = copyToBufferInt16LSB(src,len,oOffset,chIndex);
			break;
		
		case ASIOSTInt16MSB:
			amount = copyToBufferInt16MSB(src,len,oOffset,chIndex);
			break;
		
		case ASIOSTInt24LSB:
			amount = copyToBufferInt24LSB(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTInt24MSB:
			amount = copyToBufferInt24MSB(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTInt32LSB:
			amount = copyToBufferInt32LSB(src,len,oOffset,chIndex);
			break;

		case ASIOSTInt32MSB:
			amount = copyToBufferInt32MSB(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTFloat32LSB:
			amount = copyToBufferFloat32LSB(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTFloat32MSB:
			amount = copyToBufferFloat32MSB(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTFloat64LSB:
			amount = copyToBufferFloat64LSB(src,len,oOffset,chIndex);
			break;

		case ASIOSTFloat64MSB:
			amount = copyToBufferFloat64MSB(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTInt32LSB16:
			amount = copyToBufferInt32LSB16(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTInt32MSB16:
			amount = copyToBufferInt32MSB16(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTInt32LSB18:
			amount = copyToBufferInt32LSB18(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTInt32MSB18:
			amount = copyToBufferInt32MSB18(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTInt32LSB20:
			amount = copyToBufferInt32LSB20(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTInt32MSB20:
			amount = copyToBufferInt32MSB20(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTInt32LSB24:
			amount = copyToBufferInt32LSB24(src,len,oOffset,chIndex);
			break;
			
		case ASIOSTInt32MSB24:
			amount = copyToBufferInt32MSB24(src,len,oOffset,chIndex);
			break;
			
		default:
			amount = -1;
			break;
	}
	return amount;
}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace orcus
//-------------------------------------------------------------------------------------------
