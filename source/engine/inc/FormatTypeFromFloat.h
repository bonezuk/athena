//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_VIOLETOMEGA_FORMATTYPEFROMFLOAT_H
#define __ORCUS_ENGINE_VIOLETOMEGA_FORMATTYPEFROMFLOAT_H
//-------------------------------------------------------------------------------------------

#include "engine/inc/FormatType.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
//-------------------------------------------------------------------------------------------

ENGINE_EXPORT tint sampleToInteger(tfloat32 v,tint noBits);
ENGINE_EXPORT tint sampleToInteger(tfloat64 v,tint noBits);

ENGINE_EXPORT tfloat32 sampleToFloat32(tfloat64 v);

ENGINE_EXPORT tint sampleTo32BitsInteger(tfloat32 v);
ENGINE_EXPORT tint sampleTo32BitsInteger(tfloat64 v);

ENGINE_EXPORT tint sampleTo24BitsInteger(tfloat32 v);
ENGINE_EXPORT tint sampleTo24BitsInteger(tfloat64 v);

ENGINE_EXPORT tint sampleTo16BitsInteger(tfloat32 v);
ENGINE_EXPORT tint sampleTo16BitsInteger(tfloat64 v);

ENGINE_EXPORT tint sampleTo8BitsInteger(tfloat32 v);
ENGINE_EXPORT tint sampleTo8BitsInteger(tfloat64 v);

ENGINE_EXPORT void writeFloat32ToSample(const tfloat32 *src,tfloat64 *dst,tint noElements);

ENGINE_EXPORT void write8BitsFromSample(tfloat32 v,tchar *mem);
ENGINE_EXPORT void write8BitsFromSample(tfloat64 v,tchar *mem);

ENGINE_EXPORT void write16BitsLittleEndianFromSample(tfloat32 v,tchar *mem);
ENGINE_EXPORT void write16BitsLittleEndianFromSample(tfloat64 v,tchar *mem);

ENGINE_EXPORT void write16BitsBigEndianFromSample(tfloat32 v,tchar *mem);
ENGINE_EXPORT void write16BitsBigEndianFromSample(tfloat64 v,tchar *mem);

ENGINE_EXPORT void write24BitsLittleEndianFromSample(tfloat32 v,tchar *mem);
ENGINE_EXPORT void write24BitsLittleEndianFromSample(tfloat64 v,tchar *mem);

ENGINE_EXPORT void write24BitsBigEndianFromSample(tfloat32 v,tchar *mem);
ENGINE_EXPORT void write24BitsBigEndianFromSample(tfloat64 v,tchar *mem);

ENGINE_EXPORT void write32BitsLittleEndianFromSample(tfloat32 v,tchar *mem);
ENGINE_EXPORT void write32BitsLittleEndianFromSample(tfloat64 v,tchar *mem);

ENGINE_EXPORT void write32BitsBigEndianFromSample(tfloat32 v,tchar *mem);
ENGINE_EXPORT void write32BitsBigEndianFromSample(tfloat64 v,tchar *mem);

ENGINE_EXPORT void writeLittleEndian16BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeLittleEndian16BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian16BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian16BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeNative16BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeNative16BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);

ENGINE_EXPORT void writeLittleEndian24BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeLittleEndian24BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian24BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian24BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeNative24BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeNative24BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);

ENGINE_EXPORT void writeLittleEndian32BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeLittleEndian32BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian32BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian32BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeNative32BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeNative32BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);

ENGINE_EXPORT void writeLittleEndian64BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeLittleEndian64BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian64BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian64BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeNative64BitsAsLittleEndian(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeNative64BitsAsBigEndian(const tbyte *src,tbyte *dst,tint noElements);

ENGINE_EXPORT void writeLittleEndian16BitsAsNative(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian16BitsAsNative(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeLittleEndian24BitsAsNative(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian24BitsAsNative(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeLittleEndian32BitsAsNative(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian32BitsAsNative(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeLittleEndian64BitsAsNative(const tbyte *src,tbyte *dst,tint noElements);
ENGINE_EXPORT void writeBigEndian64BitsAsNative(const tbyte *src,tbyte *dst,tint noElements);

//-------------------------------------------------------------------------------------------

class ENGINE_EXPORT SampleFromInteger
{
	public:
		SampleFromInteger(tint noBits);
		
		sample_t convert(tint32 v);
		
	protected:
	
		sample_t m_divP;
		sample_t m_divN;
};

//-------------------------------------------------------------------------------------------

inline tfloat64 sample64From8Bit(tbyte v)
{
        tint r;
        tfloat64 x;

        if(v & 0x80)
        {
                r = static_cast<tint>(0xffffff00 | static_cast<tuint>(v));
        }
        else
        {
                r = static_cast<tint>(v);
        }

        if(r >= 0)
        {
                x = static_cast<tfloat64>(r) / 127.0;
        }
        else
        {
                x = static_cast<tfloat64>(r) / 128.0;
        }
        return x;
}

//-------------------------------------------------------------------------------------------

inline tfloat64 sample64From8Bit(tubyte v)
{
	return sample64From8Bit(static_cast<tbyte>(v));
}

//-------------------------------------------------------------------------------------------

inline tfloat64 sample64From8Bit(tint32 v)
{
	tfloat64 x;
	
	if(v >= 0)
	{
		x = static_cast<tfloat64>(v) / 127.0;
	}
	else
	{
		x = static_cast<tfloat64>(v) / 128.0;
	}
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

//-------------------------------------------------------------------------------------------

inline tfloat64 sample64From16Bit(tint16 v)
{
	tfloat64 x;
	
	if(v >= 0)
	{
		x = static_cast<tfloat64>(v) / 32767.0;
	}
	else
	{
		x = static_cast<tfloat64>(v) / 32768.0;
	}
	return x;
}

//-------------------------------------------------------------------------------------------

inline tfloat64 sample64From16Bit(tuint16 v)
{
	return sample64From16Bit(static_cast<tint16>(v));
}

//-------------------------------------------------------------------------------------------

inline tfloat64 sample64From16Bit(tint32 v)
{
	tfloat64 x;
	
	if(v >= 0)
	{
		x = static_cast<tfloat64>(v) / 32767.0;
	}
	else
	{
		x = static_cast<tfloat64>(v) / 32768.0;
	}
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

//-------------------------------------------------------------------------------------------

inline tfloat64 sample64From24Bit(tint32 v)
{
	tfloat64 x;
	
	if(v >= 0)
	{
		x = static_cast<tfloat64>(v) / 8388607.0;
	}
	else
	{
		x = static_cast<tfloat64>(v) / 8388608.0;
	}
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

//-------------------------------------------------------------------------------------------

inline tfloat64 sample64From24Bit(tuint32 v)
{
	return sample64From24Bit(static_cast<tint32>(v));
}

//-------------------------------------------------------------------------------------------

inline tfloat64 sample64From32Bit(tint32 v)
{
	tfloat64 x;
	
	if(v >= 0)
	{
		x = static_cast<tfloat64>(v) / 2147483647.0;
	}
	else
	{
		x = static_cast<tfloat64>(v) / 2147483648.0;
	}
	return x;
}

//-------------------------------------------------------------------------------------------

inline tfloat64 sample64From32Bit(tuint32 v)
{
	return sample64From32Bit(static_cast<tint32>(v));
}

//-------------------------------------------------------------------------------------------

inline tfloat32 sample32From8Bit(tbyte v)
{
        tint r;
        tfloat32 x;

        if(v & 0x80)
        {
                r = static_cast<tint>(0xffffff00 | static_cast<tuint>(v));
        }
        else
        {
                r = static_cast<tint>(v);
        }

        if(r >= 0)
        {
                x = static_cast<tfloat32>(r) / 127.0f;
        }
        else
        {
                x = static_cast<tfloat32>(r) / 128.0f;
        }
        return x;
}

//-------------------------------------------------------------------------------------------

inline tfloat32 sample32From8Bit(tubyte v)
{
	return sample32From8Bit(static_cast<tbyte>(v));
}

//-------------------------------------------------------------------------------------------

inline tfloat32 sample32From8Bit(tint32 v)
{
	tfloat32 x;
	
	if(v >= 0)
	{
		x = static_cast<tfloat32>(v) / 127.0f;
	}
	else
	{
		x = static_cast<tfloat32>(v) / 128.0f;
	}
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

//-------------------------------------------------------------------------------------------

inline tfloat32 sample32From16Bit(tint16 v)
{
	tfloat32 x;
	
	if(v >= 0)
	{
		x = static_cast<tfloat32>(v) / 32767.0f;
	}
	else
	{
		x = static_cast<tfloat32>(v) / 32768.0f;
	}
	return x;
}

//-------------------------------------------------------------------------------------------

inline tfloat32 sample32From16Bit(tuint16 v)
{
	return sample32From16Bit(static_cast<tint16>(v));
}

//-------------------------------------------------------------------------------------------

inline tfloat32 sample32From16Bit(tint32 v)
{
	tfloat32 x;
	
	if(v >= 0)
	{
		x = static_cast<tfloat32>(v) / 32767.0f;
	}
	else
	{
		x = static_cast<tfloat32>(v) / 32768.0f;
	}
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

//-------------------------------------------------------------------------------------------

inline tfloat32 sample32From24Bit(tint32 v)
{
	tfloat32 x;
	
	if(v >= 0)
	{
		x = static_cast<tfloat32>(v) / 8388607.0f;
	}
	else
	{
		x = static_cast<tfloat32>(v) / 8388608.0f;
	}
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

//-------------------------------------------------------------------------------------------

inline tfloat32 sample32From24Bit(tuint32 v)
{
	return sample32From24Bit(static_cast<tint32>(v));
}

//-------------------------------------------------------------------------------------------

inline tfloat32 sample32From32Bit(tint32 v)
{
	tfloat32 x;
	
	if(v >= 0)
	{
		x = static_cast<tfloat32>(v) / 2147483647.0f;
	}
	else
	{
		x = static_cast<tfloat32>(v) / 2147483648.0f;
	}
	return x;
}

//-------------------------------------------------------------------------------------------

inline tfloat32 sample32From32Bit(tuint32 v)
{
	return sample32From32Bit(static_cast<tint32>(v));
}

//-------------------------------------------------------------------------------------------

inline sample_t sampleFrom8Bit(tbyte v)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	return sample32From8Bit(v);
#else
	return sample64From8Bit(v);
#endif	
}

//-------------------------------------------------------------------------------------------

inline sample_t sampleFrom8Bit(tubyte v)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	return sample32From8Bit(v);
#else
	return sample64From8Bit(v);
#endif	
}

//-------------------------------------------------------------------------------------------

inline sample_t sampleFrom8Bit(tint32 v)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	return sample32From8Bit(v);
#else
	return sample64From8Bit(v);
#endif	
}

//-------------------------------------------------------------------------------------------

inline sample_t sampleFrom16Bit(tint16 v)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	return sample32From16Bit(v);
#else
	return sample64From16Bit(v);
#endif	
}

//-------------------------------------------------------------------------------------------

inline sample_t sampleFrom16Bit(tuint16 v)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	return sample32From16Bit(v);
#else
	return sample64From16Bit(v);
#endif	
}

//-------------------------------------------------------------------------------------------

inline sample_t sampleFrom16Bit(tint32 v)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	return sample32From16Bit(v);
#else
	return sample64From16Bit(v);
#endif	
}

//-------------------------------------------------------------------------------------------

inline sample_t sampleFrom24Bit(tint32 v)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	return sample32From24Bit(v);
#else
	return sample64From24Bit(v);
#endif	
}

//-------------------------------------------------------------------------------------------

inline sample_t sampleFrom24Bit(tuint32 v)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	return sample32From24Bit(v);
#else
	return sample64From24Bit(v);
#endif	
}

//-------------------------------------------------------------------------------------------

inline sample_t sampleFrom32Bit(tint32 v)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	return sample32From32Bit(v);
#else
	return sample64From32Bit(v);
#endif	
}

//-------------------------------------------------------------------------------------------

inline sample_t sampleFrom32Bit(tuint32 v)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	return sample32From32Bit(v);
#else
	return sample64From32Bit(v);
#endif	
}

//-------------------------------------------------------------------------------------------
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

