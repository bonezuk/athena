#include "audioio/inc/SampleConverter.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

SampleConverter::SampleConverter() : m_type(FormatDescription::e_DataSignedInteger),
	m_noInChannels(1),
	m_noOutChannels(1),
	m_noBits(16),
	m_bytesPerSample(2),
	m_littleEndian(true),
	m_alignHigh(false),
	m_volume(c_plusOneSample)
{}

//-------------------------------------------------------------------------------------------

SampleConverter::SampleConverter(tint noBits,tint bytesPerSample,bool littleEndian,bool alignHigh,bool isSigned) : m_type(FormatDescription::e_DataSignedInteger),
	m_noInChannels(1),
	m_noOutChannels(1),
	m_noBits(noBits),
	m_bytesPerSample(bytesPerSample),
	m_littleEndian(littleEndian),
	m_alignHigh(alignHigh),
	m_volume(c_plusOneSample)
{
	m_type = (isSigned) ? FormatDescription::e_DataSignedInteger : FormatDescription::e_DataUnsignedInteger;
}

//-------------------------------------------------------------------------------------------

SampleConverter::SampleConverter(bool isSinglePrecision,bool littleEndian) : m_type(FormatDescription::e_DataSignedInteger),
	m_noInChannels(1),
	m_noOutChannels(1),
	m_noBits(16),
	m_bytesPerSample(2),
	m_littleEndian(true),
	m_alignHigh(false),
	m_volume(c_plusOneSample)
{
	if(isSinglePrecision)
	{
		m_type = FormatDescription::e_DataFloatSingle;
		m_noBits = 32;
		m_bytesPerSample = 4;
	}
	else
	{
		m_type = FormatDescription::e_DataFloatDouble;
		m_noBits = 64;
		m_bytesPerSample = 8;
	}
	m_littleEndian = littleEndian;
}

//-------------------------------------------------------------------------------------------

SampleConverter::SampleConverter(const SampleConverter& rhs) : m_type(FormatDescription::e_DataSignedInteger),
	m_noInChannels(1),
	m_noOutChannels(1),
	m_noBits(16),
	m_bytesPerSample(2),
	m_littleEndian(true),
	m_alignHigh(false),
	m_volume(c_plusOneSample)
{
	copy(rhs);
}

//-------------------------------------------------------------------------------------------

const SampleConverter& SampleConverter::operator = (const SampleConverter& rhs)
{
	if(this!=&rhs)
	{
		copy(rhs);
	}
	return *this;
}

//-------------------------------------------------------------------------------------------

void SampleConverter::copy(const SampleConverter& rhs)
{
	m_type = rhs.m_type;
	m_noInChannels = rhs.m_noInChannels;
	m_noOutChannels = rhs.m_noOutChannels;
	m_noBits = rhs.m_noBits;
	m_bytesPerSample = rhs.m_bytesPerSample;
	m_littleEndian = rhs.m_littleEndian;
	m_alignHigh = rhs.m_alignHigh;
	m_volume = rhs.m_volume;
}

//-------------------------------------------------------------------------------------------

bool SampleConverter::isSupported() const
{
	bool res = false;
	
	switch(m_bytesPerSample)
	{
		case 1:
			if(m_noBits>=1 && m_noBits<=8)
			{
				res = true;
			}
			break;
			
		case 2:
			if(m_noBits>=1 && m_noBits<=16)
			{
				res = true;
			}
			break;
		
		case 3:
			if(m_noBits>=1 && m_noBits<=24)
			{
				res = true;
			}
			break;

		case 4:
			if(m_noBits>=1 && m_noBits<=32)
			{
				res = true;
			}
			break;
			
		case 8:
			if(m_noBits==64)
			{
				res = true;
			}
			break;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

FormatDescription::DataType SampleConverter::type() const
{
	return m_type;
}

//-------------------------------------------------------------------------------------------

bool SampleConverter::isLittleEndian() const
{
	return m_littleEndian;
}

//-------------------------------------------------------------------------------------------

bool SampleConverter::isAlignedHigh() const
{
	return m_alignHigh;
}

//-------------------------------------------------------------------------------------------

tint SampleConverter::numberOfInputChannels() const
{
	return m_noInChannels;
}

//-------------------------------------------------------------------------------------------

void SampleConverter::setNumberOfInputChannels(tint no)
{
	m_noInChannels = no;
}

//-------------------------------------------------------------------------------------------

tint SampleConverter::numberOfOutputChannels() const
{
	return m_noOutChannels;
}

//-------------------------------------------------------------------------------------------

void SampleConverter::setNumberOfOutputChannels(tint no)
{
	m_noOutChannels = no;
}

//-------------------------------------------------------------------------------------------

tint SampleConverter::bits() const
{
	return m_noBits;
}

//-------------------------------------------------------------------------------------------

tint SampleConverter::bytesPerSample() const
{
	return m_bytesPerSample;
}

//-------------------------------------------------------------------------------------------

void SampleConverter::setVolume(sample_t v)
{
	m_volume = v;
}

//-------------------------------------------------------------------------------------------

tint SampleConverter::calculateShift() const
{
	return (m_bytesPerSample << 3) - m_noBits;
}

//-------------------------------------------------------------------------------------------

tuint32 SampleConverter::unsignedMask() const
{
	static const tuint32 c_mask[] = {
		0x00000000, // 0 
		0x00000001, // 1
		0x00000003, // 2
		0x00000007, // 3
		0x0000000f, // 4
		0x0000001f, // 5
		0x0000003f, // 6
		0x0000007f, // 7
		0x000000ff, // 8
		0x000001ff, // 9
		0x000003ff, // 10
		0x000007ff, // 11
		0x00000fff, // 12
		0x00001fff, // 13
		0x00003fff, // 14
		0x00007fff, // 15
		0x0000ffff, // 16
		0x0001ffff, // 17
		0x0003ffff, // 18
		0x0007ffff, // 19
		0x000fffff, // 20
		0x001fffff, // 21
		0x003fffff, // 22
		0x007fffff, // 23
		0x00ffffff, // 24
		0x01ffffff, // 25
		0x03ffffff, // 26
		0x07ffffff, // 27
		0x0fffffff, // 28
		0x1fffffff, // 29
		0x3fffffff, // 30
		0x7fffffff, // 31
		0xffffffff, // 32
	};
	
	return c_mask[m_noBits];
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndian8BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tbyte) * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		*out = static_cast<tbyte>(x & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndianUnsigned8BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tbyte) * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);
	
	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		*o = static_cast<tubyte>(v & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndian8BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tbyte) * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB) << shift;
		*out = static_cast<tbyte>(x & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndianUnsigned8BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tbyte) * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		v <<= shift;
		*o = static_cast<tbyte>(v & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndian16BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint16) * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		out[0] = static_cast<tbyte>(x & 0x000000ff);
		out[1] = static_cast<tbyte>((x >> 8) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndianUnsigned16BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint16) * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		o[0] = static_cast<tubyte>(v & 0x000000ff);
		o[1] = static_cast<tubyte>((v >> 8) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndian16BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint16) * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB) << shift;
		out[0] = static_cast<tbyte>(x & 0x000000ff);
		out[1] = static_cast<tbyte>((x >> 8) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndianUnsigned16BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint16) * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		v <<= shift;
		o[0] = static_cast<tbyte>(v & 0x000000ff);
		o[1] = static_cast<tbyte>((v >> 8) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndian16BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint16) * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		out[1] = static_cast<tbyte>(x & 0x000000ff);
		out[0] = static_cast<tbyte>((x >> 8) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndianUnsigned16BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint16) * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		o[1] = static_cast<tbyte>(v & 0x000000ff);
		o[0] = static_cast<tbyte>((v >> 8) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndian16BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint16) * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB) << shift;
		out[1] = static_cast<tbyte>(x & 0x000000ff);
		out[0] = static_cast<tbyte>((x >> 8) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndianUnsigned16BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint16) * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		v <<= shift;
		o[1] = static_cast<tbyte>(v & 0x000000ff);
		o[0] = static_cast<tbyte>((v >> 8) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndian24BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = 3 * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		out[0] = static_cast<tbyte>(x & 0x000000ff);
		out[1] = static_cast<tbyte>((x >> 8) & 0x000000ff);
		out[2] = static_cast<tbyte>((x >> 16) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndianUnsigned24BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = 3 * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		o[0] = static_cast<tbyte>(v & 0x000000ff);
		o[1] = static_cast<tbyte>((v >> 8) & 0x000000ff);
		o[2] = static_cast<tbyte>((v >> 16) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndian24BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = 3 * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB) << shift;
		out[0] = static_cast<tbyte>(x & 0x000000ff);
		out[1] = static_cast<tbyte>((x >> 8) & 0x000000ff);
		out[2] = static_cast<tbyte>((x >> 16) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndianUnsigned24BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = 3 * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
        v <<= shift;
		o[0] = static_cast<tbyte>(v & 0x000000ff);
		o[1] = static_cast<tbyte>((v >> 8) & 0x000000ff);
		o[2] = static_cast<tbyte>((v >> 16) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndian24BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = 3 * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		out[2] = static_cast<tbyte>(x & 0x000000ff);
		out[1] = static_cast<tbyte>((x >> 8) & 0x000000ff);
		out[0] = static_cast<tbyte>((x >> 16) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndianUnsigned24BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = 3 * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		o[2] = static_cast<tbyte>(v & 0x000000ff);
		o[1] = static_cast<tbyte>((v >> 8) & 0x000000ff);
		o[0] = static_cast<tbyte>((v >> 16) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndian24BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = 3 * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB) << shift;
		out[2] = static_cast<tbyte>(x & 0x000000ff);
		out[1] = static_cast<tbyte>((x >> 8) & 0x000000ff);
		out[0] = static_cast<tbyte>((x >> 16) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndianUnsigned24BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = 3 * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		v <<= shift;
		o[2] = static_cast<tbyte>(v & 0x000000ff);
		o[1] = static_cast<tbyte>((v >> 8) & 0x000000ff);
		o[0] = static_cast<tbyte>((v >> 16) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndian32BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint32) * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		out[0] = static_cast<tbyte>(x & 0x000000ff);
		out[1] = static_cast<tbyte>((x >> 8) & 0x000000ff);
		out[2] = static_cast<tbyte>((x >> 16) & 0x000000ff);
		out[3] = static_cast<tbyte>((x >> 24) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndianUnsigned32BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint32) * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		o[0] = static_cast<tbyte>(v & 0x000000ff);
		o[1] = static_cast<tbyte>((v >> 8) & 0x000000ff);
		o[2] = static_cast<tbyte>((v >> 16) & 0x000000ff);
		o[3] = static_cast<tbyte>((v >> 24) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndian32BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint32) * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB) << shift;
		out[0] = static_cast<tbyte>(x & 0x000000ff);
		out[1] = static_cast<tbyte>((x >> 8) & 0x000000ff);
		out[2] = static_cast<tbyte>((x >> 16) & 0x000000ff);
		out[3] = static_cast<tbyte>((x >> 24) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndianUnsigned32BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint32) * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		v <<= shift;
		o[0] = static_cast<tbyte>(v & 0x000000ff);
		o[1] = static_cast<tbyte>((v >> 8) & 0x000000ff);
		o[2] = static_cast<tbyte>((v >> 16) & 0x000000ff);
		o[3] = static_cast<tbyte>((v >> 24) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndian32BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint32) * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		out[3] = static_cast<tbyte>(x & 0x000000ff);
		out[2] = static_cast<tbyte>((x >> 8) & 0x000000ff);
		out[1] = static_cast<tbyte>((x >> 16) & 0x000000ff);
		out[0] = static_cast<tbyte>((x >> 24) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndianUnsigned32BitLSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint32) * m_noOutChannels;
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		o[3] = static_cast<tbyte>(v & 0x000000ff);
		o[2] = static_cast<tbyte>((v >> 8) & 0x000000ff);
		o[1] = static_cast<tbyte>((v >> 16) & 0x000000ff);
		o[0] = static_cast<tbyte>((v >> 24) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndian32BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint32) * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB) << shift;
		out[3] = static_cast<tbyte>(x & 0x000000ff);
		out[2] = static_cast<tbyte>((x >> 8) & 0x000000ff);
		out[1] = static_cast<tbyte>((x >> 16) & 0x000000ff);
		out[0] = static_cast<tbyte>((x >> 24) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndianUnsigned32BitMSB(const sample_t *in,tbyte *out,tint noSamples) const
{
	tint inc = sizeof(tint32) * m_noOutChannels;
	tint shift = calculateShift();
	tuint32 s = static_cast<tuint32>(1 << (m_noBits - 1));
	sample_t dA = static_cast<sample_t>(s - 1);
	sample_t dB = static_cast<sample_t>(s);

	tuint32 min = static_cast<tuint32>(sampleToInteger(-1,dA,dB));
	tubyte *o = reinterpret_cast<tubyte *>(out);

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,o+=inc)
	{
		tint x = sampleToInteger(*in,dA,dB);
		tuint32 v = static_cast<tuint32>(x);
		if(x < 0)
		{
			v -= min;
		}
		else
		{
			v += min;
			v &= unsignedMask();
		}
		v <<= shift;
		o[3] = static_cast<tbyte>(v & 0x000000ff);
		o[2] = static_cast<tbyte>((v >> 8) & 0x000000ff);
		o[1] = static_cast<tbyte>((v >> 16) & 0x000000ff);
		o[0] = static_cast<tbyte>((v >> 24) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndianSinglePrecision(const sample_t *in,tbyte *out,tint noSamples) const
{
	union P 
	{
		tfloat32 x;
		tuint32 v;
	};

	tint inc = sizeof(tfloat32) * m_noOutChannels;

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		P x;
		x.x = static_cast<tfloat32>(*in);
		out[0] = static_cast<tbyte>(x.v & 0x000000ff);
		out[1] = static_cast<tbyte>((x.v >> 8) & 0x000000ff);
		out[2] = static_cast<tbyte>((x.v >> 16) & 0x000000ff);
		out[3] = static_cast<tbyte>((x.v >> 24) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndianSinglePrecision(const sample_t *in,tbyte *out,tint noSamples) const
{
	union P 
	{
		tfloat32 x;
		tuint32 v;
	};

	tint inc = sizeof(tfloat32) * m_noOutChannels;

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		P x;
		x.x = static_cast<tfloat32>(*in);
		out[3] = static_cast<tbyte>(x.v & 0x000000ff);
		out[2] = static_cast<tbyte>((x.v >> 8) & 0x000000ff);
		out[1] = static_cast<tbyte>((x.v >> 16) & 0x000000ff);
		out[0] = static_cast<tbyte>((x.v >> 24) & 0x000000ff);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertLittleEndianDoublePrecision(const sample_t *in,tbyte *out,tint noSamples) const
{
	union P 
	{
		tfloat64 x;
		tuint64 v;
	};

	tint inc = sizeof(tfloat64) * m_noOutChannels;

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		P x;
        x.x = static_cast<tfloat64>(*in);
		out[0] = static_cast<tbyte>(x.v & 0x00000000000000ffULL);
		out[1] = static_cast<tbyte>((x.v >> 8) & 0x00000000000000ffULL);
		out[2] = static_cast<tbyte>((x.v >> 16) & 0x00000000000000ffULL);
		out[3] = static_cast<tbyte>((x.v >> 24) & 0x00000000000000ffULL);
		out[4] = static_cast<tbyte>((x.v >> 32) & 0x00000000000000ffULL);
		out[5] = static_cast<tbyte>((x.v >> 40) & 0x00000000000000ffULL);
		out[6] = static_cast<tbyte>((x.v >> 48) & 0x00000000000000ffULL);
		out[7] = static_cast<tbyte>((x.v >> 56) & 0x00000000000000ffULL);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertBigEndianDoublePrecision(const sample_t *in,tbyte *out,tint noSamples) const
{
	union P 
	{
		tfloat64 x;
		tuint64 v;
	};

	tint inc = sizeof(tfloat64) * m_noOutChannels;

	for(tint i=0;i<noSamples;i++,in+=m_noInChannels,out+=inc)
	{
		P x;
        x.x = static_cast<tfloat64>(*in);
		out[7] = static_cast<tbyte>(x.v & 0x00000000000000ffULL);
		out[6] = static_cast<tbyte>((x.v >> 8) & 0x00000000000000ffULL);
		out[5] = static_cast<tbyte>((x.v >> 16) & 0x00000000000000ffULL);
		out[4] = static_cast<tbyte>((x.v >> 24) & 0x00000000000000ffULL);
		out[3] = static_cast<tbyte>((x.v >> 32) & 0x00000000000000ffULL);
		out[2] = static_cast<tbyte>((x.v >> 40) & 0x00000000000000ffULL);
		out[1] = static_cast<tbyte>((x.v >> 48) & 0x00000000000000ffULL);
		out[0] = static_cast<tbyte>((x.v >> 56) & 0x00000000000000ffULL);
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertInteger(const sample_t *in,tbyte *out,tint noSamples) const
{
	switch(m_bytesPerSample)
	{
		case 1:
			if(!m_alignHigh)
			{
				convertLittleEndian8BitLSB(in,out,noSamples);
			}
			else
			{
				convertLittleEndian8BitMSB(in,out,noSamples);
			}
			break;

		case 2:
			if(m_littleEndian)
			{
				if(!m_alignHigh)
				{
					convertLittleEndian16BitLSB(in,out,noSamples);
				}
				else
				{
					convertLittleEndian16BitMSB(in,out,noSamples);
				}
			}
			else
			{
				if(!m_alignHigh)
				{
					convertBigEndian16BitLSB(in,out,noSamples);
				}
				else
				{
					convertBigEndian16BitMSB(in,out,noSamples);
				}
			}
			break;

		case 3:
			if(m_littleEndian)
			{
				if(!m_alignHigh)
				{
					convertLittleEndian24BitLSB(in,out,noSamples);
				}
				else
				{
					convertLittleEndian24BitMSB(in,out,noSamples);
				}
			}
			else
			{
				if(!m_alignHigh)
				{
					convertBigEndian24BitLSB(in,out,noSamples);
				}
				else
				{
					convertBigEndian24BitMSB(in,out,noSamples);
				}
			}
			break;

		case 4:
		default:
			if(m_littleEndian)
			{
				if(!m_alignHigh)
				{
					convertLittleEndian32BitLSB(in,out,noSamples);
				}
				else
				{
					convertLittleEndian32BitMSB(in,out,noSamples);
				}
			}
			else
			{
				if(!m_alignHigh)
				{
					convertBigEndian32BitLSB(in,out,noSamples);
				}
				else
				{
					convertBigEndian32BitMSB(in,out,noSamples);
				}
			}
			break;
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convertUnsignedInteger(const sample_t *in,tbyte *out,tint noSamples) const
{
	switch(m_bytesPerSample)
	{
		case 1:
			if(!m_alignHigh)
			{
				convertLittleEndianUnsigned8BitLSB(in,out,noSamples);
			}
			else
			{
				convertLittleEndianUnsigned8BitMSB(in,out,noSamples);
			}
			break;

		case 2:
			if(m_littleEndian)
			{
				if(!m_alignHigh)
				{
					convertLittleEndianUnsigned16BitLSB(in,out,noSamples);
				}
				else
				{
					convertLittleEndianUnsigned16BitMSB(in,out,noSamples);
				}
			}
			else
			{
				if(!m_alignHigh)
				{
					convertBigEndianUnsigned16BitLSB(in,out,noSamples);
				}
				else
				{
					convertBigEndianUnsigned16BitMSB(in,out,noSamples);
				}
			}
			break;

		case 3:
			if(m_littleEndian)
			{
				if(!m_alignHigh)
				{
					convertLittleEndianUnsigned24BitLSB(in,out,noSamples);
				}
				else
				{
					convertLittleEndianUnsigned24BitMSB(in,out,noSamples);
				}
			}
			else
			{
				if(!m_alignHigh)
				{
					convertBigEndianUnsigned24BitLSB(in,out,noSamples);
				}
				else
				{
					convertBigEndianUnsigned24BitMSB(in,out,noSamples);
				}
			}
			break;

		case 4:
		default:
			if(m_littleEndian)
			{
				if(!m_alignHigh)
				{
					convertLittleEndianUnsigned32BitLSB(in,out,noSamples);
				}
				else
				{
					convertLittleEndianUnsigned32BitMSB(in,out,noSamples);
				}
			}
			else
			{
				if(!m_alignHigh)
				{
					convertBigEndianUnsigned32BitLSB(in,out,noSamples);
				}
				else
				{
					convertBigEndianUnsigned32BitMSB(in,out,noSamples);
				}
			}
			break;
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convert(const sample_t *in,tbyte *out,tint noSamples) const
{
	switch(m_type)
	{
		case FormatDescription::e_DataFloatSingle:
			if(m_littleEndian)
			{
				convertLittleEndianSinglePrecision(in,out,noSamples);
			}
			else
			{
				convertBigEndianSinglePrecision(in,out,noSamples);
			}
			break;
			
		case FormatDescription::e_DataFloatDouble:
			if(m_littleEndian)
			{
				convertLittleEndianDoublePrecision(in,out,noSamples);
			}
			else
			{
				convertBigEndianDoublePrecision(in,out,noSamples);
			}
			break;
			
		case FormatDescription::e_DataUnsignedInteger:
			convertUnsignedInteger(in,out,noSamples);
			break;
			
		case FormatDescription::e_DataSignedInteger:
		default:
			convertInteger(in,out,noSamples);
			break;
	}
}

//-------------------------------------------------------------------------------------------

void SampleConverter::convert(const sample_t *in,tubyte *out,tint noSamples) const
{
	convert(in,reinterpret_cast<tbyte *>(out),noSamples);
}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
