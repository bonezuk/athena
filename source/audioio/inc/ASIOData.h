//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_AUDIOIO_ASIODATA_H
#define __OMEGA_AUDIOIO_ASIODATA_H
//-------------------------------------------------------------------------------------------

#include "engine/inc/RData.h"
#include "audioio/inc/ASIODriver.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT ASIOData : public engine::RData
{
	public:
		ASIOData();
		ASIOData(tint len,tint inChannel,tint outChannel);
		ASIOData(const engine::AData& rhs);
		virtual ~ASIOData();
		
		virtual void *asioData(tint chIdx,tint pIdx);
		virtual const void *asioDataConst(tint chIdx,tint pIdx) const;
		
		virtual void setSampleType(ASIOSampleType t);
		virtual tint getSampleSize() const;
		
		virtual void setVolume(sample_t v);
		virtual void convert();
		virtual void reset();
		
	protected:
	
		ASIOSampleType m_sampleType;
		void **m_asioDataArray;
		
		bool m_convertFlag;
		sample_t m_volume;
		
		virtual void copy(const engine::AData& rhs);
		
		virtual void asioAllocate();
		virtual const void *asioDataI(tint chIdx,tint pIdx) const;
		
		virtual tint copyToBuffer(const sample_t *src,tint len,tint oOffset,tint chIndex);
		
	private:
	
		sample_t volumeAndClip(sample_t x);
		
		tint copyToBufferInt16LSB(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt16MSB(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt24LSB(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt24MSB(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt32LSB(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt32MSB(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferFloat32LSB(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferFloat32MSB(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferFloat64LSB(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferFloat64MSB(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt32LSB16(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt32MSB16(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt32LSB18(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt32MSB18(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt32LSB20(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt32MSB20(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt32LSB24(const sample_t *src,tint len,tint oOffset,tint chIndex);
		tint copyToBufferInt32MSB24(const sample_t *src,tint len,tint oOffset,tint chIndex);
};

//-------------------------------------------------------------------------------------------

#if defined(SINGLE_FLOAT_SAMPLE)

inline sample_t ASIOData::volumeAndClip(sample_t x)
{
	x *= m_volume;
	if(x < -1.0f)
	{
		x = -1.0f;
	}
	else if(x > 1.0f)
	{
		x = 1.0f;
	}
	return x;
}

#else

inline sample_t ASIOData::volumeAndClip(sample_t x)
{
	x *= m_volume;
	if(x < -1.0)
	{
		x = -1.0;
	}
	else if(x > 1.0)
	{
		x = 1.0;
	}
	return x;
}

#endif

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

