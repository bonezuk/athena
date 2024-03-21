//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_AUDIOIO_SAMPLECONVERTER_H
#define __OMEGA_AUDIOIO_SAMPLECONVERTER_H
//-------------------------------------------------------------------------------------------

#include "engine/inc/RData.h"
#include "audioio/inc/FormatDescription.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT SampleConverter
{		typedef enum
		{
			e_DataFloatSingle = 1,
			e_DataFloatDouble,
			e_DataSignedInteger,
			e_DataUnsignedInteger
		} DataType;

	public:
		SampleConverter();
		SampleConverter(tint noBits,tint bytesPerSample,bool littleEndian,bool alignHigh,bool isSigned);
		SampleConverter(bool isSinglePrecision,bool littleEndian);
		SampleConverter(const SampleConverter& rhs);
		
		const SampleConverter& operator = (const SampleConverter& rhs);
		
		bool isSupported() const;
		
		bool isLittleEndian() const;
		bool isAlignedHigh() const;
		
		FormatDescription::DataType type() const;
		
		tint bits() const;
		tint bytesPerSample() const;
		
		tint numberOfInputChannels() const;
		void setNumberOfInputChannels(tint no);
		tint numberOfOutputChannels() const;
		void setNumberOfOutputChannels(tint no);
		
		void setVolume(sample_t v);
		
		void convert(const sample_t *in,tbyte *out,tint noSamples, engine::CodecDataType type = engine::e_SampleFloat) const;
		void convert(const sample_t *in,tubyte *out,tint noSamples, engine::CodecDataType type = engine::e_SampleFloat) const;
		
	protected:
		
		FormatDescription::DataType m_type;
		tint m_noInChannels;
		tint m_noOutChannels;
		tint m_noBits;
		tint m_bytesPerSample;
		bool m_littleEndian;
		bool m_alignHigh;
		sample_t m_volume;
		
		void copy(const SampleConverter& rhs);
		
		tint calculateShift() const;
		
		tint sampleToInteger(sample_t v,const sample_t& dA,const sample_t& dB) const;

		tuint32 unsignedMask() const;
		tuint16 signedMaskInt16() const;
		tuint32 signedMaskInt24() const;
		tuint32 signedMaskInt32() const;

		void convertInteger(const sample_t *in,tbyte *out,tint noSamples,engine::CodecDataType type) const;
		void convertUnsignedInteger(const sample_t *in,tbyte *out,tint noSamples) const;

        void convertLittleEndian8BitLSB(const sample_t *in,tbyte *out,tint noSamples,engine::CodecDataType type) const;
		void convertLittleEndian8BitLSBFloat(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian8BitLSBInt16(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian8BitLSBInt24(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian8BitLSBInt32(const sample_t *in,tbyte *out,tint noSamples) const;
		
		void convertLittleEndian8BitMSB(const sample_t *in,tbyte *out,tint noSamples,engine::CodecDataType type) const;
		void convertLittleEndian8BitMSBFloat(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian8BitMSBInt16(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian8BitMSBInt24(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian8BitMSBInt32(const sample_t *in,tbyte *out,tint noSamples) const;
				
		void convertLittleEndianUnsigned8BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndianUnsigned8BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;

		void convertLittleEndian16BitLSB(const sample_t *in,tbyte *out,tint noSamples,engine::CodecDataType type) const;
		void convertLittleEndian16BitLSBFloat(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian16BitLSBInt16(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian16BitLSBInt24(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian16BitLSBInt32(const sample_t *in,tbyte *out,tint noSamples) const;
		
		void convertBigEndian16BitLSB(const sample_t *in,tbyte *out,tint noSamples,engine::CodecDataType type) const;
		void convertBigEndian16BitLSBFloat(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndian16BitLSBInt16(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndian16BitLSBInt24(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndian16BitLSBInt32(const sample_t *in,tbyte *out,tint noSamples) const;
		
		void convertLittleEndian16BitMSB(const sample_t *in,tbyte *out,tint noSamples,engine::CodecDataType type) const;
		void convertLittleEndian16BitMSBFloat(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian16BitMSBInt16(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian16BitMSBInt24(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian16BitMSBInt32(const sample_t *in,tbyte *out,tint noSamples) const;
		
		void convertBigEndian16BitMSB(const sample_t *in,tbyte *out,tint noSamples,engine::CodecDataType type) const;
		void convertBigEndian16BitMSBFloat(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndian16BitMSBInt16(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndian16BitMSBInt24(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndian16BitMSBInt32(const sample_t *in,tbyte *out,tint noSamples) const;
		
		void convertLittleEndianUnsigned16BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndianUnsigned16BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndianUnsigned16BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndianUnsigned16BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;

		void convertLittleEndian24BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndianUnsigned24BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian24BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndianUnsigned24BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndian24BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndianUnsigned24BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndian24BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndianUnsigned24BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;

		void convertLittleEndian32BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndianUnsigned32BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndian32BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndianUnsigned32BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndian32BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndianUnsigned32BitLSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndian32BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndianUnsigned32BitMSB(const sample_t *in,tbyte *out,tint noSamples) const;
		
		void convertLittleEndianSinglePrecision(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndianSinglePrecision(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertLittleEndianDoublePrecision(const sample_t *in,tbyte *out,tint noSamples) const;
		void convertBigEndianDoublePrecision(const sample_t *in,tbyte *out,tint noSamples) const;
};

//-------------------------------------------------------------------------------------------

inline tint SampleConverter::sampleToInteger(sample_t v,const sample_t& dA,const sample_t& dB) const
{
	tint x;

	v *= m_volume;
	
#if defined(SINGLE_FLOAT_SAMPLE)
	if(v <= -1.0f)
	{
		v = -1.0f;
	}
	else if(v >= 1.0f)
	{
		v = 1.0f;
	}
	
	if(v >= 0.0f)
	{
		x = static_cast<tint>(floor((v + (1.0f / (dA * 2.0f))) * dA));
	}
	else
	{
		x = static_cast<tint>(ceil(0.0f - (((-v) + (1.0f / (dB * 2.0f))) * dB)));
	}
#else
	if(v <= -1.0)
	{
		v = -1.0;
	}
	else if(v >= 1.0)
	{
		v = 1.0;
	}
	
	if(v >= 0.0)
	{
		x = static_cast<tint>(floor((v + (1.0 / (dA * 2.0))) * dA));
	}
	else
	{
		x = static_cast<tint>(ceil(0.0 - (((-v) + (1.0 / (dB * 2.0))) * dB)));
	}
#endif

	return x;
}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
