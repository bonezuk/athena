#include "gtest/gtest.h"

#include "audioio/inc/SampleConverter.h"

using namespace omega;
using namespace audioio;
using namespace testing;

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,isNotSupported)
{
    SampleConverter sampleConverterA(9,1,true,false,true);
	EXPECT_FALSE(sampleConverterA.isSupported());

    SampleConverter sampleConverterB(32,5,true,false,true);
    EXPECT_FALSE(sampleConverterB.isSupported());

    SampleConverter sampleConverterC(64,4,true,false,true);
    EXPECT_FALSE(sampleConverterC.isSupported());
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,constructorDefault)
{
	SampleConverter sampleConverter;
	EXPECT_TRUE(sampleConverter.isLittleEndian());
	EXPECT_FALSE(sampleConverter.isAlignedHigh());
	EXPECT_EQ(16,sampleConverter.bits());
	EXPECT_EQ(2,sampleConverter.bytesPerSample());	
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,constructorCopy)
{
    SampleConverter sampleConverterA(9,1,true,false,true);
	SampleConverter sampleConverterB(sampleConverterA);
	EXPECT_TRUE(sampleConverterB.isLittleEndian());
	EXPECT_FALSE(sampleConverterB.isAlignedHigh());
	EXPECT_EQ(9,sampleConverterB.bits());
	EXPECT_EQ(1,sampleConverterB.bytesPerSample());
    EXPECT_EQ(1,sampleConverterB.numberOfInputChannels());
    EXPECT_EQ(1,sampleConverterB.numberOfOutputChannels());
	
    SampleConverter sampleConverterC(24,3,false,true,true);
	sampleConverterC.setNumberOfInputChannels(3);
	sampleConverterC.setNumberOfOutputChannels(8);
	
    SampleConverter sampleConverterD(sampleConverterC);
    EXPECT_FALSE(sampleConverterD.isLittleEndian());
    EXPECT_TRUE(sampleConverterD.isAlignedHigh());
    EXPECT_EQ(24,sampleConverterD.bits());
    EXPECT_EQ(3,sampleConverterD.bytesPerSample());
    EXPECT_EQ(3,sampleConverterD.numberOfInputChannels());
    EXPECT_EQ(8,sampleConverterD.numberOfOutputChannels());
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,equalityOperator)
{
    SampleConverter sampleConverterA(9,1,true,false,true);
	SampleConverter sampleConverterB;
	sampleConverterB = sampleConverterA;
	EXPECT_TRUE(sampleConverterB.isLittleEndian());
	EXPECT_FALSE(sampleConverterB.isAlignedHigh());
	EXPECT_EQ(9,sampleConverterB.bits());
	EXPECT_EQ(1,sampleConverterB.bytesPerSample());
    EXPECT_EQ(1,sampleConverterB.numberOfInputChannels());
    EXPECT_EQ(1,sampleConverterB.numberOfOutputChannels());
	
    SampleConverter sampleConverterC(24,3,false,true,true);
	sampleConverterC.setNumberOfInputChannels(3);
	sampleConverterC.setNumberOfOutputChannels(8);
		
	SampleConverter sampleConverterD;
	sampleConverterD = sampleConverterC;
	
    EXPECT_FALSE(sampleConverterD.isLittleEndian());
    EXPECT_TRUE(sampleConverterD.isAlignedHigh());
    EXPECT_EQ(24,sampleConverterD.bits());
    EXPECT_EQ(3,sampleConverterD.numberOfInputChannels());
    EXPECT_EQ(8,sampleConverterD.numberOfOutputChannels());
}

//-------------------------------------------------------------------------------------------

tint isSampleEquiv(tuint32 a,tuint32 b,tint N)
{
	tint r = 0;

	if(a==b || a==b-1 || a==b+1|| a==b-2 || a==b+2)
	{
		r = 1;
	}
	else if(N==3 && (((a==0x00800000 || a==0) && b==0x00ffffff) || ((b==0x00800000 || b==0) && a==0x00ffffff)))
	{
		r = 1;
	}
	return r;
}

void testSampleOutputConvertion(const tubyte *expect,const tubyte *out,tint noBits,tint bytesPerSample,tint noSamples)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	if(bytesPerSample>=3)
	{
		const tint N = 3;
		const tubyte *x,*y;
		
		x = expect;
		y = out;
		for(int i=0;i<noSamples;i++)
		{
			tuint32 a,b,r = 0;
			
			a = 0;
			b = 0;
			for(int j=0;j<N;j++)
			{
				a |= static_cast<tuint32>(x[j]) << (j * 8);
				b |= static_cast<tuint32>(y[j]) << (j * 8);
			}
			r |= isSampleEquiv(a,b,N);

			a = 0;
			b = 0;
			for(int j=0;j<N;j++)
			{
				a |= static_cast<tuint32>(x[j]) << ((N-(j+1)) * 8);
				b |= static_cast<tuint32>(y[j]) << ((N-(j+1)) * 8);
			}
			r |= isSampleEquiv(a,b,N);

			a = 0;
			b = 0;
			for(int j=0;j<N;j++)
			{
				a |= static_cast<tuint32>(x[bytesPerSample-(j+1)]) << (j * 8);
				b |= static_cast<tuint32>(y[bytesPerSample-(j+1)]) << (j * 8);
			}
			r |= isSampleEquiv(a,b,N);

			a = 0;
			b = 0;
			for(int j=0;j<N;j++)
			{
				a |= static_cast<tuint32>(x[bytesPerSample-(j+1)]) << ((N-(j+1)) * 8);
				b |= static_cast<tuint32>(y[bytesPerSample-(j+1)]) << ((N-(j+1)) * 8);
			}
			r |= isSampleEquiv(a,b,N);

			if(!r)
			{
				int c =0;
			}
			EXPECT_TRUE(r);
			
			x += bytesPerSample;
			y += bytesPerSample;
		}
	}
	else
	{
		EXPECT_EQ(0,memcmp(expect,out,12 * bytesPerSample));
	}
#else
	EXPECT_EQ(0,memcmp(expect,out,12 * bytesPerSample));
#endif
}

//-------------------------------------------------------------------------------------------

void testSampleConvertion(tint noBits,tint bytesPerSample,bool littleEndian,bool alignHigh,const sample_t *samples,const tubyte *expect)
{
    tubyte *out = new tubyte [12 * bytesPerSample];
	
	SampleConverter sampleConverter(noBits,bytesPerSample,littleEndian,alignHigh,true);
	
	EXPECT_TRUE(sampleConverter.isSupported());
	
	EXPECT_EQ(FormatDescription::e_DataSignedInteger,sampleConverter.type());
	EXPECT_EQ(littleEndian,sampleConverter.isLittleEndian());
	EXPECT_EQ(alignHigh,sampleConverter.isAlignedHigh());
	EXPECT_EQ(noBits,sampleConverter.bits());
	EXPECT_EQ(bytesPerSample,sampleConverter.bytesPerSample());
	EXPECT_EQ(1,sampleConverter.numberOfInputChannels());
	EXPECT_EQ(1,sampleConverter.numberOfOutputChannels());
	
	sampleConverter.convert(samples,out,12);
	
	testSampleOutputConvertion(expect,out,noBits,bytesPerSample,12);
	
	delete [] out;
}

//-------------------------------------------------------------------------------------------

void testSampleUnsignedConvertion(tint noBits,tint bytesPerSample,bool littleEndian,bool alignHigh,const sample_t *samples,const tubyte *expect)
{
    tubyte *out = new tubyte [12 * bytesPerSample];
	
	SampleConverter sampleConverter(noBits,bytesPerSample,littleEndian,alignHigh,false);
	
	EXPECT_TRUE(sampleConverter.isSupported());
	
	EXPECT_EQ(FormatDescription::e_DataUnsignedInteger,sampleConverter.type());
	EXPECT_EQ(littleEndian,sampleConverter.isLittleEndian());
	EXPECT_EQ(alignHigh,sampleConverter.isAlignedHigh());
	EXPECT_EQ(noBits,sampleConverter.bits());
	EXPECT_EQ(bytesPerSample,sampleConverter.bytesPerSample());
	EXPECT_EQ(1,sampleConverter.numberOfInputChannels());
	EXPECT_EQ(1,sampleConverter.numberOfOutputChannels());
	
	sampleConverter.convert(samples,out,12);

    testSampleOutputConvertion(expect,out,noBits,bytesPerSample,12);
	
	delete [] out;
}

//-------------------------------------------------------------------------------------------

void createExpectedDoubleOutput(const sample_t *input,tfloat64 *sampleExpect,tint noSamples)
{
	for(int i=0;i<noSamples;i++)
	{
		sampleExpect[i] = static_cast<tfloat64>(input[i]);
	}
}

TEST(SampleConverter,doubleSamplesToDoubleLittleEndian)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	tfloat64 sampleExpect[12];
	createExpectedDoubleOutput(c_sampleInput,sampleExpect,12);
	
	tbyte expect[12 * 8];
	engine::writeNative64BitsAsLittleEndian(reinterpret_cast<const tbyte*>(sampleExpect),expect,12);
	
	tubyte *out = new tubyte [12 * sizeof(tfloat64)];
	
	SampleConverter sampleConverter(false,true);
	
	EXPECT_TRUE(sampleConverter.isSupported());
	
	EXPECT_EQ(FormatDescription::e_DataFloatDouble,sampleConverter.type());
	EXPECT_EQ(true,sampleConverter.isLittleEndian());
	EXPECT_EQ(false,sampleConverter.isAlignedHigh());
	EXPECT_EQ(64,sampleConverter.bits());
	EXPECT_EQ(8,sampleConverter.bytesPerSample());
	EXPECT_EQ(1,sampleConverter.numberOfInputChannels());
	EXPECT_EQ(1,sampleConverter.numberOfOutputChannels());
	
	sampleConverter.convert(c_sampleInput,out,12);
	
	EXPECT_EQ(0,memcmp(expect,out,12 * sizeof(tfloat64)));
	
	delete [] out;
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesToDoubleBigEndian)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	tfloat64 sampleExpect[12];
	createExpectedDoubleOutput(c_sampleInput,sampleExpect,12);
	
	tbyte expect[12 * 8];
    engine::writeNative64BitsAsBigEndian(reinterpret_cast<const tbyte*>(sampleExpect),expect,12);
	
    tubyte *out = new tubyte [12 * sizeof(tfloat64)];

	SampleConverter sampleConverter(false,false);
	
	EXPECT_TRUE(sampleConverter.isSupported());
	
	EXPECT_EQ(FormatDescription::e_DataFloatDouble,sampleConverter.type());
	EXPECT_EQ(false,sampleConverter.isLittleEndian());
	EXPECT_EQ(false,sampleConverter.isAlignedHigh());
	EXPECT_EQ(64,sampleConverter.bits());
	EXPECT_EQ(8,sampleConverter.bytesPerSample());
	EXPECT_EQ(1,sampleConverter.numberOfInputChannels());
	EXPECT_EQ(1,sampleConverter.numberOfOutputChannels());
	
	sampleConverter.convert(c_sampleInput,out,12);
	
	EXPECT_EQ(0,memcmp(expect,out,12 * sizeof(tfloat64)));
	
	delete [] out;
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesToFloatLittleEndian)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tfloat32 c_sampleSingle[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};	
	tbyte expect[12 * 4];
	engine::writeNative32BitsAsLittleEndian(reinterpret_cast<const tbyte*>(c_sampleSingle),expect,12);
	
    tubyte *out = new tubyte [12 * sizeof(tfloat32)];
	
	SampleConverter sampleConverter(true,true);
	
	EXPECT_TRUE(sampleConverter.isSupported());
	
	EXPECT_EQ(FormatDescription::e_DataFloatSingle,sampleConverter.type());
	EXPECT_EQ(true,sampleConverter.isLittleEndian());
	EXPECT_EQ(false,sampleConverter.isAlignedHigh());
	EXPECT_EQ(32,sampleConverter.bits());
	EXPECT_EQ(4,sampleConverter.bytesPerSample());
	EXPECT_EQ(1,sampleConverter.numberOfInputChannels());
	EXPECT_EQ(1,sampleConverter.numberOfOutputChannels());
	
	sampleConverter.convert(c_sampleInput,out,12);
	
	EXPECT_EQ(0,memcmp(expect,out,12 * sizeof(tfloat32)));
	
	delete [] out;
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesToFloatBigEndian)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tfloat32 c_sampleSingle[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
	tbyte expect[12 * 4];
	engine::writeNative32BitsAsBigEndian(reinterpret_cast<const tbyte*>(c_sampleSingle),expect,12);
	
    tubyte *out = new tubyte [12 * sizeof(tfloat32)];
	
	SampleConverter sampleConverter(true,false);
	
	EXPECT_TRUE(sampleConverter.isSupported());
	
	EXPECT_EQ(FormatDescription::e_DataFloatSingle,sampleConverter.type());
	EXPECT_EQ(false,sampleConverter.isLittleEndian());
	EXPECT_EQ(false,sampleConverter.isAlignedHigh());
	EXPECT_EQ(32,sampleConverter.bits());
	EXPECT_EQ(4,sampleConverter.bytesPerSample());
	EXPECT_EQ(1,sampleConverter.numberOfInputChannels());
	EXPECT_EQ(1,sampleConverter.numberOfOutputChannels());
	
	sampleConverter.convert(c_sampleInput,out,12);
	
	EXPECT_EQ(0,memcmp(expect,out,12 * sizeof(tfloat32)));
	
	delete [] out;
}

//-------------------------------------------------------------------------------------------
// 3-Bits
//  1.0 0x00000003 0x00000006
//  0.8 0x00000002 0x00000004
//  0.6 0x00000002 0x00000004
//  0.4 0x00000001 0x00000002
//  0.2 0x00000001 0x00000002
// -0.2 0xffffffff 0xfffffffe
// -0.4 0xfffffffe 0xfffffffc
// -0.6 0xfffffffe 0xfffffffc
// -0.8 0xfffffffd 0xfffffffa
// -1.0 0xfffffffc 0xfffffff8
//  1.2 0x00000003 0x00000006
// -1.2 0xfffffffc 0xfffffff8
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleIn8BitsLSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x03,
		0x02,
		0x02,
		0x01,
		0x01,
		0xff,
		0xfe,
		0xfe,
		0xfd,
		0xfc,
		0x03,
		0xfc
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleIn8BitsMSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x60,
		0x40,
		0x40,
		0x20,
		0x20,
		0xe0,
		0xc0,
		0xc0,
		0xa0,
		0x80,
		0x60,
		0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x03, 0x00,
		0x02, 0x00,
		0x02, 0x00,
		0x01, 0x00,
		0x01, 0x00,
		0xff, 0xff,
		0xfe, 0xff,
		0xfe, 0xff,
		0xfd, 0xff,
		0xfc, 0xff,
		0x03, 0x00,
		0xfc, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x60,
		0x00, 0x40,
		0x00, 0x40,
		0x00, 0x20,
		0x00, 0x20,
		0x00, 0xe0,
		0x00, 0xc0,
		0x00, 0xc0,
		0x00, 0xa0,
		0x00, 0x80,
		0x00, 0x60,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x03,
		0x00, 0x02,
		0x00, 0x02,
		0x00, 0x01,
		0x00, 0x01,
		0xff, 0xff,
		0xff, 0xfe,
		0xff, 0xfe,
		0xff, 0xfd,
		0xff, 0xfc,
		0x00, 0x03,
		0xff, 0xfc
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x60, 0x00,
		0x40, 0x00,
		0x40, 0x00,
		0x20, 0x00,
		0x20, 0x00,
		0xe0, 0x00,
		0xc0, 0x00,
		0xc0, 0x00,
		0xa0, 0x00,
		0x80, 0x00,
		0x60, 0x00,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x03, 0x00, 0x00,
		0x02, 0x00, 0x00,
		0x02, 0x00, 0x00,
		0x01, 0x00, 0x00,
		0x01, 0x00, 0x00,
		0xff, 0xff, 0xff,
		0xfe, 0xff, 0xff,
		0xfe, 0xff, 0xff,
		0xfd, 0xff, 0xff,
		0xfc, 0xff, 0xff,
		0x03, 0x00, 0x00,
		0xfc, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x60,
		0x00, 0x00, 0x40,
		0x00, 0x00, 0x40,
		0x00, 0x00, 0x20,
		0x00, 0x00, 0x20,
		0x00, 0x00, 0xe0,
		0x00, 0x00, 0xc0,
		0x00, 0x00, 0xc0,
		0x00, 0x00, 0xa0,
		0x00, 0x00, 0x80,
		0x00, 0x00, 0x60,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x03,
		0x00, 0x00, 0x02,
		0x00, 0x00, 0x02,
		0x00, 0x00, 0x01,
		0x00, 0x00, 0x01,
		0xff, 0xff, 0xff,
		0xff, 0xff, 0xfe,
		0xff, 0xff, 0xfe,
		0xff, 0xff, 0xfd,
		0xff, 0xff, 0xfc,
		0x00, 0x00, 0x03,
		0xff, 0xff, 0xfc
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x60, 0x00, 0x00,
		0x40, 0x00, 0x00,
		0x40, 0x00, 0x00,
		0x20, 0x00, 0x00,
		0x20, 0x00, 0x00,
		0xe0, 0x00, 0x00,
		0xc0, 0x00, 0x00,
		0xc0, 0x00, 0x00,
		0xa0, 0x00, 0x00,
		0x80, 0x00, 0x00,
		0x60, 0x00, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x03, 0x00, 0x00, 0x00,
		0x02, 0x00, 0x00, 0x00,
		0x02, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0xff,
		0xfe, 0xff, 0xff, 0xff,
		0xfe, 0xff, 0xff, 0xff,
		0xfd, 0xff, 0xff, 0xff,
		0xfc, 0xff, 0xff, 0xff,
		0x03, 0x00, 0x00, 0x00,
		0xfc, 0xff, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x60,
		0x00, 0x00, 0x00, 0x40,
		0x00, 0x00, 0x00, 0x40,
		0x00, 0x00, 0x00, 0x20,
		0x00, 0x00, 0x00, 0x20,
		0x00, 0x00, 0x00, 0xe0,
		0x00, 0x00, 0x00, 0xc0,
		0x00, 0x00, 0x00, 0xc0,
		0x00, 0x00, 0x00, 0xa0,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0x00, 0x60,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x03,
		0x00, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x02,
		0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x01,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xfe,
		0xff, 0xff, 0xff, 0xfe,
		0xff, 0xff, 0xff, 0xfd,
		0xff, 0xff, 0xff, 0xfc,
		0x00, 0x00, 0x00, 0x03,
		0xff, 0xff, 0xff, 0xfc
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo3BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 3;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x60, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x00, 0x00,
		0x20, 0x00, 0x00, 0x00,
		0x20, 0x00, 0x00, 0x00,
		0xe0, 0x00, 0x00, 0x00,
		0xc0, 0x00, 0x00, 0x00,
		0xc0, 0x00, 0x00, 0x00,
		0xa0, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x60, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 4-Bits
//  1.0 0x00000007 0x00000070
//  0.8 0x00000006 0x00000060
//  0.6 0x00000004 0x00000040
//  0.4 0x00000003 0x00000030
//  0.2 0x00000001 0x00000010
// -0.2 0xfffffffe 0xffffffe0
// -0.4 0xfffffffd 0xffffffd0
// -0.6 0xfffffffb 0xffffffb0
// -0.8 0xfffffffa 0xffffffa0
// -1.0 0xfffffff8 0xffffff80
//  1.0 0x00000007 0x00000070
// -1.0 0xfffffff8 0xffffff80
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleIn8BitsLSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x07,
		0x06,
		0x04,
		0x03,
		0x01,
		0xfe,
		0xfd,
		0xfb,
		0xfa,
		0xf8,
		0x07,
		0xf8
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleIn8BitsMSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x70,
		0x60,
		0x40,
		0x30,
		0x10,
		0xe0,
		0xd0,
		0xb0,
		0xa0,
		0x80,
		0x70,
		0x80
	};	
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x07, 0x00,
		0x06, 0x00,
		0x04, 0x00,
		0x03, 0x00,
		0x01, 0x00,
		0xfe, 0xff,
		0xfd, 0xff,
		0xfb, 0xff,
		0xfa, 0xff,
		0xf8, 0xff,
		0x07, 0x00,
		0xf8, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x70,
		0x00, 0x60,
		0x00, 0x40,
		0x00, 0x30,
		0x00, 0x10,
		0x00, 0xe0,
		0x00, 0xd0,
		0x00, 0xb0,
		0x00, 0xa0,
		0x00, 0x80,
		0x00, 0x70,
		0x00, 0x80
	};	
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x07,
		0x00, 0x06,
		0x00, 0x04,
		0x00, 0x03,
		0x00, 0x01,
		0xff, 0xfe,
		0xff, 0xfd, 
		0xff, 0xfb,
		0xff, 0xfa,
		0xff, 0xf8,
		0x00, 0x07,
		0xff, 0xf8
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x70, 0x00,
		0x60, 0x00,
		0x40, 0x00,
		0x30, 0x00,
		0x10, 0x00,
		0xe0, 0x00,
		0xd0, 0x00,
		0xb0, 0x00,
		0xa0, 0x00,
		0x80, 0x00,
		0x70, 0x00,
		0x80, 0x00
	};	
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x07, 0x00, 0x00,
		0x06, 0x00, 0x00,
		0x04, 0x00, 0x00,
		0x03, 0x00, 0x00,
		0x01, 0x00, 0x00,
		0xfe, 0xff, 0xff,
		0xfd, 0xff, 0xff,
		0xfb, 0xff, 0xff,
		0xfa, 0xff, 0xff,
		0xf8, 0xff, 0xff,
		0x07, 0x00, 0x00,
		0xf8, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x70,
		0x00, 0x00, 0x60,
		0x00, 0x00, 0x40,
		0x00, 0x00, 0x30,
		0x00, 0x00, 0x10,
		0x00, 0x00, 0xe0,
		0x00, 0x00, 0xd0,
		0x00, 0x00, 0xb0,
		0x00, 0x00, 0xa0,
		0x00, 0x00, 0x80,
		0x00, 0x00, 0x70,
		0x00, 0x00, 0x80
	};	
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x07,
		0x00, 0x00, 0x06,
		0x00, 0x00, 0x04,
		0x00, 0x00, 0x03,
		0x00, 0x00, 0x01,
		0xff, 0xff, 0xfe,
		0xff, 0xff, 0xfd, 
		0xff, 0xff, 0xfb,
		0xff, 0xff, 0xfa,
		0xff, 0xff, 0xf8,
		0x00, 0x00, 0x07,
		0xff, 0xff, 0xf8
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x70, 0x00, 0x00,
		0x60, 0x00, 0x00,
		0x40, 0x00, 0x00,
		0x30, 0x00, 0x00,
		0x10, 0x00, 0x00,
		0xe0, 0x00, 0x00,
		0xd0, 0x00, 0x00,
		0xb0, 0x00, 0x00,
		0xa0, 0x00, 0x00,
		0x80, 0x00, 0x00,
		0x70, 0x00, 0x00,
		0x80, 0x00, 0x00
	};	
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x07, 0x00, 0x00, 0x00,
		0x06, 0x00, 0x00, 0x00,
		0x04, 0x00, 0x00, 0x00,
		0x03, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x00,
		0xfe, 0xff, 0xff, 0xff,
		0xfd, 0xff, 0xff, 0xff,
		0xfb, 0xff, 0xff, 0xff,
		0xfa, 0xff, 0xff, 0xff,
		0xf8, 0xff, 0xff, 0xff,
		0x07, 0x00, 0x00, 0x00,
		0xf8, 0xff, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x70,
		0x00, 0x00, 0x00, 0x60,
		0x00, 0x00, 0x00, 0x40,
		0x00, 0x00, 0x00, 0x30,
		0x00, 0x00, 0x00, 0x10,
		0x00, 0x00, 0x00, 0xe0,
		0x00, 0x00, 0x00, 0xd0,
		0x00, 0x00, 0x00, 0xb0,
		0x00, 0x00, 0x00, 0xa0,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0x00, 0x70,
		0x00, 0x00, 0x00, 0x80
	};	
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 4;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x07,
		0x00, 0x00, 0x00, 0x06,
		0x00, 0x00, 0x00, 0x04,
		0x00, 0x00, 0x00, 0x03,
		0x00, 0x00, 0x00, 0x01,
		0xff, 0xff, 0xff, 0xfe,
		0xff, 0xff, 0xff, 0xfd, 
		0xff, 0xff, 0xff, 0xfb,
		0xff, 0xff, 0xff, 0xfa,
		0xff, 0xff, 0xff, 0xf8,
		0x00, 0x00, 0x00, 0x07,
		0xff, 0xff, 0xff, 0xf8
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo4BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 4;
    const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x70, 0x00, 0x00, 0x00,
		0x60, 0x00, 0x00, 0x00,
		0x40, 0x00, 0x00, 0x00,
		0x30, 0x00, 0x00, 0x00,
		0x10, 0x00, 0x00, 0x00,
		0xe0, 0x00, 0x00, 0x00,
		0xd0, 0x00, 0x00, 0x00,
		0xb0, 0x00, 0x00, 0x00,
		0xa0, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x70, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};	
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 5-Bits
//  1.0 0x0000000f 0x00000078
//  0.8 0x0000000c 0x00000060
//  0.6 0x00000009 0x00000048
//  0.4 0x00000006 0x00000030
//  0.2 0x00000003 0x00000018
// -0.2 0xfffffffd 0xffffffe8
// -0.4 0xfffffffa 0xffffffd0
// -0.6 0xfffffff6 0xffffffb0
// -0.8 0xfffffff3 0xffffff98
// -1.0 0xfffffff0 0xffffff80
//  1.0 0x0000000f 0x00000078
// -1.0 0xfffffff0 0xffffff80
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleIn8BitsLSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x0f,
		0x0c,
		0x09,
		0x06,
		0x03,
		0xfd,
		0xfa,
		0xf6,
		0xf3,
		0xf0,
		0x0f,
		0xf0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleIn8BitsMSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x78,
		0x60,
		0x48,
		0x30,
		0x18,
		0xe8,
		0xd0,
		0xb0,
		0x98,
		0x80,
		0x78,
		0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x0f, 0x00,
		0x0c, 0x00,
		0x09, 0x00,
		0x06, 0x00,
		0x03, 0x00,
		0xfd, 0xff,
		0xfa, 0xff,
		0xf6, 0xff,
		0xf3, 0xff,
		0xf0, 0xff,
		0x0f, 0x00,
		0xf0, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x78,
		0x00, 0x60,
		0x00, 0x48,
		0x00, 0x30,
		0x00, 0x18,
		0x00, 0xe8,
		0x00, 0xd0,
		0x00, 0xb0,
		0x00, 0x98,
		0x00, 0x80,
		0x00, 0x78,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x0f,
		0x00, 0x0c,
		0x00, 0x09,
		0x00, 0x06,
		0x00, 0x03,
		0xff, 0xfd,
		0xff, 0xfa,
		0xff, 0xf6,
		0xff, 0xf3,
		0xff, 0xf0,
		0x00, 0x0f,
		0xff, 0xf0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x78, 0x00,
		0x60, 0x00,
		0x48, 0x00,
		0x30, 0x00,
		0x18, 0x00,
		0xe8, 0x00,
		0xd0, 0x00,
		0xb0, 0x00,
		0x98, 0x00,
		0x80, 0x00,
		0x78, 0x00,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x0f, 0x00, 0x00,
		0x0c, 0x00, 0x00,
		0x09, 0x00, 0x00,
		0x06, 0x00, 0x00,
		0x03, 0x00, 0x00,
		0xfd, 0xff, 0xff,
		0xfa, 0xff, 0xff,
		0xf6, 0xff, 0xff,
		0xf3, 0xff, 0xff,
		0xf0, 0xff, 0xff,
		0x0f, 0x00, 0x00,
		0xf0, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x78,
		0x00, 0x00, 0x60,
		0x00, 0x00, 0x48,
		0x00, 0x00, 0x30,
		0x00, 0x00, 0x18,
		0x00, 0x00, 0xe8,
		0x00, 0x00, 0xd0,
		0x00, 0x00, 0xb0,
		0x00, 0x00, 0x98,
		0x00, 0x00, 0x80,
		0x00, 0x00, 0x78,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x0f,
		0x00, 0x00, 0x0c,
		0x00, 0x00, 0x09,
		0x00, 0x00, 0x06,
		0x00, 0x00, 0x03,
		0xff, 0xff, 0xfd,
		0xff, 0xff, 0xfa,
		0xff, 0xff, 0xf6,
		0xff, 0xff, 0xf3,
		0xff, 0xff, 0xf0,
		0x00, 0x00, 0x0f,
		0xff, 0xff, 0xf0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x78, 0x00, 0x00,
		0x60, 0x00, 0x00,
		0x48, 0x00, 0x00,
		0x30, 0x00, 0x00,
		0x18, 0x00, 0x00,
		0xe8, 0x00, 0x00,
		0xd0, 0x00, 0x00,
		0xb0, 0x00, 0x00,
		0x98, 0x00, 0x00,
		0x80, 0x00, 0x00,
		0x78, 0x00, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x0f, 0x00, 0x00, 0x00,
		0x0c, 0x00, 0x00, 0x00,
		0x09, 0x00, 0x00, 0x00,
		0x06, 0x00, 0x00, 0x00,
		0x03, 0x00, 0x00, 0x00,
		0xfd, 0xff, 0xff, 0xff,
		0xfa, 0xff, 0xff, 0xff,
		0xf6, 0xff, 0xff, 0xff,
		0xf3, 0xff, 0xff, 0xff,
		0xf0, 0xff, 0xff, 0xff,
		0x0f, 0x00, 0x00, 0x00,
		0xf0, 0xff, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x78,
		0x00, 0x00, 0x00, 0x60,
		0x00, 0x00, 0x00, 0x48,
		0x00, 0x00, 0x00, 0x30,
		0x00, 0x00, 0x00, 0x18,
		0x00, 0x00, 0x00, 0xe8,
		0x00, 0x00, 0x00, 0xd0,
		0x00, 0x00, 0x00, 0xb0,
		0x00, 0x00, 0x00, 0x98,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0x00, 0x78,
		0x00, 0x00, 0x00, 0x80,
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 5;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x0f,
		0x00, 0x00, 0x00, 0x0c,
		0x00, 0x00, 0x00, 0x09,
		0x00, 0x00, 0x00, 0x06,
		0x00, 0x00, 0x00, 0x03,
		0xff, 0xff, 0xff, 0xfd,
		0xff, 0xff, 0xff, 0xfa,
		0xff, 0xff, 0xff, 0xf6,
		0xff, 0xff, 0xff, 0xf3,
		0xff, 0xff, 0xff, 0xf0,
		0x00, 0x00, 0x00, 0x0f,
		0xff, 0xff, 0xff, 0xf0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo5BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 5;
    const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x78, 0x00, 0x00, 0x00,
		0x60, 0x00, 0x00, 0x00,
		0x48, 0x00, 0x00, 0x00,
		0x30, 0x00, 0x00, 0x00,
		0x18, 0x00, 0x00, 0x00,
		0xe8, 0x00, 0x00, 0x00,
		0xd0, 0x00, 0x00, 0x00,
		0xb0, 0x00, 0x00, 0x00,
		0x98, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x78, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 6-Bits
//  1.0 0x0000001f 0x0000007c
//  0.8 0x00000019 0x00000064
//  0.6 0x00000013 0x0000004c
//  0.4 0x0000000c 0x00000030
//  0.2 0x00000006 0x00000018
// -0.2 0xfffffffa 0xffffffe8
// -0.4 0xfffffff3 0xffffffcc
// -0.6 0xffffffed 0xffffffb4
// -0.8 0xffffffe6 0xffffff98
// -1.0 0xffffffe0 0xffffff80
//  1.0 0x0000001f 0x0000007c
// -1.0 0xffffffe0 0xffffff80
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleIn8BitsLSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x1f,
		0x19,
		0x13,
		0x0c,
		0x06,
		0xfa,
		0xf3,
		0xed,
		0xe6,
		0xe0,
		0x1f,
		0xe0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleIn8BitsMSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7c,
		0x64,
		0x4c,
		0x30,
		0x18,
		0xe8,
		0xcc,
		0xb4,
		0x98,
		0x80,
		0x7c,
		0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x1f, 0x00,
		0x19, 0x00,
		0x13, 0x00,
		0x0c, 0x00,
		0x06, 0x00,
		0xfa, 0xff,
		0xf3, 0xff,
		0xed, 0xff,
		0xe6, 0xff,
		0xe0, 0xff,
		0x1f, 0x00,
		0xe0, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x7c,
		0x00, 0x64,
		0x00, 0x4c,
		0x00, 0x30,
		0x00, 0x18,
		0x00, 0xe8,
		0x00, 0xcc,
		0x00, 0xb4,
		0x00, 0x98,
		0x00, 0x80,
		0x00, 0x7c,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x1f,
		0x00, 0x19,
		0x00, 0x13,
		0x00, 0x0c,
		0x00, 0x06,
		0xff, 0xfa,
		0xff, 0xf3,
		0xff, 0xed,
		0xff, 0xe6,
		0xff, 0xe0,
		0x00, 0x1f,
		0xff, 0xe0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7c, 0x00,
		0x64, 0x00,
		0x4c, 0x00,
		0x30, 0x00,
		0x18, 0x00,
		0xe8, 0x00,
		0xcc, 0x00,
		0xb4, 0x00,
		0x98, 0x00,
		0x80, 0x00,
		0x7c, 0x00,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x1f, 0x00, 0x00,
		0x19, 0x00, 0x00,
		0x13, 0x00, 0x00,
		0x0c, 0x00, 0x00,
		0x06, 0x00, 0x00,
		0xfa, 0xff, 0xff,
		0xf3, 0xff, 0xff,
		0xed, 0xff, 0xff,
		0xe6, 0xff, 0xff,
		0xe0, 0xff, 0xff,
		0x1f, 0x00, 0x00,
		0xe0, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x7c,
		0x00, 0x00, 0x64,
		0x00, 0x00, 0x4c,
		0x00, 0x00, 0x30,
		0x00, 0x00, 0x18,
		0x00, 0x00, 0xe8,
		0x00, 0x00, 0xcc,
		0x00, 0x00, 0xb4,
		0x00, 0x00, 0x98,
		0x00, 0x00, 0x80,
		0x00, 0x00, 0x7c,
		0x00, 0x00, 0x80,
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x1f,
		0x00, 0x00, 0x19,
		0x00, 0x00, 0x13,
		0x00, 0x00, 0x0c,
		0x00, 0x00, 0x06,
		0xff, 0xff, 0xfa,
		0xff, 0xff, 0xf3,
		0xff, 0xff, 0xed,
		0xff, 0xff, 0xe6,
		0xff, 0xff, 0xe0,
		0x00, 0x00, 0x1f,
		0xff, 0xff, 0xe0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7c, 0x00, 0x00,
		0x64, 0x00, 0x00,
		0x4c, 0x00, 0x00,
		0x30, 0x00, 0x00,
		0x18, 0x00, 0x00,
		0xe8, 0x00, 0x00,
		0xcc, 0x00, 0x00,
		0xb4, 0x00, 0x00,
		0x98, 0x00, 0x00,
		0x80, 0x00, 0x00,
		0x7c, 0x00, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x1f, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0x13, 0x00, 0x00, 0x00,
		0x0c, 0x00, 0x00, 0x00,
		0x06, 0x00, 0x00, 0x00,
		0xfa, 0xff, 0xff, 0xff,
		0xf3, 0xff, 0xff, 0xff,
		0xed, 0xff, 0xff, 0xff,
		0xe6, 0xff, 0xff, 0xff,
		0xe0, 0xff, 0xff, 0xff,
		0x1f, 0x00, 0x00, 0x00,
		0xe0, 0xff, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x7c,
		0x00, 0x00, 0x00, 0x64,
		0x00, 0x00, 0x00, 0x4c,
		0x00, 0x00, 0x00, 0x30,
		0x00, 0x00, 0x00, 0x18,
		0x00, 0x00, 0x00, 0xe8,
		0x00, 0x00, 0x00, 0xcc,
		0x00, 0x00, 0x00, 0xb4,
		0x00, 0x00, 0x00, 0x98,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0x00, 0x7c,
		0x00, 0x00, 0x00, 0x80,
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x1f,
		0x00, 0x00, 0x00, 0x19,
		0x00, 0x00, 0x00, 0x13,
		0x00, 0x00, 0x00, 0x0c,
		0x00, 0x00, 0x00, 0x06,
		0xff, 0xff, 0xff, 0xfa,
		0xff, 0xff, 0xff, 0xf3,
		0xff, 0xff, 0xff, 0xed,
		0xff, 0xff, 0xff, 0xe6,
		0xff, 0xff, 0xff, 0xe0,
		0x00, 0x00, 0x00, 0x1f,
		0xff, 0xff, 0xff, 0xe0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo6BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 6;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7c, 0x00, 0x00, 0x00,
		0x64, 0x00, 0x00, 0x00,
		0x4c, 0x00, 0x00, 0x00,
		0x30, 0x00, 0x00, 0x00,
		0x18, 0x00, 0x00, 0x00,
		0xe8, 0x00, 0x00, 0x00,
		0xcc, 0x00, 0x00, 0x00,
		0xb4, 0x00, 0x00, 0x00,
		0x98, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7c, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 7-Bits
//  1.0 0x0000003f 0x0000007e
//  0.8 0x00000032 0x00000064
//  0.6 0x00000026 0x0000004c
//  0.4 0x00000019 0x00000032
//  0.2 0x0000000d 0x0000001a
// -0.2 0xfffffff3 0xffffffe6
// -0.4 0xffffffe6 0xffffffcc
// -0.6 0xffffffda 0xffffffb4
// -0.8 0xffffffcd 0xffffff9a
// -1.0 0xffffffc0 0xffffff80
//  1.0 0x0000003f 0x0000007e
// -1.0 0xffffffc0 0xffffff80
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleIn8BitsLSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x3f,
		0x32,
		0x26,
		0x19,
		0x0d,
		0xf3,
		0xe6,
		0xda,
		0xcd,
		0xc0,
		0x3f,
		0xc0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleIn8BitsUnsignedLSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f,
		0x72,
		0x66,
		0x59,
		0x4d,
		0x33,
		0x26,
		0x1a,
		0x0d,
		0x00,
		0x7f,
		0x00
	};
	
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleIn8BitsMSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7e,
		0x64,
		0x4c,
		0x32,
		0x1a,
		0xe6,
		0xcc,
		0xb4,
		0x9a,
		0x80,
		0x7e,
		0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleIn8BitsUnsignedMSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xfe,
		0xe4,
		0xcc,
		0xb2,
		0x9a,
		0x66,
		0x4c,
		0x34,
		0x1a,
		0x00,
		0xfe,
		0x00
	};
	
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x3f, 0x00,
		0x32, 0x00,
		0x26, 0x00,
		0x19, 0x00,
		0x0d, 0x00,
		0xf3, 0xff,
		0xe6, 0xff,
		0xda, 0xff,
		0xcd, 0xff,
		0xc0, 0xff,
		0x3f, 0x00,
		0xc0, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x7e,
		0x00, 0x64,
		0x00, 0x4c,
		0x00, 0x32,
		0x00, 0x1a,
		0x00, 0xe6,
		0x00, 0xcc,
		0x00, 0xb4,
		0x00, 0x9a,
		0x00, 0x80,
		0x00, 0x7e,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x3f,
		0x00, 0x32,
		0x00, 0x26,
		0x00, 0x19,
		0x00, 0x0d,
		0xff, 0xf3,
		0xff, 0xe6,
		0xff, 0xda,
		0xff, 0xcd,
		0xff, 0xc0,
		0x00, 0x3f,
		0xff, 0xc0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7e, 0x00,
		0x64, 0x00,
		0x4c, 0x00,
		0x32, 0x00,
		0x1a, 0x00,
		0xe6, 0x00,
		0xcc, 0x00,
		0xb4, 0x00,
		0x9a, 0x00,
		0x80, 0x00,
		0x7e, 0x00,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x3f, 0x00, 0x00,
		0x32, 0x00, 0x00,
		0x26, 0x00, 0x00,
		0x19, 0x00, 0x00,
		0x0d, 0x00, 0x00,
		0xf3, 0xff, 0xff,
		0xe6, 0xff, 0xff,
		0xda, 0xff, 0xff,
		0xcd, 0xff, 0xff,
		0xc0, 0xff, 0xff,
		0x3f, 0x00, 0x00,
		0xc0, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x7e,
		0x00, 0x00, 0x64,
		0x00, 0x00, 0x4c,
		0x00, 0x00, 0x32,
		0x00, 0x00, 0x1a,
		0x00, 0x00, 0xe6,
		0x00, 0x00, 0xcc,
		0x00, 0x00, 0xb4,
		0x00, 0x00, 0x9a,
		0x00, 0x00, 0x80,
		0x00, 0x00, 0x7e,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x3f,
		0x00, 0x00, 0x32,
		0x00, 0x00, 0x26,
		0x00, 0x00, 0x19,
		0x00, 0x00, 0x0d,
		0xff, 0xff, 0xf3,
		0xff, 0xff, 0xe6,
		0xff, 0xff, 0xda,
		0xff, 0xff, 0xcd,
		0xff, 0xff, 0xc0,
		0x00, 0x00, 0x3f,
		0xff, 0xff, 0xc0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7e, 0x00, 0x00,
		0x64, 0x00, 0x00,
		0x4c, 0x00, 0x00,
		0x32, 0x00, 0x00,
		0x1a, 0x00, 0x00,
		0xe6, 0x00, 0x00,
		0xcc, 0x00, 0x00,
		0xb4, 0x00, 0x00,
		0x9a, 0x00, 0x00,
		0x80, 0x00, 0x00,
		0x7e, 0x00, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x3f, 0x00, 0x00, 0x00,
		0x32, 0x00, 0x00, 0x00,
		0x26, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0x0d, 0x00, 0x00, 0x00,
		0xf3, 0xff, 0xff, 0xff,
		0xe6, 0xff, 0xff, 0xff,
		0xda, 0xff, 0xff, 0xff,
		0xcd, 0xff, 0xff, 0xff,
		0xc0, 0xff, 0xff, 0xff,
		0x3f, 0x00, 0x00, 0x00,
		0xc0, 0xff, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x7e,
		0x00, 0x00, 0x00, 0x64,
		0x00, 0x00, 0x00, 0x4c,
		0x00, 0x00, 0x00, 0x32,
		0x00, 0x00, 0x00, 0x1a,
		0x00, 0x00, 0x00, 0xe6,
		0x00, 0x00, 0x00, 0xcc,
		0x00, 0x00, 0x00, 0xb4,
		0x00, 0x00, 0x00, 0x9a,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0x00, 0x7e,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x3f,
		0x00, 0x00, 0x00, 0x32,
		0x00, 0x00, 0x00, 0x26,
		0x00, 0x00, 0x00, 0x19,
		0x00, 0x00, 0x00, 0x0d,
		0xff, 0xff, 0xff, 0xf3,
		0xff, 0xff, 0xff, 0xe6,
		0xff, 0xff, 0xff, 0xda,
		0xff, 0xff, 0xff, 0xcd,
		0xff, 0xff, 0xff, 0xc0,
		0x00, 0x00, 0x00, 0x3f,
		0xff, 0xff, 0xff, 0xc0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo7BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7e, 0x00, 0x00, 0x00,
		0x64, 0x00, 0x00, 0x00,
		0x4c, 0x00, 0x00, 0x00,
		0x32, 0x00, 0x00, 0x00,
		0x1a, 0x00, 0x00, 0x00,
		0xe6, 0x00, 0x00, 0x00,
		0xcc, 0x00, 0x00, 0x00,
		0xb4, 0x00, 0x00, 0x00,
		0x9a, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7e, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 8-Bits
//  1.0 0x0000007f 0x000007f0
//  0.8 0x00000066 0x00000660
//  0.6 0x0000004c 0x000004c0
//  0.4 0x00000033 0x00000330
//  0.2 0x00000019 0x00000190
// -0.2 0xffffffe6 0xfffffe60
// -0.4 0xffffffcd 0xfffffcd0
// -0.6 0xffffffb3 0xfffffb30
// -0.8 0xffffff9a 0xfffff9a0
// -1.0 0xffffff80 0xfffff800
//  1.0 0x0000007f 0x000007f0
// -1.0 0xffffff80 0xfffff800
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleIn8Bits)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f,
		0x66,
		0x4c,
		0x33,
		0x19,
		0xe6,
		0xcd,
		0xb3,
		0x9a,
		0x80,
		0x7f,
		0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0x00,
		0x66, 0x00,
		0x4c, 0x00,
		0x33, 0x00,
		0x19, 0x00,
		0xe6, 0xff,
		0xcd, 0xff,
		0xb3, 0xff,
		0x9a, 0xff,
		0x80, 0xff,
		0x7f, 0x00,
		0x80, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x7f,
		0x00, 0x66,
		0x00, 0x4c,
		0x00, 0x33,
		0x00, 0x19,
		0x00, 0xe6,
		0x00, 0xcd,
		0x00, 0xb3,
		0x00, 0x9a,
		0x00, 0x80,
		0x00, 0x7f,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x7f,
		0x00, 0x66,
		0x00, 0x4c,
		0x00, 0x33,
		0x00, 0x19,
		0xff, 0xe6,
		0xff, 0xcd,
		0xff, 0xb3,
		0xff, 0x9a,
		0xff, 0x80,
		0x00, 0x7f,
		0xff, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0x00,
		0x66, 0x00,
		0x4c, 0x00,
		0x33, 0x00,
		0x19, 0x00,
		0xe6, 0x00,
		0xcd, 0x00,
		0xb3, 0x00,
		0x9a, 0x00,
		0x80, 0x00,
		0x7f, 0x00,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0x00, 0x00,
		0x66, 0x00, 0x00,
		0x4c, 0x00, 0x00,
		0x33, 0x00, 0x00,
		0x19, 0x00, 0x00,
		0xe6, 0xff, 0xff,
		0xcd, 0xff, 0xff,
		0xb3, 0xff, 0xff,
		0x9a, 0xff, 0xff,
		0x80, 0xff, 0xff,
		0x7f, 0x00, 0x00,
		0x80, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x7f,
		0x00, 0x00, 0x66,
		0x00, 0x00, 0x4c,
		0x00, 0x00, 0x33,
		0x00, 0x00, 0x19,
		0x00, 0x00, 0xe6,
		0x00, 0x00, 0xcd,
		0x00, 0x00, 0xb3,
		0x00, 0x00, 0x9a,
		0x00, 0x00, 0x80,
		0x00, 0x00, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x7f,
		0x00, 0x00, 0x66,
		0x00, 0x00, 0x4c,
		0x00, 0x00, 0x33,
		0x00, 0x00, 0x19,
		0xff, 0xff, 0xe6,
		0xff, 0xff, 0xcd,
		0xff, 0xff, 0xb3,
		0xff, 0xff, 0x9a,
		0xff, 0xff, 0x80,
		0x00, 0x00, 0x7f,
		0xff, 0xff, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0x00, 0x00,
		0x66, 0x00, 0x00,
		0x4c, 0x00, 0x00,
		0x33, 0x00, 0x00,
		0x19, 0x00, 0x00,
		0xe6, 0x00, 0x00,
		0xcd, 0x00, 0x00,
		0xb3, 0x00, 0x00,
		0x9a, 0x00, 0x00,
		0x80, 0x00, 0x00,
		0x7f, 0x00, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0x00, 0x00, 0x00,
		0x66, 0x00, 0x00, 0x00,
		0x4c, 0x00, 0x00, 0x00,
		0x33, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0xe6, 0xff, 0xff, 0xff,
		0xcd, 0xff, 0xff, 0xff,
		0xb3, 0xff, 0xff, 0xff,
		0x9a, 0xff, 0xff, 0xff,
		0x80, 0xff, 0xff, 0xff,
		0x7f, 0x00, 0x00, 0x00,
		0x80, 0xff, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x7f,
		0x00, 0x00, 0x00, 0x66,
		0x00, 0x00, 0x00, 0x4c,
		0x00, 0x00, 0x00, 0x33,
		0x00, 0x00, 0x00, 0x19,
		0x00, 0x00, 0x00, 0xe6,
		0x00, 0x00, 0x00, 0xcd,
		0x00, 0x00, 0x00, 0xb3,
		0x00, 0x00, 0x00, 0x9a,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0x00, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0x7f,
		0x00, 0x00, 0x00, 0x66,
		0x00, 0x00, 0x00, 0x4c,
		0x00, 0x00, 0x00, 0x33,
		0x00, 0x00, 0x00, 0x19,
		0xff, 0xff, 0xff, 0xe6,
		0xff, 0xff, 0xff, 0xcd,
		0xff, 0xff, 0xff, 0xb3,
		0xff, 0xff, 0xff, 0x9a,
		0xff, 0xff, 0xff, 0x80,
		0x00, 0x00, 0x00, 0x7f,
		0xff, 0xff, 0xff, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo8BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 8;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0x00, 0x00, 0x00,
		0x66, 0x00, 0x00, 0x00,
		0x4c, 0x00, 0x00, 0x00,
		0x33, 0x00, 0x00, 0x00,
		0x19, 0x00, 0x00, 0x00,
		0xe6, 0x00, 0x00, 0x00,
		0xcd, 0x00, 0x00, 0x00,
		0xb3, 0x00, 0x00, 0x00,
		0x9a, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 9-Bits
//  1.0 0x000000ff 0x000007f8
//  0.8 0x000000cc 0x00000660
//  0.6 0x00000099 0x000004c8
//  0.4 0x00000066 0x00000330
//  0.2 0x00000033 0x00000198
// -0.2 0xffffffcd 0xfffffe68
// -0.4 0xffffff9a 0xfffffcd0
// -0.6 0xffffff66 0xfffffb30
// -0.8 0xffffff33 0xfffff998
// -1.0 0xffffff00 0xfffff800
//  1.0 0x000000ff 0x000007f8
// -1.0 0xffffff00 0xfffff800
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x00,
		0xcc, 0x00,
		0x99, 0x00,
		0x66, 0x00,
		0x33, 0x00,
		0xcd, 0xff,
		0x9a, 0xff,
		0x66, 0xff,
		0x33, 0xff,
		0x00, 0xff,
		0xff, 0x00,
		0x00, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x80, 0x7f,
		0x00, 0x66,
		0x80, 0x4c,
		0x00, 0x33,
		0x80, 0x19,
		0x80, 0xe6,
		0x00, 0xcd,
		0x00, 0xb3,
		0x80, 0x99,
		0x00, 0x80,
		0x80, 0x7f,
		0x00, 0x80,
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xff,
		0x00, 0xcc,
		0x00, 0x99,
		0x00, 0x66,
		0x00, 0x33,
		0xff, 0xcd,
		0xff, 0x9a,
		0xff, 0x66,
		0xff, 0x33,
		0xff, 0x00,
		0x00, 0xff,
		0xff, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0x80,
		0x66, 0x00,
		0x4c, 0x80,
		0x33, 0x00,
		0x19, 0x80,
		0xe6, 0x80,
		0xcd, 0x00,
		0xb3, 0x00,
		0x99, 0x80,
		0x80, 0x00,
		0x7f, 0x80,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x00, 0x00,
		0xcc, 0x00, 0x00,
		0x99, 0x00, 0x00,
		0x66, 0x00, 0x00,
		0x33, 0x00, 0x00,
		0xcd, 0xff, 0xff,
		0x9a, 0xff, 0xff,
		0x66, 0xff, 0xff,
		0x33, 0xff, 0xff,
		0x00, 0xff, 0xff,
		0xff, 0x00, 0x00,
		0x00, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x80, 0x7f,
		0x00, 0x00, 0x66,
		0x00, 0x80, 0x4c,
		0x00, 0x00, 0x33,
		0x00, 0x80, 0x19,
		0x00, 0x80, 0xe6,
		0x00, 0x00, 0xcd,
		0x00, 0x00, 0xb3,
		0x00, 0x80, 0x99,
		0x00, 0x00, 0x80,
		0x00, 0x80, 0x7f,
		0x00, 0x00, 0x80,
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0xff,
		0x00, 0x00, 0xcc,
		0x00, 0x00, 0x99,
		0x00, 0x00, 0x66,
		0x00, 0x00, 0x33,
		0xff, 0xff, 0xcd,
		0xff, 0xff, 0x9a,
		0xff, 0xff, 0x66,
		0xff, 0xff, 0x33,
		0xff, 0xff, 0x00,
		0x00, 0x00, 0xff,
		0xff, 0xff, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f,0x80,0x00,
		0x66,0x00,0x00,
		0x4c,0x80,0x00,
		0x33,0x00,0x00,
		0x19,0x80,0x00,
		0xe6,0x80,0x00,
		0xcd,0x00,0x00,
		0xb3,0x00,0x00,
		0x99,0x80,0x00,
		0x80,0x00,0x00,
		0x7f,0x80,0x00,
		0x80,0x00,0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x00, 0x00, 0x00,
		0xcc, 0x00, 0x00, 0x00,
		0x99, 0x00, 0x00, 0x00,
		0x66, 0x00, 0x00, 0x00,
		0x33, 0x00, 0x00, 0x00,
		0xcd, 0xff, 0xff, 0xff,
		0x9a, 0xff, 0xff, 0xff,
		0x66, 0xff, 0xff, 0xff,
		0x33, 0xff, 0xff, 0xff,
		0x00, 0xff, 0xff, 0xff,
		0xff, 0x00, 0x00, 0x00,
		0x00, 0xff, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x80, 0x7f,
		0x00, 0x00, 0x00, 0x66,
		0x00, 0x00, 0x80, 0x4c,
		0x00, 0x00, 0x00, 0x33,
		0x00, 0x00, 0x80, 0x19,
		0x00, 0x00, 0x80, 0xe6,
		0x00, 0x00, 0x00, 0xcd,
		0x00, 0x00, 0x00, 0xb3,
		0x00, 0x00, 0x80, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0x80, 0x7f,
		0x00, 0x00, 0x00, 0x80,
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x00, 0xff,
		0x00, 0x00, 0x00, 0xcc,
		0x00, 0x00, 0x00, 0x99,
		0x00, 0x00, 0x00, 0x66,
		0x00, 0x00, 0x00, 0x33,
		0xff, 0xff, 0xff, 0xcd,
		0xff, 0xff, 0xff, 0x9a,
		0xff, 0xff, 0xff, 0x66,
		0xff, 0xff, 0xff, 0x33,
		0xff, 0xff, 0xff, 0x00,
		0x00, 0x00, 0x00, 0xff,
		0xff, 0xff, 0xff, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo9BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 9;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f,0x80,0x00,0x00,
		0x66,0x00,0x00,0x00,
		0x4c,0x80,0x00,0x00,
		0x33,0x00,0x00,0x00,
		0x19,0x80,0x00,0x00,
		0xe6,0x80,0x00,0x00,
		0xcd,0x00,0x00,0x00,
		0xb3,0x00,0x00,0x00,
		0x99,0x80,0x00,0x00,
		0x80,0x00,0x00,0x00,
		0x7f,0x80,0x00,0x00,
		0x80,0x00,0x00,0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 10-Bits
//  1.0 0x000001ff 0x000007fc
//  0.8 0x00000199 0x00000664
//  0.6 0x00000133 0x000004cc
//  0.4 0x000000cc 0x00000330
//  0.2 0x00000066 0x00000198
// -0.2 0xffffff9a 0xfffffe68
// -0.4 0xffffff33 0xfffffccc
// -0.6 0xfffffecd 0xfffffb34
// -0.8 0xfffffe66 0xfffff998
// -1.0 0xfffffe00 0xfffff800
//  1.0 0x000001ff 0x000007fc
// -1.0 0xfffffe00 0xfffff800
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x01,
		0x99, 0x01,
		0x33, 0x01,
		0xcc, 0x00,
		0x66, 0x00,
		0x9a, 0xff,
		0x33, 0xff,
		0xcd, 0xfe,
		0x66, 0xfe,
		0x00, 0xfe,
		0xff, 0x01,
		0x00, 0xfe
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xc0, 0x7f,
		0x40, 0x66,
		0xc0, 0x4c,
		0x00, 0x33,
		0x80, 0x19,
		0x80, 0xe6,
		0xc0, 0xcc,
		0x40, 0xb3,
		0x80, 0x99,
		0x00, 0x80,
		0xc0, 0x7f,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x01, 0xff,
		0x01, 0x99,
		0x01, 0x33,
		0x00, 0xcc,
		0x00, 0x66,
		0xff, 0x9a,
		0xff, 0x33,
		0xfe, 0xcd,
		0xfe, 0x66,
		0xfe, 0x00,
		0x01, 0xff,
		0xfe, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xc0,
		0x66, 0x40,
		0x4c, 0xc0,
		0x33, 0x00,
		0x19, 0x80,
		0xe6, 0x80,
		0xcc, 0xc0,
		0xb3, 0x40,
		0x99, 0x80,
		0x80, 0x00,
		0x7f, 0xc0,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x01, 0x00,
		0x99, 0x01, 0x00,
		0x33, 0x01, 0x00,
		0xcc, 0x00, 0x00,
		0x66, 0x00, 0x00,
		0x9a, 0xff, 0xff,
		0x33, 0xff, 0xff,
		0xcd, 0xfe, 0xff,
		0x66, 0xfe, 0xff,
		0x00, 0xfe, 0xff,
		0xff, 0x01, 0x00,
		0x00, 0xfe, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xc0, 0x7f,
		0x00, 0x40, 0x66,
		0x00, 0xc0, 0x4c,
		0x00, 0x00, 0x33,
		0x00, 0x80, 0x19,
		0x00, 0x80, 0xe6,
		0x00, 0xc0, 0xcc,
		0x00, 0x40, 0xb3,
		0x00, 0x80, 0x99,
		0x00, 0x00, 0x80,
		0x00, 0xc0, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x01, 0xff,
		0x00, 0x01, 0x99,
		0x00, 0x01, 0x33,
		0x00, 0x00, 0xcc,
		0x00, 0x00, 0x66,
		0xff, 0xff, 0x9a,
		0xff, 0xff, 0x33,
		0xff, 0xfe, 0xcd,
		0xff, 0xfe, 0x66,
		0xff, 0xfe, 0x00,
		0x00, 0x01, 0xff,
		0xff, 0xfe, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xc0, 0x00,
		0x66, 0x40, 0x00,
		0x4c, 0xc0, 0x00,
		0x33, 0x00, 0x00,
		0x19, 0x80, 0x00,
		0xe6, 0x80, 0x00,
		0xcc, 0xc0, 0x00,
		0xb3, 0x40, 0x00,
		0x99, 0x80, 0x00,
		0x80, 0x00, 0x00,
		0x7f, 0xc0, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x01, 0x00, 0x00,
		0x99, 0x01, 0x00, 0x00,
		0x33, 0x01, 0x00, 0x00,
		0xcc, 0x00, 0x00, 0x00,
		0x66, 0x00, 0x00, 0x00,
		0x9a, 0xff, 0xff, 0xff,
		0x33, 0xff, 0xff, 0xff,
		0xcd, 0xfe, 0xff, 0xff,
		0x66, 0xfe, 0xff, 0xff,
		0x00, 0xfe, 0xff, 0xff,
		0xff, 0x01, 0x00, 0x00,
		0x00, 0xfe, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0xc0, 0x7f,
		0x00, 0x00, 0x40, 0x66,
		0x00, 0x00, 0xc0, 0x4c,
		0x00, 0x00, 0x00, 0x33,
		0x00, 0x00, 0x80, 0x19,
		0x00, 0x00, 0x80, 0xe6,
		0x00, 0x00, 0xc0, 0xcc,
		0x00, 0x00, 0x40, 0xb3,
		0x00, 0x00, 0x80, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0xc0, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x01, 0xff,
		0x00, 0x00, 0x01, 0x99,
		0x00, 0x00, 0x01, 0x33,
		0x00, 0x00, 0x00, 0xcc,
		0x00, 0x00, 0x00, 0x66,
		0xff, 0xff, 0xff, 0x9a,
		0xff, 0xff, 0xff, 0x33,
		0xff, 0xff, 0xfe, 0xcd,
		0xff, 0xff, 0xfe, 0x66,
		0xff, 0xff, 0xfe, 0x00,
		0x00, 0x00, 0x01, 0xff,
		0xff, 0xff, 0xfe, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo10BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 10;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xc0, 0x00, 0x00,
		0x66, 0x40, 0x00, 0x00,
		0x4c, 0xc0, 0x00, 0x00,
		0x33, 0x00, 0x00, 0x00,
		0x19, 0x80, 0x00, 0x00,
		0xe6, 0x80, 0x00, 0x00,
		0xcc, 0xc0, 0x00, 0x00,
		0xb3, 0x40, 0x00, 0x00,
		0x99, 0x80, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xc0, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 11-Bits
//  1.0 0x000003ff 0x000007fe
//  0.8 0x00000332 0x00000664
//  0.6 0x00000266 0x000004cc
//  0.4 0x00000199 0x00000332
//  0.2 0x000000cd 0x0000019a
// -0.2 0xffffff33 0xfffffe66
// -0.4 0xfffffe66 0xfffffccc
// -0.6 0xfffffd9a 0xfffffb34
// -0.8 0xfffffccd 0xfffff99a
// -1.0 0xfffffc00 0xfffff800
//  1.0 0x000003ff 0x000007fe
// -1.0 0xfffffc00 0xfffff800
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x03,
		0x32, 0x03,
		0x66, 0x02,
		0x99, 0x01,
		0xcd, 0x00,
		0x33, 0xff,
		0x66, 0xfe,
		0x9a, 0xfd,
		0xcd, 0xfc,
		0x00, 0xfc,
		0xff, 0x03,
		0x00, 0xfc
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xe0, 0x7f,
		0x40, 0x66,
		0xc0, 0x4c,
		0x20, 0x33,
		0xa0, 0x19,
        0x60, 0xe6,
		0xc0, 0xcc,
		0x40, 0xb3,
		0xa0, 0x99,
		0x00, 0x80,
		0xe0, 0x7f,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x03, 0xff,
		0x03, 0x32,
		0x02, 0x66,
		0x01, 0x99,
		0x00, 0xcd,
		0xff, 0x33,
		0xfe, 0x66,
		0xfd, 0x9a,
		0xfc, 0xcd,
		0xfc, 0x00,
		0x03, 0xff,
		0xfc, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xe0,
		0x66, 0x40,
		0x4c, 0xc0,
		0x33, 0x20,
		0x19, 0xa0,
		0xe6, 0x60,
		0xcc, 0xc0,
		0xb3, 0x40,
		0x99, 0xa0,
		0x80, 0x00,
		0x7f, 0xe0,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x03, 0x00,
		0x32, 0x03, 0x00,
		0x66, 0x02, 0x00,
		0x99, 0x01, 0x00,
		0xcd, 0x00, 0x00,
		0x33, 0xff, 0xff,
		0x66, 0xfe, 0xff,
		0x9a, 0xfd, 0xff,
		0xcd, 0xfc, 0xff,
		0x00, 0xfc, 0xff,
		0xff, 0x03, 0x00,
		0x00, 0xfc, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xe0, 0x7f,
		0x00, 0x40, 0x66,
		0x00, 0xc0, 0x4c,
		0x00, 0x20, 0x33,
		0x00, 0xa0, 0x19,
        0x00, 0x60, 0xe6,
		0x00, 0xc0, 0xcc,
		0x00, 0x40, 0xb3,
		0x00, 0xa0, 0x99,
		0x00, 0x00, 0x80,
		0x00, 0xe0, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x03, 0xff,
		0x00, 0x03, 0x32,
		0x00, 0x02, 0x66,
		0x00, 0x01, 0x99,
		0x00, 0x00, 0xcd,
		0xff, 0xff, 0x33,
		0xff, 0xfe, 0x66,
		0xff, 0xfd, 0x9a,
		0xff, 0xfc, 0xcd,
		0xff, 0xfc, 0x00,
		0x00, 0x03, 0xff,
		0xff, 0xfc, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xe0, 0x00,
		0x66, 0x40, 0x00,
		0x4c, 0xc0, 0x00,
		0x33, 0x20, 0x00,
		0x19, 0xa0, 0x00,
		0xe6, 0x60, 0x00,
		0xcc, 0xc0, 0x00,
		0xb3, 0x40, 0x00,
		0x99, 0xa0, 0x00,
		0x80, 0x00, 0x00,
		0x7f, 0xe0, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x03, 0x00, 0x00,
		0x32, 0x03, 0x00, 0x00,
		0x66, 0x02, 0x00, 0x00,
		0x99, 0x01, 0x00, 0x00,
		0xcd, 0x00, 0x00, 0x00,
		0x33, 0xff, 0xff, 0xff,
		0x66, 0xfe, 0xff, 0xff,
		0x9a, 0xfd, 0xff, 0xff,
		0xcd, 0xfc, 0xff, 0xff,
		0x00, 0xfc, 0xff, 0xff,
		0xff, 0x03, 0x00, 0x00,
		0x00, 0xfc, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0xe0, 0x7f,
		0x00, 0x00, 0x40, 0x66,
		0x00, 0x00, 0xc0, 0x4c,
		0x00, 0x00, 0x20, 0x33,
		0x00, 0x00, 0xa0, 0x19,
        0x00, 0x00, 0x60, 0xe6,
		0x00, 0x00, 0xc0, 0xcc,
		0x00, 0x00, 0x40, 0xb3,
		0x00, 0x00, 0xa0, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0xe0, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x03, 0xff,
		0x00, 0x00, 0x03, 0x32,
		0x00, 0x00, 0x02, 0x66,
		0x00, 0x00, 0x01, 0x99,
		0x00, 0x00, 0x00, 0xcd,
		0xff, 0xff, 0xff, 0x33,
		0xff, 0xff, 0xfe, 0x66,
		0xff, 0xff, 0xfd, 0x9a,
		0xff, 0xff, 0xfc, 0xcd,
		0xff, 0xff, 0xfc, 0x00,
		0x00, 0x00, 0x03, 0xff,
		0xff, 0xff, 0xfc, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo11BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 11;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xe0, 0x00, 0x00,
		0x66, 0x40, 0x00, 0x00,
		0x4c, 0xc0, 0x00, 0x00,
		0x33, 0x20, 0x00, 0x00,
		0x19, 0xa0, 0x00, 0x00,
		0xe6, 0x60, 0x00, 0x00,
		0xcc, 0xc0, 0x00, 0x00,
		0xb3, 0x40, 0x00, 0x00,
		0x99, 0xa0, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xe0, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 12-Bits
//  1.0 0x000007ff 0x00007ff0
//  0.8 0x00000666 0x00006660
//  0.6 0x000004cc 0x00004cc0
//  0.4 0x00000333 0x00003330
//  0.2 0x00000199 0x00001990
// -0.2 0xfffffe66 0xffffe660
// -0.4 0xfffffccd 0xffffccd0
// -0.6 0xfffffb33 0xffffb330
// -0.8 0xfffff99a 0xffff99a0
// -1.0 0xfffff800 0xffff8000
//  1.0 0x000007ff 0x00007ff0
// -1.0 0xfffff800 0xffff8000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x07,
		0x66, 0x06,
		0xcc, 0x04,
		0x33, 0x03,
		0x99, 0x01,
		0x66, 0xfe,
		0xcd, 0xfc,
		0x33, 0xfb,
		0x9a, 0xf9,
		0x00, 0xf8,
		0xff, 0x07,
		0x00, 0xf8
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xf0, 0x7f,
		0x60, 0x66,
		0xc0, 0x4c,
		0x30, 0x33,
		0x90, 0x19,
		0x60, 0xe6,
		0xd0, 0xcc,
		0x30, 0xb3,
		0xa0, 0x99,
		0x00, 0x80,
		0xf0, 0x7f,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x07, 0xff,
		0x06, 0x66,
		0x04, 0xcc,
		0x03, 0x33,
		0x01, 0x99,
		0xfe, 0x66,
		0xfc, 0xcd,
		0xfb, 0x33,
		0xf9, 0x9a,
		0xf8, 0x00,
		0x07, 0xff,
		0xf8, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xf0,
		0x66, 0x60,
		0x4c, 0xc0,
		0x33, 0x30,
		0x19, 0x90,
		0xe6, 0x60,
		0xcc, 0xd0,
		0xb3, 0x30,
		0x99, 0xa0,
		0x80, 0x00,
		0x7f, 0xf0,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x07, 0x00,
		0x66, 0x06, 0x00,
		0xcc, 0x04, 0x00,
		0x33, 0x03, 0x00,
		0x99, 0x01, 0x00,
		0x66, 0xfe, 0xff,
		0xcd, 0xfc, 0xff,
		0x33, 0xfb, 0xff,
		0x9a, 0xf9, 0xff,
		0x00, 0xf8, 0xff,
		0xff, 0x07, 0x00,
		0x00, 0xf8, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xf0, 0x7f,
		0x00, 0x60, 0x66,
		0x00, 0xc0, 0x4c,
		0x00, 0x30, 0x33,
		0x00, 0x90, 0x19,
		0x00, 0x60, 0xe6,
		0x00, 0xd0, 0xcc,
		0x00, 0x30, 0xb3,
		0x00, 0xa0, 0x99,
		0x00, 0x00, 0x80,
		0x00, 0xf0, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x07, 0xff,
		0x00, 0x06, 0x66,
		0x00, 0x04, 0xcc,
		0x00, 0x03, 0x33,
		0x00, 0x01, 0x99,
		0xff, 0xfe, 0x66,
		0xff, 0xfc, 0xcd,
		0xff, 0xfb, 0x33,
		0xff, 0xf9, 0x9a,
		0xff, 0xf8, 0x00,
		0x00, 0x07, 0xff,
		0xff, 0xf8, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xf0, 0x00,
		0x66, 0x60, 0x00,
		0x4c, 0xc0, 0x00,
		0x33, 0x30, 0x00,
		0x19, 0x90, 0x00,
		0xe6, 0x60, 0x00,
		0xcc, 0xd0, 0x00,
		0xb3, 0x30, 0x00,
		0x99, 0xa0, 0x00,
		0x80, 0x00, 0x00,
		0x7f, 0xf0, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x07, 0x00, 0x00,
		0x66, 0x06, 0x00, 0x00,
		0xcc, 0x04, 0x00, 0x00,
		0x33, 0x03, 0x00, 0x00,
		0x99, 0x01, 0x00, 0x00,
		0x66, 0xfe, 0xff, 0xff,
		0xcd, 0xfc, 0xff, 0xff,
		0x33, 0xfb, 0xff, 0xff,
		0x9a, 0xf9, 0xff, 0xff,
		0x00, 0xf8, 0xff, 0xff,
		0xff, 0x07, 0x00, 0x00,
		0x00, 0xf8, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0xf0, 0x7f,
		0x00, 0x00, 0x60, 0x66,
		0x00, 0x00, 0xc0, 0x4c,
		0x00, 0x00, 0x30, 0x33,
		0x00, 0x00, 0x90, 0x19,
		0x00, 0x00, 0x60, 0xe6,
		0x00, 0x00, 0xd0, 0xcc,
		0x00, 0x00, 0x30, 0xb3,
		0x00, 0x00, 0xa0, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0xf0, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x07, 0xff,
		0x00, 0x00, 0x06, 0x66,
		0x00, 0x00, 0x04, 0xcc,
		0x00, 0x00, 0x03, 0x33,
		0x00, 0x00, 0x01, 0x99,
		0xff, 0xff, 0xfe, 0x66,
		0xff, 0xff, 0xfc, 0xcd,
		0xff, 0xff, 0xfb, 0x33,
		0xff, 0xff, 0xf9, 0x9a,
		0xff, 0xff, 0xf8, 0x00,
		0x00, 0x00, 0x07, 0xff,
		0xff, 0xff, 0xf8, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo12BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 12;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xf0, 0x00, 0x00,
		0x66, 0x60, 0x00, 0x00,
		0x4c, 0xc0, 0x00, 0x00,
		0x33, 0x30, 0x00, 0x00,
		0x19, 0x90, 0x00, 0x00,
		0xe6, 0x60, 0x00, 0x00,
		0xcc, 0xd0, 0x00, 0x00,
		0xb3, 0x30, 0x00, 0x00,
		0x99, 0xa0, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xf0, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 13-Bits
//  1.0 0x00000fff 0x00007ff8
//  0.8 0x00000ccc 0x00006660
//  0.6 0x00000999 0x00004cc8
//  0.4 0x00000666 0x00003330
//  0.2 0x00000333 0x00001998
// -0.2 0xfffffccd 0xffffe668
// -0.4 0xfffff99a 0xffffccd0
// -0.6 0xfffff666 0xffffb330
// -0.8 0xfffff333 0xffff9998
// -1.0 0xfffff000 0xffff8000
//  1.0 0x00000fff 0x00007ff8
// -1.0 0xfffff000 0xffff8000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x0f,
		0xcc, 0x0c,
		0x99, 0x09,
		0x66, 0x06,
		0x33, 0x03,
		0xcd, 0xfc,
		0x9a, 0xf9,
		0x66, 0xf6,
		0x33, 0xf3,
		0x00, 0xf0,
		0xff, 0x0f,
		0x00, 0xf0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xf8, 0x7f,
		0x60, 0x66,
		0xc8, 0x4c,
		0x30, 0x33,
		0x98, 0x19,
		0x68, 0xe6,
		0xd0, 0xcc,
		0x30, 0xb3,
		0x98, 0x99,
		0x00, 0x80,
		0xf8, 0x7f,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x0f, 0xff,
		0x0c, 0xcc,
		0x09, 0x99,
		0x06, 0x66,
		0x03, 0x33,
		0xfc, 0xcd,
		0xf9, 0x9a,
		0xf6, 0x66,
		0xf3, 0x33,
		0xf0, 0x00,
		0x0f, 0xff,
		0xf0, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xf8,
		0x66, 0x60,
		0x4c, 0xc8,
		0x33, 0x30,
		0x19, 0x98,
		0xe6, 0x68,
		0xcc, 0xd0,
		0xb3, 0x30,
		0x99, 0x98,
		0x80, 0x00,
		0x7f, 0xf8,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x0f, 0x00,
		0xcc, 0x0c, 0x00,
		0x99, 0x09, 0x00,
		0x66, 0x06, 0x00,
		0x33, 0x03, 0x00,
		0xcd, 0xfc, 0xff,
		0x9a, 0xf9, 0xff,
		0x66, 0xf6, 0xff,
		0x33, 0xf3, 0xff,
		0x00, 0xf0, 0xff,
		0xff, 0x0f, 0x00,
		0x00, 0xf0, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xf8, 0x7f,
		0x00, 0x60, 0x66,
		0x00, 0xc8, 0x4c,
		0x00, 0x30, 0x33,
		0x00, 0x98, 0x19,
		0x00, 0x68, 0xe6,
		0x00, 0xd0, 0xcc,
		0x00, 0x30, 0xb3,
		0x00, 0x98, 0x99,
		0x00, 0x00, 0x80,
		0x00, 0xf8, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x0f, 0xff,
		0x00, 0x0c, 0xcc,
		0x00, 0x09, 0x99,
		0x00, 0x06, 0x66,
		0x00, 0x03, 0x33,
		0xff, 0xfc, 0xcd,
		0xff, 0xf9, 0x9a,
		0xff, 0xf6, 0x66,
		0xff, 0xf3, 0x33,
		0xff, 0xf0, 0x00,
		0x00, 0x0f, 0xff,
		0xff, 0xf0, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xf8, 0x00,
		0x66, 0x60, 0x00,
		0x4c, 0xc8, 0x00,
		0x33, 0x30, 0x00,
		0x19, 0x98, 0x00,
		0xe6, 0x68, 0x00,
		0xcc, 0xd0, 0x00,
		0xb3, 0x30, 0x00,
		0x99, 0x98, 0x00,
		0x80, 0x00, 0x00,
		0x7f, 0xf8, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x0f, 0x00, 0x00,
		0xcc, 0x0c, 0x00, 0x00,
		0x99, 0x09, 0x00, 0x00,
		0x66, 0x06, 0x00, 0x00,
		0x33, 0x03, 0x00, 0x00,
		0xcd, 0xfc, 0xff, 0xff,
		0x9a, 0xf9, 0xff, 0xff,
		0x66, 0xf6, 0xff, 0xff,
		0x33, 0xf3, 0xff, 0xff,
		0x00, 0xf0, 0xff, 0xff,
		0xff, 0x0f, 0x00, 0x00,
		0x00, 0xf0, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0xf8, 0x7f,
		0x00, 0x00, 0x60, 0x66,
		0x00, 0x00, 0xc8, 0x4c,
		0x00, 0x00, 0x30, 0x33,
		0x00, 0x00, 0x98, 0x19,
		0x00, 0x00, 0x68, 0xe6,
		0x00, 0x00, 0xd0, 0xcc,
		0x00, 0x00, 0x30, 0xb3,
		0x00, 0x00, 0x98, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0xf8, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x0f, 0xff,
		0x00, 0x00, 0x0c, 0xcc,
		0x00, 0x00, 0x09, 0x99,
		0x00, 0x00, 0x06, 0x66,
		0x00, 0x00, 0x03, 0x33,
		0xff, 0xff, 0xfc, 0xcd,
		0xff, 0xff, 0xf9, 0x9a,
		0xff, 0xff, 0xf6, 0x66,
		0xff, 0xff, 0xf3, 0x33,
		0xff, 0xff, 0xf0, 0x00,
		0x00, 0x00, 0x0f, 0xff,
		0xff, 0xff, 0xf0, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo13BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 13;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xf8, 0x00, 0x00,
		0x66, 0x60, 0x00, 0x00,
		0x4c, 0xc8, 0x00, 0x00,
		0x33, 0x30, 0x00, 0x00,
		0x19, 0x98, 0x00, 0x00,
		0xe6, 0x68, 0x00, 0x00,
		0xcc, 0xd0, 0x00, 0x00,
		0xb3, 0x30, 0x00, 0x00,
		0x99, 0x98, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xf8, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 14-Bits
//  1.0 0x00001fff 0x00007ffc
//  0.8 0x00001999 0x00006664
//  0.6 0x00001333 0x00004ccc
//  0.4 0x00000ccc 0x00003330
//  0.2 0x00000666 0x00001998
// -0.2 0xfffff99a 0xffffe668
// -0.4 0xfffff333 0xffffcccc
// -0.6 0xffffeccd 0xffffb334
// -0.8 0xffffe666 0xffff9998
// -1.0 0xffffe000 0xffff8000
//  1.0 0x00001fff 0x00007ffc
// -1.0 0xffffe000 0xffff8000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x1f,
		0x99, 0x19,
		0x33, 0x13,
		0xcc, 0x0c,
		0x66, 0x06,
		0x9a, 0xf9,
		0x33, 0xf3,
		0xcd, 0xec,
		0x66, 0xe6,
		0x00, 0xe0,
		0xff, 0x1f,
		0x00, 0xe0,
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xfc, 0x7f,
		0x64, 0x66,
		0xcc, 0x4c,
		0x30, 0x33,
		0x98, 0x19,
		0x68, 0xe6,
		0xcc, 0xcc,
		0x34, 0xb3,
		0x98, 0x99,
        0x00, 0x80,
		0xfc, 0x7f,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x1f, 0xff,
		0x19, 0x99,
		0x13, 0x33,
		0x0c, 0xcc,
		0x06, 0x66,
		0xf9, 0x9a,
		0xf3, 0x33,
		0xec, 0xcd,
		0xe6, 0x66,
		0xe0, 0x00,
		0x1f, 0xff,
		0xe0, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xfc,
		0x66, 0x64,
		0x4c, 0xcc,
		0x33, 0x30,
		0x19, 0x98,
		0xe6, 0x68,
		0xcc, 0xcc,
		0xb3, 0x34,
		0x99, 0x98,
		0x80, 0x00,
		0x7f, 0xfc,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x1f, 0x00,
		0x99, 0x19, 0x00,
		0x33, 0x13, 0x00,
		0xcc, 0x0c, 0x00,
		0x66, 0x06, 0x00,
		0x9a, 0xf9, 0xff,
		0x33, 0xf3, 0xff,
		0xcd, 0xec, 0xff,
		0x66, 0xe6, 0xff,
		0x00, 0xe0, 0xff,
		0xff, 0x1f, 0x00,
		0x00, 0xe0, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xfc, 0x7f,
		0x00, 0x64, 0x66,
		0x00, 0xcc, 0x4c,
		0x00, 0x30, 0x33,
		0x00, 0x98, 0x19,
		0x00, 0x68, 0xe6,
		0x00, 0xcc, 0xcc,
		0x00, 0x34, 0xb3,
		0x00, 0x98, 0x99,
        0x00, 0x00, 0x80,
		0x00, 0xfc, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x1f, 0xff,
		0x00, 0x19, 0x99,
		0x00, 0x13, 0x33,
		0x00, 0x0c, 0xcc,
		0x00, 0x06, 0x66,
		0xff, 0xf9, 0x9a,
		0xff, 0xf3, 0x33,
		0xff, 0xec, 0xcd,
		0xff, 0xe6, 0x66,
		0xff, 0xe0, 0x00,
		0x00, 0x1f, 0xff,
		0xff, 0xe0, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xfc, 0x00,
		0x66, 0x64, 0x00,
		0x4c, 0xcc, 0x00,
		0x33, 0x30, 0x00,
		0x19, 0x98, 0x00,
		0xe6, 0x68, 0x00,
		0xcc, 0xcc, 0x00,
		0xb3, 0x34, 0x00,
		0x99, 0x98, 0x00,
		0x80, 0x00, 0x00,
		0x7f, 0xfc, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x1f, 0x00, 0x00,
		0x99, 0x19, 0x00, 0x00,
		0x33, 0x13, 0x00, 0x00,
		0xcc, 0x0c, 0x00, 0x00,
		0x66, 0x06, 0x00, 0x00,
		0x9a, 0xf9, 0xff, 0xff,
		0x33, 0xf3, 0xff, 0xff,
		0xcd, 0xec, 0xff, 0xff,
		0x66, 0xe6, 0xff, 0xff,
		0x00, 0xe0, 0xff, 0xff,
		0xff, 0x1f, 0x00, 0x00,
		0x00, 0xe0, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0xfc, 0x7f,
		0x00, 0x00, 0x64, 0x66,
		0x00, 0x00, 0xcc, 0x4c,
		0x00, 0x00, 0x30, 0x33,
		0x00, 0x00, 0x98, 0x19,
		0x00, 0x00, 0x68, 0xe6,
		0x00, 0x00, 0xcc, 0xcc,
		0x00, 0x00, 0x34, 0xb3,
		0x00, 0x00, 0x98, 0x99,
        0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0xfc, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x1f, 0xff,
		0x00, 0x00, 0x19, 0x99,
		0x00, 0x00, 0x13, 0x33,
		0x00, 0x00, 0x0c, 0xcc,
		0x00, 0x00, 0x06, 0x66,
		0xff, 0xff, 0xf9, 0x9a,
		0xff, 0xff, 0xf3, 0x33,
		0xff, 0xff, 0xec, 0xcd,
		0xff, 0xff, 0xe6, 0x66,
		0xff, 0xff, 0xe0, 0x00,
		0x00, 0x00, 0x1f, 0xff,
		0xff, 0xff, 0xe0, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo14BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 14;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xfc, 0x00, 0x00,
		0x66, 0x64, 0x00, 0x00,
		0x4c, 0xcc, 0x00, 0x00,
		0x33, 0x30, 0x00, 0x00,
		0x19, 0x98, 0x00, 0x00,
		0xe6, 0x68, 0x00, 0x00,
		0xcc, 0xcc, 0x00, 0x00,
		0xb3, 0x34, 0x00, 0x00,
		0x99, 0x98, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xfc, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 15-Bits
//  1.0 0x00003fff 0x00007ffe
//  0.8 0x00003332 0x00006664
//  0.6 0x00002666 0x00004ccc
//  0.4 0x00001999 0x00003332
//  0.2 0x00000ccd 0x0000199a
// -0.2 0xfffff333 0xffffe666
// -0.4 0xffffe666 0xffffcccc
// -0.6 0xffffd99a 0xffffb334
// -0.8 0xffffcccd 0xffff999a
// -1.0 0xffffc000 0xffff8000
//  1.0 0x00003fff 0x00007ffe
// -1.0 0xffffc000 0xffff8000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInLittleEndian16BitsLSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x3f,
		0x32, 0x33,
		0x66, 0x26,
		0x99, 0x19,
		0xcd, 0x0c,
		0x33, 0xf3,
		0x66, 0xe6,
		0x9a, 0xd9,
		0xcd, 0xcc,
		0x00, 0xc0,
		0xff, 0x3f,
		0x00, 0xc0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInLittleEndian16BitsUnsignedLSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x7f,
		0x32, 0x73,
		0x66, 0x66,
		0x99, 0x59,
		0xcd, 0x4c,
		0x33, 0x33,
		0x66, 0x26,
		0x9a, 0x19,
		0xcd, 0x0c,
		0x00, 0x00,
		0xff, 0x7f,
		0x00, 0x00	
    };
	
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInLittleEndian16BitsMSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xfe, 0x7f,
		0x64, 0x66,
		0xcc, 0x4c,
		0x32, 0x33,
		0x9a, 0x19,
		0x66, 0xe6,
		0xcc, 0xcc,
		0x34, 0xb3,
		0x9a, 0x99,
		0x00, 0x80,
		0xfe, 0x7f,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInLittleEndian16BitsUnsignedMSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xfe, 0xff,
		0x64, 0xe6,
		0xcc, 0xcc,
		0x32, 0xb3,
		0x9a, 0x99,
		0x66, 0x66,
		0xcc, 0x4c,
		0x34, 0x33,
		0x9a, 0x19,
		0x00, 0x00,
		0xfe, 0xff,
		0x00, 0x00
	};
	
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInBigEndian16BitsLSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x3f, 0xff,
		0x33, 0x32,
		0x26, 0x66,
		0x19, 0x99,
		0x0c, 0xcd,
		0xf3, 0x33,
		0xe6, 0x66,
		0xd9, 0x9a,
		0xcc, 0xcd,
		0xc0, 0x00,
		0x3f, 0xff,
		0xc0, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInBigEndian16BitsUnsignedLSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff,
		0x73, 0x32,
		0x66, 0x66,
		0x59, 0x99,
		0x4c, 0xcd,
		0x33, 0x33,
		0x26, 0x66,
		0x19, 0x9a,
		0x0c, 0xcd,
		0x00, 0x00,
		0x7f, 0xff,
		0x00, 0x00
	};

	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInBigEndian16BitsMSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xfe,
		0x66, 0x64,
		0x4c, 0xcc,
		0x33, 0x32,
		0x19, 0x9a,
		0xe6, 0x66,
		0xcc, 0xcc,
		0xb3, 0x34,
		0x99, 0x9a,
		0x80, 0x00,
		0x7f, 0xfe,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInBigEndian16BitsUnsignedMSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xfe,
		0xe6, 0x64,
		0xcc, 0xcc,
		0xb3, 0x32,
		0x99, 0x9a,
		0x66, 0x66,
		0x4c, 0xcc,
		0x33, 0x34,
		0x19, 0x9a,
		0x00, 0x00,
		0xff, 0xfe,
		0x00, 0x00
	};
	
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x3f, 0x00,
		0x32, 0x33, 0x00,
		0x66, 0x26, 0x00,
		0x99, 0x19, 0x00,
		0xcd, 0x0c, 0x00,
		0x33, 0xf3, 0xff,
		0x66, 0xe6, 0xff,
		0x9a, 0xd9, 0xff,
		0xcd, 0xcc, 0xff,
		0x00, 0xc0, 0xff,
		0xff, 0x3f, 0x00,
		0x00, 0xc0, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xfe, 0x7f,
		0x00, 0x64, 0x66,
		0x00, 0xcc, 0x4c,
		0x00, 0x32, 0x33,
		0x00, 0x9a, 0x19,
		0x00, 0x66, 0xe6,
		0x00, 0xcc, 0xcc,
		0x00, 0x34, 0xb3,
		0x00, 0x9a, 0x99,
		0x00, 0x00, 0x80,
		0x00, 0xfe, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x3f, 0xff,
		0x00, 0x33, 0x32,
		0x00, 0x26, 0x66,
		0x00, 0x19, 0x99,
		0x00, 0x0c, 0xcd,
		0xff, 0xf3, 0x33,
		0xff, 0xe6, 0x66,
		0xff, 0xd9, 0x9a,
		0xff, 0xcc, 0xcd,
		0xff, 0xc0, 0x00,
		0x00, 0x3f, 0xff,
		0xff, 0xc0, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xfe, 0x00,
		0x66, 0x64, 0x00,
		0x4c, 0xcc, 0x00,
		0x33, 0x32, 0x00,
		0x19, 0x9a, 0x00,
		0xe6, 0x66, 0x00,
		0xcc, 0xcc, 0x00,
		0xb3, 0x34, 0x00,
		0x99, 0x9a, 0x00,
		0x80, 0x00, 0x00,
		0x7f, 0xfe, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x3f, 0x00, 0x00,
		0x32, 0x33, 0x00, 0x00,
		0x66, 0x26, 0x00, 0x00,
		0x99, 0x19, 0x00, 0x00,
		0xcd, 0x0c, 0x00, 0x00,
		0x33, 0xf3, 0xff, 0xff,
		0x66, 0xe6, 0xff, 0xff,
		0x9a, 0xd9, 0xff, 0xff,
		0xcd, 0xcc, 0xff, 0xff,
		0x00, 0xc0, 0xff, 0xff,
		0xff, 0x3f, 0x00, 0x00,
		0x00, 0xc0, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0xfe, 0x7f,
		0x00, 0x00, 0x64, 0x66,
		0x00, 0x00, 0xcc, 0x4c,
		0x00, 0x00, 0x32, 0x33,
		0x00, 0x00, 0x9a, 0x19,
		0x00, 0x00, 0x66, 0xe6,
		0x00, 0x00, 0xcc, 0xcc,
		0x00, 0x00, 0x34, 0xb3,
		0x00, 0x00, 0x9a, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0xfe, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x3f, 0xff,
		0x00, 0x00, 0x33, 0x32,
		0x00, 0x00, 0x26, 0x66,
		0x00, 0x00, 0x19, 0x99,
		0x00, 0x00, 0x0c, 0xcd,
		0xff, 0xff, 0xf3, 0x33,
		0xff, 0xff, 0xe6, 0x66,
		0xff, 0xff, 0xd9, 0x9a,
		0xff, 0xff, 0xcc, 0xcd,
		0xff, 0xff, 0xc0, 0x00,
		0x00, 0x00, 0x3f, 0xff,
		0xff, 0xff, 0xc0, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo15BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xfe, 0x00, 0x00,
		0x66, 0x64, 0x00, 0x00,
		0x4c, 0xcc, 0x00, 0x00,
		0x33, 0x32, 0x00, 0x00,
		0x19, 0x9a, 0x00, 0x00,
		0xe6, 0x66, 0x00, 0x00,
		0xcc, 0xcc, 0x00, 0x00,
		0xb3, 0x34, 0x00, 0x00,
		0x99, 0x9a, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xfe, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 16-Bits
//  1.0 0x00007fff 0x0007fff0
//  0.8 0x00006666 0x00066660
//  0.6 0x00004ccc 0x0004ccc0
//  0.4 0x00003333 0x00033330
//  0.2 0x00001999 0x00019990
// -0.2 0xffffe666 0xfffe6660
// -0.4 0xffffcccd 0xfffcccd0
// -0.6 0xffffb333 0xfffb3330
// -0.8 0xffff999a 0xfff999a0
// -1.0 0xffff8000 0xfff80000
//  1.0 0x00007fff 0x0007fff0
// -1.0 0xffff8000 0xfff80000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInLittleEndian16Bits)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x7f,
		0x66, 0x66,
		0xcc, 0x4c,
		0x33, 0x33,
		0x99, 0x19,
		0x66, 0xe6,
		0xcd, 0xcc,
		0x33, 0xb3,
		0x9a, 0x99,
		0x00, 0x80,
		0xff, 0x7f,
		0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInLittleEndianUnsigned16Bits)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff,
		0x66, 0xe6,
		0xcc, 0xcc,
		0x33, 0xb3,
		0x99, 0x99,
		0x66, 0x66,
		0xcd, 0x4c,
		0x33, 0x33,
		0x9a, 0x19,
		0x00, 0x00,
		0xff, 0xff,
		0x00, 0x00
	};
		
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInBigEndian16Bits)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff,
		0x66, 0x66,
		0x4c, 0xcc,
		0x33, 0x33,
		0x19, 0x99,
		0xe6, 0x66,
		0xcc, 0xcd,
		0xb3, 0x33,
		0x99, 0x9a,
		0x80, 0x00,
		0x7f, 0xff,
		0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInBigEndianUnsigned16Bits)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff,
		0xe6, 0x66,
		0xcc, 0xcc,
		0xb3, 0x33,
		0x99, 0x99,
		0x66, 0x66,
		0x4c, 0xcd,
		0x33, 0x33,
		0x19, 0x9a,
		0x00, 0x00,
		0xff, 0xff,
		0x00, 0x00
	};

	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x7f, 0x00,
		0x66, 0x66, 0x00,
		0xcc, 0x4c, 0x00,
		0x33, 0x33, 0x00,
		0x99, 0x19, 0x00,
		0x66, 0xe6, 0xff,
		0xcd, 0xcc, 0xff,
		0x33, 0xb3, 0xff,
		0x9a, 0x99, 0xff,
		0x00, 0x80, 0xff,
		0xff, 0x7f, 0x00,
		0x00, 0x80, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xff, 0x7f,
		0x00, 0x66, 0x66,
		0x00, 0xcc, 0x4c,
		0x00, 0x33, 0x33,
		0x00, 0x99, 0x19,
		0x00, 0x66, 0xe6,
		0x00, 0xcd, 0xcc,
		0x00, 0x33, 0xb3,
		0x00, 0x9a, 0x99,
		0x00, 0x00, 0x80,
		0x00, 0xff, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x7f, 0xff,
		0x00, 0x66, 0x66,
		0x00, 0x4c, 0xcc,
		0x00, 0x33, 0x33,
		0x00, 0x19, 0x99,
		0xff, 0xe6, 0x66,
		0xff, 0xcc, 0xcd,
		0xff, 0xb3, 0x33,
		0xff, 0x99, 0x9a,
		0xff, 0x80, 0x00,
		0x00, 0x7f, 0xff,
		0xff, 0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0x00,
		0x66, 0x66, 0x00,
		0x4c, 0xcc, 0x00,
		0x33, 0x33, 0x00,
		0x19, 0x99, 0x00,
		0xe6, 0x66, 0x00,
		0xcc, 0xcd, 0x00,
		0xb3, 0x33, 0x00,
		0x99, 0x9a, 0x00,
		0x80, 0x00, 0x00,
		0x7f, 0xff, 0x00,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0x7f, 0x00, 0x00,
		0x66, 0x66, 0x00, 0x00,
		0xcc, 0x4c, 0x00, 0x00,
		0x33, 0x33, 0x00, 0x00,
		0x99, 0x19, 0x00, 0x00,
		0x66, 0xe6, 0xff, 0xff,
		0xcd, 0xcc, 0xff, 0xff,
		0x33, 0xb3, 0xff, 0xff,
		0x9a, 0x99, 0xff, 0xff,
		0x00, 0x80, 0xff, 0xff,
		0xff, 0x7f, 0x00, 0x00,
		0x00, 0x80, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0xff, 0x7f,
		0x00, 0x00, 0x66, 0x66,
		0x00, 0x00, 0xcc, 0x4c,
		0x00, 0x00, 0x33, 0x33,
		0x00, 0x00, 0x99, 0x19,
		0x00, 0x00, 0x66, 0xe6,
		0x00, 0x00, 0xcd, 0xcc,
		0x00, 0x00, 0x33, 0xb3,
		0x00, 0x00, 0x9a, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0x7f, 0xff,
		0x00, 0x00, 0x66, 0x66,
		0x00, 0x00, 0x4c, 0xcc,
		0x00, 0x00, 0x33, 0x33,
		0x00, 0x00, 0x19, 0x99,
		0xff, 0xff, 0xe6, 0x66,
		0xff, 0xff, 0xcc, 0xcd,
		0xff, 0xff, 0xb3, 0x33,
		0xff, 0xff, 0x99, 0x9a,
		0xff, 0xff, 0x80, 0x00,
		0x00, 0x00, 0x7f, 0xff,
		0xff, 0xff, 0x80, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo16BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 16;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0x00, 0x00,
		0x66, 0x66, 0x00, 0x00,
		0x4c, 0xcc, 0x00, 0x00,
		0x33, 0x33, 0x00, 0x00,
		0x19, 0x99, 0x00, 0x00,
		0xe6, 0x66, 0x00, 0x00,
		0xcc, 0xcd, 0x00, 0x00,
		0xb3, 0x33, 0x00, 0x00,
		0x99, 0x9a, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 17-Bits
//  1.0 0x0000ffff 0x0007fff8
//  0.8 0x0000cccc 0x00066660
//  0.6 0x00009999 0x0004ccc8
//  0.4 0x00006666 0x00033330
//  0.2 0x00003333 0x00019998
// -0.2 0xffffcccd 0xfffe6668
// -0.4 0xffff999a 0xfffcccd0
// -0.6 0xffff6666 0xfffb3330
// -0.8 0xffff3333 0xfff99998
// -1.0 0xffff0000 0xfff80000
//  1.0 0x0000ffff 0x0007fff8
// -1.0 0xffff0000 0xfff80000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo17BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 17;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x00,
		0xcc, 0xcc, 0x00,
		0x99, 0x99, 0x00,
		0x66, 0x66, 0x00,
		0x33, 0x33, 0x00,
		0xcd, 0xcc, 0xff,
		0x9a, 0x99, 0xff,
		0x66, 0x66, 0xff,
		0x33, 0x33, 0xff,
		0x00, 0x00, 0xff,
		0xff, 0xff, 0x00,
		0x00, 0x00, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo17BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 17;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x80, 0xff, 0x7f,
		0x00, 0x66, 0x66,
		0x80, 0xcc, 0x4c,
		0x00, 0x33, 0x33,
		0x80, 0x99, 0x19,
		0x80, 0x66, 0xe6,
		0x00, 0xcd, 0xcc,
		0x00, 0x33, 0xb3,
		0x80, 0x99, 0x99,
		0x00, 0x00, 0x80,
		0x80, 0xff, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo17BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 17;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xff, 0xff,
		0x00, 0xcc, 0xcc,
		0x00, 0x99, 0x99,
		0x00, 0x66, 0x66,
		0x00, 0x33, 0x33,
		0xff, 0xcc, 0xcd,
		0xff, 0x99, 0x9a,
		0xff, 0x66, 0x66,
		0xff, 0x33, 0x33,
		0xff, 0x00, 0x00,
		0x00, 0xff, 0xff,
		0xff, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo17BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 17;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0x80,
		0x66, 0x66, 0x00,
		0x4c, 0xcc, 0x80,
		0x33, 0x33, 0x00,
		0x19, 0x99, 0x80,
		0xe6, 0x66, 0x80,
		0xcc, 0xcd, 0x00,
		0xb3, 0x33, 0x00,
		0x99, 0x99, 0x80,
		0x80, 0x00, 0x00,
		0x7f, 0xff, 0x80,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo17BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 17;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x00, 0x00,
		0xcc, 0xcc, 0x00, 0x00,
		0x99, 0x99, 0x00, 0x00,
		0x66, 0x66, 0x00, 0x00,
		0x33, 0x33, 0x00, 0x00,
		0xcd, 0xcc, 0xff, 0xff,
		0x9a, 0x99, 0xff, 0xff,
		0x66, 0x66, 0xff, 0xff,
		0x33, 0x33, 0xff, 0xff,
		0x00, 0x00, 0xff, 0xff,
		0xff, 0xff, 0x00, 0x00,
		0x00, 0x00, 0xff, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo17BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 17;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x80, 0xff, 0x7f,
		0x00, 0x00, 0x66, 0x66,
		0x00, 0x80, 0xcc, 0x4c,
		0x00, 0x00, 0x33, 0x33,
		0x00, 0x80, 0x99, 0x19,
		0x00, 0x80, 0x66, 0xe6,
		0x00, 0x00, 0xcd, 0xcc,
		0x00, 0x00, 0x33, 0xb3,
		0x00, 0x80, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0x80, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo17BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 17;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x00, 0xff, 0xff,
		0x00, 0x00, 0xcc, 0xcc,
		0x00, 0x00, 0x99, 0x99,
		0x00, 0x00, 0x66, 0x66,
		0x00, 0x00, 0x33, 0x33,
		0xff, 0xff, 0xcc, 0xcd,
		0xff, 0xff, 0x99, 0x9a,
		0xff, 0xff, 0x66, 0x66,
		0xff, 0xff, 0x33, 0x33,
		0xff, 0xff, 0x00, 0x00,
		0x00, 0x00, 0xff, 0xff,
		0xff, 0xff, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo17BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 17;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0x80, 0x00,
		0x66, 0x66, 0x00, 0x00,
		0x4c, 0xcc, 0x80, 0x00,
		0x33, 0x33, 0x00, 0x00,
		0x19, 0x99, 0x80, 0x00,
		0xe6, 0x66, 0x80, 0x00,
		0xcc, 0xcd, 0x00, 0x00,
		0xb3, 0x33, 0x00, 0x00,
		0x99, 0x99, 0x80, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0x80, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 18-Bits
//  1.0 0x0001ffff 0x0007fffc
//  0.8 0x00019999 0x00066664
//  0.6 0x00013333 0x0004cccc
//  0.4 0x0000cccc 0x00033330
//  0.2 0x00006666 0x00019998
// -0.2 0xffff999a 0xfffe6668
// -0.4 0xffff3333 0xfffccccc
// -0.6 0xfffecccd 0xfffb3334
// -0.8 0xfffe6666 0xfff99998
// -1.0 0xfffe0000 0xfff80000
//  1.0 0x0001ffff 0x0007fffc
// -1.0 0xfffe0000 0xfff80000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo18BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 18;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x01,
		0x99, 0x99, 0x01,
		0x33, 0x33, 0x01,
		0xcc, 0xcc, 0x00,
		0x66, 0x66, 0x00,
		0x9a, 0x99, 0xff,
		0x33, 0x33, 0xff,
		0xcd, 0xcc, 0xfe,
		0x66, 0x66, 0xfe,
		0x00, 0x00, 0xfe,
		0xff, 0xff, 0x01,
		0x00, 0x00, 0xfe
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo18BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 18;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xc0, 0xff, 0x7f,
		0x40, 0x66, 0x66,
		0xc0, 0xcc, 0x4c,
		0x00, 0x33, 0x33,
		0x80, 0x99, 0x19,
		0x80, 0x66, 0xe6,
		0xc0, 0xcc, 0xcc,
		0x40, 0x33, 0xb3,
		0x80, 0x99, 0x99,
		0x00, 0x00, 0x80,
		0xc0, 0xff, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo18BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 18;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x01, 0xff, 0xff,
		0x01, 0x99, 0x99,
		0x01, 0x33, 0x33,
		0x00, 0xcc, 0xcc,
		0x00, 0x66, 0x66,
		0xff, 0x99, 0x9a,
		0xff, 0x33, 0x33,
		0xfe, 0xcc, 0xcd,
		0xfe, 0x66, 0x66,
		0xfe, 0x00, 0x00,
		0x01, 0xff, 0xff,
		0xfe, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo18BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 18;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12] = {
        1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
    };
#else
    const sample_t c_sampleInput[12] = {
        1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
    };
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xc0,
		0x66, 0x66, 0x40,
		0x4c, 0xcc, 0xc0,
		0x33, 0x33, 0x00,
		0x19, 0x99, 0x80,
		0xe6, 0x66, 0x80,
		0xcc, 0xcc, 0xc0,
		0xb3, 0x33, 0x40,
		0x99, 0x99, 0x80,
		0x80, 0x00, 0x00,
		0x7f, 0xff, 0xc0,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo18BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 18;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x01, 0x00,
		0x99, 0x99, 0x01, 0x00,
		0x33, 0x33, 0x01, 0x00,
		0xcc, 0xcc, 0x00, 0x00,
		0x66, 0x66, 0x00, 0x00,
		0x9a, 0x99, 0xff, 0xff,
		0x33, 0x33, 0xff, 0xff,
		0xcd, 0xcc, 0xfe, 0xff,
		0x66, 0x66, 0xfe, 0xff,
		0x00, 0x00, 0xfe, 0xff,
		0xff, 0xff, 0x01, 0x00,
		0x00, 0x00, 0xfe, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo18BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 18;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xc0, 0xff, 0x7f,
		0x00, 0x40, 0x66, 0x66,
		0x00, 0xc0, 0xcc, 0x4c,
		0x00, 0x00, 0x33, 0x33,
		0x00, 0x80, 0x99, 0x19,
		0x00, 0x80, 0x66, 0xe6,
		0x00, 0xc0, 0xcc, 0xcc,
		0x00, 0x40, 0x33, 0xb3,
		0x00, 0x80, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0xc0, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo18BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 18;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x01, 0xff, 0xff,
		0x00, 0x01, 0x99, 0x99,
		0x00, 0x01, 0x33, 0x33,
		0x00, 0x00, 0xcc, 0xcc,
		0x00, 0x00, 0x66, 0x66,
		0xff, 0xff, 0x99, 0x9a,
		0xff, 0xff, 0x33, 0x33,
		0xff, 0xfe, 0xcc, 0xcd,
		0xff, 0xfe, 0x66, 0x66,
		0xff, 0xfe, 0x00, 0x00,
		0x00, 0x01, 0xff, 0xff,
		0xff, 0xfe, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo18BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 18;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xc0, 0x00,
		0x66, 0x66, 0x40, 0x00,
		0x4c, 0xcc, 0xc0, 0x00,
		0x33, 0x33, 0x00, 0x00,
		0x19, 0x99, 0x80, 0x00,
		0xe6, 0x66, 0x80, 0x00,
		0xcc, 0xcc, 0xc0, 0x00,
		0xb3, 0x33, 0x40, 0x00,
		0x99, 0x99, 0x80, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xc0, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 19-Bits
//  1.0 0x0003ffff 0x0007fffe
//  0.8 0x00033332 0x00066664
//  0.6 0x00026666 0x0004cccc
//  0.4 0x00019999 0x00033332
//  0.2 0x0000cccd 0x0001999a
// -0.2 0xffff3333 0xfffe6666
// -0.4 0xfffe6666 0xfffccccc
// -0.6 0xfffd999a 0xfffb3334
// -0.8 0xfffccccd 0xfff9999a
// -1.0 0xfffc0000 0xfff80000
//  1.0 0x0003ffff 0x0007fffe
// -1.0 0xfffc0000 0xfff80000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo19BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 19;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x03,
		0x32, 0x33, 0x03,
		0x66, 0x66, 0x02,
		0x99, 0x99, 0x01,
		0xcd, 0xcc, 0x00,
		0x33, 0x33, 0xff,
		0x66, 0x66, 0xfe,
		0x9a, 0x99, 0xfd,
		0xcd, 0xcc, 0xfc,
		0x00, 0x00, 0xfc,
		0xff, 0xff, 0x03,
		0x00, 0x00, 0xfc
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo19BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 19;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xe0, 0xff, 0x7f,
		0x40, 0x66, 0x66,
		0xc0, 0xcc, 0x4c,
		0x20, 0x33, 0x33,
		0xa0, 0x99, 0x19,
		0x60, 0x66, 0xe6,
		0xc0, 0xcc, 0xcc,
		0x40, 0x33, 0xb3,
		0xa0, 0x99, 0x99,
		0x00, 0x00, 0x80,
		0xe0, 0xff, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo19BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 19;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x03, 0xff, 0xff,
		0x03, 0x33, 0x32,
		0x02, 0x66, 0x66,
        0x01, 0x99, 0x99,
		0x00, 0xcc, 0xcd,
		0xff, 0x33, 0x33,
		0xfe, 0x66, 0x66,
		0xfd, 0x99, 0x9a,
		0xfc, 0xcc, 0xcd,
		0xfc, 0x00, 0x00,
		0x03, 0xff, 0xff,
		0xfc, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo19BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 19;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xe0,
		0x66, 0x66, 0x40,
		0x4c, 0xcc, 0xc0,
		0x33, 0x33, 0x20,
		0x19, 0x99, 0xa0,
		0xe6, 0x66, 0x60,
		0xcc, 0xcc, 0xc0,
		0xb3, 0x33, 0x40,
		0x99, 0x99, 0xa0,
		0x80, 0x00, 0x00,
		0x7f, 0xff, 0xe0,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo19BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 19;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x03, 0x00,
		0x32, 0x33, 0x03, 0x00,
		0x66, 0x66, 0x02, 0x00,
		0x99, 0x99, 0x01, 0x00,
		0xcd, 0xcc, 0x00, 0x00,
		0x33, 0x33, 0xff, 0xff,
		0x66, 0x66, 0xfe, 0xff,
		0x9a, 0x99, 0xfd, 0xff,
		0xcd, 0xcc, 0xfc, 0xff,
		0x00, 0x00, 0xfc, 0xff,
		0xff, 0xff, 0x03, 0x00,
		0x00, 0x00, 0xfc, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo19BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 19;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xe0, 0xff, 0x7f,
		0x00, 0x40, 0x66, 0x66,
		0x00, 0xc0, 0xcc, 0x4c,
		0x00, 0x20, 0x33, 0x33,
		0x00, 0xa0, 0x99, 0x19,
		0x00, 0x60, 0x66, 0xe6,
		0x00, 0xc0, 0xcc, 0xcc,
		0x00, 0x40, 0x33, 0xb3,
		0x00, 0xa0, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0xe0, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo19BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 19;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x03, 0xff, 0xff,
		0x00, 0x03, 0x33, 0x32,
		0x00, 0x02, 0x66, 0x66,
        0x00, 0x01, 0x99, 0x99,
		0x00, 0x00, 0xcc, 0xcd,
		0xff, 0xff, 0x33, 0x33,
		0xff, 0xfe, 0x66, 0x66,
		0xff, 0xfd, 0x99, 0x9a,
		0xff, 0xfc, 0xcc, 0xcd,
		0xff, 0xfc, 0x00, 0x00,
		0x00, 0x03, 0xff, 0xff,
		0xff, 0xfc, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo19BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 19;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xe0, 0x00,
		0x66, 0x66, 0x40, 0x00,
		0x4c, 0xcc, 0xc0, 0x00,
		0x33, 0x33, 0x20, 0x00,
		0x19, 0x99, 0xa0, 0x00,
		0xe6, 0x66, 0x60, 0x00,
		0xcc, 0xcc, 0xc0, 0x00,
		0xb3, 0x33, 0x40, 0x00,
		0x99, 0x99, 0xa0, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xe0, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 20-Bits
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo20BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 20;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x07,
		0x66, 0x66, 0x06,
		0xcc, 0xcc, 0x04,
		0x33, 0x33, 0x03,
		0x99, 0x99, 0x01,
		0x66, 0x66, 0xfe,
		0xcd, 0xcc, 0xfc,
		0x33, 0x33, 0xfb,
		0x9a, 0x99, 0xf9,
		0x00, 0x00, 0xf8,
		0xff, 0xff, 0x07,
		0x00, 0x00, 0xf8
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo20BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 20;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xf0, 0xff, 0x7f,
		0x60, 0x66, 0x66,
		0xc0, 0xcc, 0x4c,
		0x30, 0x33, 0x33,
		0x90, 0x99, 0x19,
		0x60, 0x66, 0xe6,
		0xd0, 0xcc, 0xcc,
		0x30, 0x33, 0xb3,
		0xa0, 0x99, 0x99,
		0x00, 0x00, 0x80,
		0xf0, 0xff, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo20BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 20;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x07, 0xFF, 0xFF,
		0x06, 0x66, 0x66,
		0x04, 0xCC, 0xCC,
		0x03, 0x33, 0x33,
		0x01, 0x99, 0x99,
		0xFE, 0x66, 0x66,
		0xFC, 0xCC, 0xCD,
		0xFB, 0x33, 0x33,
		0xF9, 0x99, 0x9A,
		0xF8, 0x00, 0x00,
		0x07, 0xFF, 0xFF,
		0xF8, 0x00, 0x00,
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo20BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 20;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7F, 0xFF, 0xF0,
		0x66, 0x66, 0x60,
		0x4C, 0xCC, 0xC0,
		0x33, 0x33, 0x30,
		0x19, 0x99, 0x90,
		0xE6, 0x66, 0x60,
		0xCC, 0xCC, 0xD0,
		0xB3, 0x33, 0x30,
		0x99, 0x99, 0xA0,
		0x80, 0x00, 0x00,
		0x7F, 0xFF, 0xF0,
		0x80, 0x00, 0x00,
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo20BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 20;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x07, 0x00,
		0x66, 0x66, 0x06, 0x00,
		0xcc, 0xcc, 0x04, 0x00,
		0x33, 0x33, 0x03, 0x00,
		0x99, 0x99, 0x01, 0x00,
		0x66, 0x66, 0xfe, 0xff,
		0xcd, 0xcc, 0xfc, 0xff,
		0x33, 0x33, 0xfb, 0xff,
		0x9a, 0x99, 0xf9, 0xff,
		0x00, 0x00, 0xf8, 0xff,
		0xff, 0xff, 0x07, 0x00,
		0x00, 0x00, 0xf8, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo20BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 20;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xf0, 0xff, 0x7f,
		0x00, 0x60, 0x66, 0x66,
		0x00, 0xc0, 0xcc, 0x4c,
		0x00, 0x30, 0x33, 0x33,
		0x00, 0x90, 0x99, 0x19,
		0x00, 0x60, 0x66, 0xe6,
		0x00, 0xd0, 0xcc, 0xcc,
		0x00, 0x30, 0x33, 0xb3,
		0x00, 0xa0, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0xf0, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo20BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 20;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x07, 0xFF, 0xFF,
		0x00, 0x06, 0x66, 0x66,
		0x00, 0x04, 0xCC, 0xCC,
		0x00, 0x03, 0x33, 0x33,
		0x00, 0x01, 0x99, 0x99,
		0xFF, 0xFE, 0x66, 0x66,
		0xFF, 0xFC, 0xCC, 0xCD,
		0xFF, 0xFB, 0x33, 0x33,
		0xFF, 0xF9, 0x99, 0x9A,
		0xFF, 0xF8, 0x00, 0x00,
		0x00, 0x07, 0xFF, 0xFF,
		0xFF, 0xF8, 0x00, 0x00,
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo20BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 20;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7F, 0xFF, 0xF0, 0x00,
		0x66, 0x66, 0x60, 0x00,
		0x4C, 0xCC, 0xC0, 0x00,
		0x33, 0x33, 0x30, 0x00,
		0x19, 0x99, 0x90, 0x00,
		0xE6, 0x66, 0x60, 0x00,
		0xCC, 0xCC, 0xD0, 0x00,
		0xB3, 0x33, 0x30, 0x00,
		0x99, 0x99, 0xA0, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7F, 0xFF, 0xF0, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 21-Bits
//  1.0 0x000fffff 0x007ffff8
//  0.8 0x000ccccc 0x00666660
//  0.6 0x00099999 0x004cccc8
//  0.4 0x00066666 0x00333330
//  0.2 0x00033333 0x00199998
// -0.2 0xfffccccd 0xffe66668
// -0.4 0xfff9999a 0xffccccd0
// -0.6 0xfff66666 0xffb33330
// -0.8 0xfff33333 0xff999998
// -1.0 0xfff00000 0xff800000
//  1.0 0x000fffff 0x007ffff8
// -1.0 0xfff00000 0xff800000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo21BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 21;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x0f,
		0xcc, 0xcc, 0x0c,
		0x99, 0x99, 0x09,
		0x66, 0x66, 0x06,
		0x33, 0x33, 0x03,
		0xcd, 0xcc, 0xfc,
		0x9a, 0x99, 0xf9,
		0x66, 0x66, 0xf6,
		0x33, 0x33, 0xf3,
		0x00, 0x00, 0xf0,
		0xff, 0xff, 0x0f,
		0x00, 0x00, 0xf0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo21BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 21;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xf8, 0xff, 0x7f,
		0x60, 0x66, 0x66,
		0xc8, 0xcc, 0x4c,
		0x30, 0x33, 0x33,
		0x98, 0x99, 0x19,
		0x68, 0x66, 0xe6,
		0xd0, 0xcc, 0xcc,
		0x30, 0x33, 0xb3,
		0x98, 0x99, 0x99,
		0x00, 0x00, 0x80,
		0xf8, 0xff, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo21BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 21;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x0f, 0xff, 0xff,
		0x0c, 0xcc, 0xcc,
		0x09, 0x99, 0x99,
		0x06, 0x66, 0x66,
		0x03, 0x33, 0x33,
		0xfc, 0xcc, 0xcd,
		0xf9, 0x99, 0x9a,
		0xf6, 0x66, 0x66,
		0xf3, 0x33, 0x33,
		0xf0, 0x00, 0x00,
		0x0f, 0xff, 0xff,
		0xf0, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo21BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 21;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xf8,
		0x66, 0x66, 0x60,
		0x4c, 0xcc, 0xc8,
		0x33, 0x33, 0x30,
		0x19, 0x99, 0x98,
		0xe6, 0x66, 0x68,
		0xcc, 0xcc, 0xd0,
		0xb3, 0x33, 0x30,
		0x99, 0x99, 0x98,
		0x80, 0x00, 0x00,
		0x7f, 0xff, 0xf8,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo21BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 21;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff,0xff,0x0f,0x00,
		0xcc,0xcc,0x0c,0x00,
		0x99,0x99,0x09,0x00,
		0x66,0x66,0x06,0x00,
		0x33,0x33,0x03,0x00,
		0xcd,0xcc,0xfc,0xff,
		0x9a,0x99,0xf9,0xff,
		0x66,0x66,0xf6,0xff,
		0x33,0x33,0xf3,0xff,
		0x00,0x00,0xf0,0xff,
		0xff,0xff,0x0f,0x00,
		0x00,0x00,0xf0,0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo21BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 21;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xf8, 0xff, 0x7f,
		0x00, 0x60, 0x66, 0x66,
		0x00, 0xc8, 0xcc, 0x4c,
		0x00, 0x30, 0x33, 0x33,
		0x00, 0x98, 0x99, 0x19,
		0x00, 0x68, 0x66, 0xe6,
		0x00, 0xd0, 0xcc, 0xcc,
		0x00, 0x30, 0x33, 0xb3,
		0x00, 0x98, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0xf8, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo21BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 21;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x0f, 0xff, 0xff,
		0x00, 0x0c, 0xcc, 0xcc,
		0x00, 0x09, 0x99, 0x99,
		0x00, 0x06, 0x66, 0x66,
		0x00, 0x03, 0x33, 0x33,
		0xff, 0xfc, 0xcc, 0xcd,
		0xff, 0xf9, 0x99, 0x9a,
		0xff, 0xf6, 0x66, 0x66,
		0xff, 0xf3, 0x33, 0x33,
		0xff, 0xf0, 0x00, 0x00,
		0x00, 0x0f, 0xff, 0xff,
		0xff, 0xf0, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo21BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 21;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xf8, 0x00,
		0x66, 0x66, 0x60, 0x00,
		0x4c, 0xcc, 0xc8, 0x00,
		0x33, 0x33, 0x30, 0x00,
		0x19, 0x99, 0x98, 0x00,
		0xe6, 0x66, 0x68, 0x00,
		0xcc, 0xcc, 0xd0, 0x00,
		0xb3, 0x33, 0x30, 0x00,
		0x99, 0x99, 0x98, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xf8, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 22-Bits
//  1.0 0x001fffff 0x007ffffc
//  0.8 0x00199999 0x00666664
//  0.6 0x00133333 0x004ccccc
//  0.4 0x000ccccc 0x00333330
//  0.2 0x00066666 0x00199998
// -0.2 0xfff9999a 0xffe66668
// -0.4 0xfff33333 0xffcccccc
// -0.6 0xffeccccd 0xffb33334
// -0.8 0xffe66666 0xff999998
// -1.0 0xffe00000 0xff800000
//  1.0 0x001fffff 0x007ffffc
// -1.0 0xffe00000 0xff800000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo22BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 22;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x1f,
		0x99, 0x99, 0x19,
		0x33, 0x33, 0x13,
		0xcc, 0xcc, 0x0c,
		0x66, 0x66, 0x06,
		0x9a, 0x99, 0xf9,
		0x33, 0x33, 0xf3,
		0xcd, 0xcc, 0xec,
		0x66, 0x66, 0xe6,
		0x00, 0x00, 0xe0,
		0xff, 0xff, 0x1f,
		0x00, 0x00, 0xe0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo22BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 22;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xfc, 0xff, 0x7f,
		0x64, 0x66, 0x66,
		0xcc, 0xcc, 0x4c,
		0x30, 0x33, 0x33,
		0x98, 0x99, 0x19,
		0x68, 0x66, 0xe6,
		0xcc, 0xcc, 0xcc,
		0x34, 0x33, 0xb3,
		0x98, 0x99, 0x99,
		0x00, 0x00, 0x80,
		0xfc, 0xff, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo22BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 22;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x1f, 0xff, 0xff,
		0x19, 0x99, 0x99,
		0x13, 0x33, 0x33,
		0x0c, 0xcc, 0xcc,
		0x06, 0x66, 0x66,
		0xf9, 0x99, 0x9a,
		0xf3, 0x33, 0x33,
		0xec, 0xcc, 0xcd,
		0xe6, 0x66, 0x66,
		0xe0, 0x00, 0x00,
		0x1f, 0xff, 0xff,
		0xe0, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo22BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 22;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xfc,
		0x66, 0x66, 0x64,
		0x4c, 0xcc, 0xcc,
		0x33, 0x33, 0x30,
		0x19, 0x99, 0x98,
		0xe6, 0x66, 0x68,
		0xcc, 0xcc, 0xcc,
		0xb3, 0x33, 0x34,
		0x99, 0x99, 0x98,
		0x80, 0x00, 0x00,
		0x7f, 0xff, 0xfc,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo22BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 22;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x1f, 0x00,
		0x99, 0x99, 0x19, 0x00,
		0x33, 0x33, 0x13, 0x00,
		0xcc, 0xcc, 0x0c, 0x00,
		0x66, 0x66, 0x06, 0x00,
		0x9a, 0x99, 0xf9, 0xff,
		0x33, 0x33, 0xf3, 0xff,
		0xcd, 0xcc, 0xec, 0xff,
		0x66, 0x66, 0xe6, 0xff,
		0x00, 0x00, 0xe0, 0xff,
		0xff, 0xff, 0x1f, 0x00,
		0x00, 0x00, 0xe0, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo22BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 22;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xfc, 0xff, 0x7f,
		0x00, 0x64, 0x66, 0x66,
		0x00, 0xcc, 0xcc, 0x4c,
		0x00, 0x30, 0x33, 0x33,
		0x00, 0x98, 0x99, 0x19,
		0x00, 0x68, 0x66, 0xe6,
		0x00, 0xcc, 0xcc, 0xcc,
		0x00, 0x34, 0x33, 0xb3,
		0x00, 0x98, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0xfc, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo22BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 22;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x1f, 0xff, 0xff,
		0x00, 0x19, 0x99, 0x99,
		0x00, 0x13, 0x33, 0x33,
		0x00, 0x0c, 0xcc, 0xcc,
		0x00, 0x06, 0x66, 0x66,
		0xff, 0xf9, 0x99, 0x9a,
		0xff, 0xf3, 0x33, 0x33,
		0xff, 0xec, 0xcc, 0xcd,
		0xff, 0xe6, 0x66, 0x66,
		0xff, 0xe0, 0x00, 0x00,
		0x00, 0x1f, 0xff, 0xff,
		0xff, 0xe0, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo22BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 22;
    const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xfc, 0x00,
		0x66, 0x66, 0x64, 0x00,
		0x4c, 0xcc, 0xcc, 0x00,
		0x33, 0x33, 0x30, 0x00,
		0x19, 0x99, 0x98, 0x00,
		0xe6, 0x66, 0x68, 0x00,
		0xcc, 0xcc, 0xcc, 0x00,
		0xb3, 0x33, 0x34, 0x00,
		0x99, 0x99, 0x98, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xfc, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 23-Bits
//  1.0 0x003fffff 0x007ffffe
//  0.8 0x00333332 0x00666664
//  0.6 0x00266666 0x004ccccc
//  0.4 0x00199999 0x00333332
//  0.2 0x000ccccd 0x0019999a
// -0.2 0xfff33333 0xffe66666
// -0.4 0xffe66666 0xffcccccc
// -0.6 0xffd9999a 0xffb33334
// -0.8 0xffcccccd 0xff99999a
// -1.0 0xffc00000 0xff800000
//  1.0 0x003fffff 0x007ffffe
// -1.0 0xffc00000 0xff800000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo23BitSampleInLittleEndian24BitsLSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x3f,
		0x32, 0x33, 0x33,
		0x66, 0x66, 0x26,
		0x99, 0x99, 0x19,
		0xcd, 0xcc, 0x0c,
		0x33, 0x33, 0xf3,
		0x66, 0x66, 0xe6,
		0x9a, 0x99, 0xd9,
		0xcd, 0xcc, 0xcc,
		0x00, 0x00, 0xc0,
		0xff, 0xff, 0x3f,
		0x00, 0x00, 0xc0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

TEST(SampleConverter,doubleSamplesTo23BitSampleInLittleEndianUnsigned24BitsLSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x7f,
		0x32, 0x33, 0x73,
		0x66, 0x66, 0x66,
		0x99, 0x99, 0x59,
		0xcd, 0xcc, 0x4c,
		0x33, 0x33, 0x33,
		0x66, 0x66, 0x26,
		0x9a, 0x99, 0x19,
		0xcd, 0xcc, 0x0c,
		0x00, 0x00, 0x00,
		0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00
	};
		
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo23BitSampleInLittleEndian24BitsMSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xfe, 0xff, 0x7f,
		0x64, 0x66, 0x66,
		0xcc, 0xcc, 0x4c,
		0x32, 0x33, 0x33,
		0x9a, 0x99, 0x19,
		0x66, 0x66, 0xe6,
		0xcc, 0xcc, 0xcc,
		0x34, 0x33, 0xb3,
		0x9a, 0x99, 0x99,
		0x00, 0x00, 0x80,
		0xfe, 0xff, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

TEST(SampleConverter,doubleSamplesTo23BitSampleInLittleEndianUnsigned24BitsMSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xfe, 0xff, 0xff,
		0x64, 0x66, 0xe6,
		0xcc, 0xcc, 0xcc,
		0x32, 0x33, 0xb3,
		0x9a, 0x99, 0x99,
		0x66, 0x66, 0x66,
		0xcc, 0xcc, 0x4c,
		0x34, 0x33, 0x33,
		0x9a, 0x99, 0x19,
		0x00, 0x00, 0x00,
		0xfe, 0xff, 0xff,
		0x00, 0x00, 0x00
	};
	
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo23BitSampleInBigEndian24BitsLSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x3f, 0xff, 0xff,
		0x33, 0x33, 0x32,
		0x26, 0x66, 0x66,
		0x19, 0x99, 0x99,
		0x0c, 0xcc, 0xcd,
		0xf3, 0x33, 0x33,
		0xe6, 0x66, 0x66,
		0xd9, 0x99, 0x9a,
		0xcc, 0xcc, 0xcd,
		0xc0, 0x00, 0x00,
		0x3f, 0xff, 0xff,
		0xc0, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

TEST(SampleConverter,doubleSamplesTo23BitSampleInBigEndianUnsigned24BitsLSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff,
		0x73, 0x33, 0x32,
		0x66, 0x66, 0x66,
		0x59, 0x99, 0x99,
		0x4c, 0xcc, 0xcd,
		0x33, 0x33, 0x33,
		0x26, 0x66, 0x66,
		0x19, 0x99, 0x9a,
		0x0c, 0xcc, 0xcd,
		0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff,
		0x00, 0x00, 0x00
	};

	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo23BitSampleInBigEndian24BitsMSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xfe,
		0x66, 0x66, 0x64,
		0x4c, 0xcc, 0xcc,
		0x33, 0x33, 0x32,
		0x19, 0x99, 0x9a,
		0xe6, 0x66, 0x66,
		0xcc, 0xcc, 0xcc,
		0xb3, 0x33, 0x34,
		0x99, 0x99, 0x9a,
		0x80, 0x00, 0x00,
		0x7f, 0xff, 0xfe,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

TEST(SampleConverter,doubleSamplesTo23BitSampleInBigEndianUnsigned24BitsMSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xfe,
		0xe6, 0x66, 0x64,
		0xcc, 0xcc, 0xcc,
		0xb3, 0x33, 0x32,
		0x99, 0x99, 0x9a,
		0x66, 0x66, 0x66,
		0x4c, 0xcc, 0xcc,
		0x33, 0x33, 0x34,
		0x19, 0x99, 0x9a,
		0x00, 0x00, 0x00,
		0xff, 0xff, 0xfe,
		0x00, 0x00, 0x00
	};

	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo23BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x3f, 0x00,
		0x32, 0x33, 0x33, 0x00,
		0x66, 0x66, 0x26, 0x00,
		0x99, 0x99, 0x19, 0x00,
		0xcd, 0xcc, 0x0c, 0x00,
		0x33, 0x33, 0xf3, 0xff,
		0x66, 0x66, 0xe6, 0xff,
		0x9a, 0x99, 0xd9, 0xff,
		0xcd, 0xcc, 0xcc, 0xff,
		0x00, 0x00, 0xc0, 0xff,
		0xff, 0xff, 0x3f, 0x00,
		0x00, 0x00, 0xc0, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo23BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xfe, 0xff, 0x7f,
		0x00,0x64,0x66,0x66,
		0x00,0xcc,0xcc,0x4c,
		0x00,0x32,0x33,0x33,
		0x00,0x9a,0x99,0x19,
		0x00,0x66,0x66,0xe6,
		0x00,0xcc,0xcc,0xcc,
		0x00,0x34,0x33,0xb3,
		0x00,0x9a,0x99,0x99,
		0x00,0x00,0x00,0x80,
		0x00,0xfe,0xff,0x7f,
		0x00,0x00,0x00,0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo23BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x3f, 0xff, 0xff,
		0x00, 0x33, 0x33, 0x32,
		0x00, 0x26, 0x66, 0x66,
		0x00, 0x19, 0x99, 0x99,
		0x00, 0x0c, 0xcc, 0xcd,
		0xff, 0xf3, 0x33, 0x33,
		0xff, 0xe6, 0x66, 0x66,
		0xff, 0xd9, 0x99, 0x9a,
		0xff, 0xcc, 0xcc, 0xcd,
		0xff, 0xc0, 0x00, 0x00,
		0x00, 0x3f, 0xff, 0xff,
		0xff, 0xc0, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo23BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xfe, 0x00,
		0x66, 0x66, 0x64, 0x00,
		0x4c, 0xcc, 0xcc, 0x00,
		0x33, 0x33, 0x32, 0x00,
		0x19, 0x99, 0x9a, 0x00,
		0xe6, 0x66, 0x66, 0x00,
		0xcc, 0xcc, 0xcc, 0x00,
		0xb3, 0x33, 0x34, 0x00,
		0x99, 0x99, 0x9a, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xfe, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 24-Bits
//  1.0 0x007fffff 0x07fffff0
//  0.8 0x00666666 0x06666660
//  0.6 0x004ccccc 0x04ccccc0
//  0.4 0x00333333 0x03333330
//  0.2 0x00199999 0x01999990
// -0.2 0xffe66666 0xfe666660
// -0.4 0xffcccccd 0xfcccccd0
// -0.6 0xffb33333 0xfb333330
// -0.8 0xff99999a 0xf99999a0
// -1.0 0xff800000 0xf8000000
//  1.0 0x007fffff 0x07fffff0
// -1.0 0xff800000 0xf8000000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo24BitSampleInLittleEndian24Bits)
{
	const tint c_noBits = 24;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x7f,
		0x66, 0x66, 0x66,
		0xcc, 0xcc, 0x4c,
		0x33, 0x33, 0x33,
		0x99, 0x99, 0x19,
		0x66, 0x66, 0xe6,
		0xcd, 0xcc, 0xcc,
		0x33, 0x33, 0xb3,
		0x9a, 0x99, 0x99,
		0x00, 0x00, 0x80,
		0xff, 0xff, 0x7f,
		0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo24BitSampleInLittleEndianUnsigned24Bits)
{
	const tint c_noBits = 24;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] ={
		0xff,0xff,0xff,
		0x66,0x66,0xe6,
		0xcc,0xcc,0xcc,
		0x33,0x33,0xb3,
		0x99,0x99,0x99,
		0x66,0x66,0x66,
		0xcd,0xcc,0x4c,
		0x33,0x33,0x33,
		0x9a,0x99,0x19,
		0x00,0x00,0x00,
		0xff,0xff,0xff,
		0x00,0x00,0x00
	};
	
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo24BitSampleInBigEndian24Bits)
{
	const tint c_noBits = 24;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff,
		0x66, 0x66, 0x66,
		0x4c, 0xcc, 0xcc,
		0x33, 0x33, 0x33,
		0x19, 0x99, 0x99,
		0xe6, 0x66, 0x66,
		0xcc, 0xcc, 0xcd,
		0xb3, 0x33, 0x33,
		0x99, 0x99, 0x9a,
		0x80, 0x00, 0x00,
		0x7f, 0xff, 0xff,
		0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo24BitSampleInBigEndianUnsigned24Bits)
{
	const tint c_noBits = 24;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] ={
		0xff,0xff,0xff,
		0xe6,0x66,0x66,
		0xcc,0xcc,0xcc,
		0xb3,0x33,0x33,
		0x99,0x99,0x99,
		0x66,0x66,0x66,
		0x4c,0xcc,0xcd,
		0x33,0x33,0x33,
		0x19,0x99,0x9a,
		0x00,0x00,0x00,
		0xff,0xff,0xff,
		0x00,0x00,0x00
	};

	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo24BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 24;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0x7f, 0x00,
		0x66, 0x66, 0x66, 0x00,
		0xcc, 0xcc, 0x4c, 0x00,
		0x33, 0x33, 0x33, 0x00,
		0x99, 0x99, 0x19, 0x00,
		0x66, 0x66, 0xe6, 0xff,
		0xcd, 0xcc, 0xcc, 0xff,
		0x33, 0x33, 0xb3, 0xff,
		0x9a, 0x99, 0x99, 0xff,
		0x00, 0x00, 0x80, 0xff,
		0xff, 0xff, 0x7f, 0x00,
		0x00, 0x00, 0x80, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo24BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 24;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xff, 0xff, 0x7f,
		0x00, 0x66, 0x66, 0x66,
		0x00, 0xcc, 0xcc, 0x4c,
		0x00, 0x33, 0x33, 0x33,
		0x00, 0x99, 0x99, 0x19,
		0x00, 0x66, 0x66, 0xe6,
		0x00, 0xcd, 0xcc, 0xcc,
		0x00, 0x33, 0x33, 0xb3,
		0x00, 0x9a, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x00, 0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo24BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 24;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0x7f, 0xff, 0xff,
		0x00, 0x66, 0x66, 0x66,
		0x00, 0x4c, 0xcc, 0xcc,
		0x00, 0x33, 0x33, 0x33,
		0x00, 0x19, 0x99, 0x99,
		0xff, 0xe6, 0x66, 0x66,
		0xff, 0xcc, 0xcc, 0xcd,
		0xff, 0xb3, 0x33, 0x33,
		0xff, 0x99, 0x99, 0x9a,
		0xff, 0x80, 0x00, 0x00,
		0x00, 0x7f, 0xff, 0xff,
		0xff, 0x80, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo24BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 24;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff, 0x00,
		0x66, 0x66, 0x66, 0x00,
		0x4c, 0xcc, 0xcc, 0x00,
		0x33, 0x33, 0x33, 0x00,
		0x19, 0x99, 0x99, 0x00,
		0xe6, 0x66, 0x66, 0x00,
		0xcc, 0xcc, 0xcd, 0x00,
		0xb3, 0x33, 0x33, 0x00,
		0x99, 0x99, 0x9a, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 25-Bits
//  1.0 0x00ffffff 0x07fffff8
//  0.8 0x00cccccc 0x06666660
//  0.6 0x00999999 0x04ccccc8
//  0.4 0x00666666 0x03333330
//  0.2 0x00333333 0x01999998
// -0.2 0xffcccccd 0xfe666668
// -0.4 0xff99999a 0xfcccccd0
// -0.6 0xff666666 0xfb333330
// -0.8 0xff333333 0xf9999998
// -1.0 0xff000000 0xf8000000
//  1.0 0x00ffffff 0x07fffff8
// -1.0 0xff000000 0xf8000000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo25BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 25;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0x00,
		0xcc, 0xcc, 0xcc, 0x00,
		0x99, 0x99, 0x99, 0x00,
		0x66, 0x66, 0x66, 0x00,
		0x33, 0x33, 0x33, 0x00,
		0xcd, 0xcc, 0xcc, 0xff,
		0x9a, 0x99, 0x99, 0xff,
		0x66, 0x66, 0x66, 0xff,
		0x33, 0x33, 0x33, 0xff,
		0x00, 0x00, 0x00, 0xff,
		0xff, 0xff, 0xff, 0x00,
		0x00, 0x00, 0x00, 0xff
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo25BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 25;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x80, 0xff, 0xff, 0x7f,
		0x00, 0x66, 0x66, 0x66,
		0x80, 0xcc, 0xcc, 0x4c,
		0x00, 0x33, 0x33, 0x33,
		0x80, 0x99, 0x99, 0x19,
		0x80, 0x66, 0x66, 0xe6,
		0x00, 0xcd, 0xcc, 0xcc,
		0x00, 0x33, 0x33, 0xb3,
		0x80, 0x99, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0x80, 0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo25BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 25;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x00, 0xff, 0xff, 0xff,
		0x00, 0xcc, 0xcc, 0xcc,
		0x00, 0x99, 0x99, 0x99,
		0x00, 0x66, 0x66, 0x66,
		0x00, 0x33, 0x33, 0x33,
		0xff, 0xcc, 0xcc, 0xcd,
		0xff, 0x99, 0x99, 0x9a,
		0xff, 0x66, 0x66, 0x66,
		0xff, 0x33, 0x33, 0x33,
		0xff, 0x00, 0x00, 0x00,
		0x00, 0xff, 0xff, 0xff,
		0xff, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo25BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 25;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff, 0x80,
		0x66, 0x66, 0x66, 0x00,
		0x4c, 0xcc, 0xcc, 0x80,
		0x33, 0x33, 0x33, 0x00,
		0x19, 0x99, 0x99, 0x80,
		0xe6, 0x66, 0x66, 0x80,
		0xcc, 0xcc, 0xcd, 0x00,
		0xb3, 0x33, 0x33, 0x00,
		0x99, 0x99, 0x99, 0x80,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0x80,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 26-Bits
//  1.0 0x01ffffff 0x07fffffc
//  0.8 0x01999999 0x06666664
//  0.6 0x01333333 0x04cccccc
//  0.4 0x00cccccc 0x03333330
//  0.2 0x00666666 0x01999998
// -0.2 0xff99999a 0xfe666668
// -0.4 0xff333333 0xfccccccc
// -0.6 0xfecccccd 0xfb333334
// -0.8 0xfe666666 0xf9999998
// -1.0 0xfe000000 0xf8000000
//  1.0 0x01ffffff 0x07fffffc
// -1.0 0xfe000000 0xf8000000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo26BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 26;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0x01,
		0x99, 0x99, 0x99, 0x01,
		0x33, 0x33, 0x33, 0x01,
		0xcc, 0xcc, 0xcc, 0x00,
		0x66, 0x66, 0x66, 0x00,
		0x9a, 0x99, 0x99, 0xff,
		0x33, 0x33, 0x33, 0xff,
		0xcd, 0xcc, 0xcc, 0xfe,
		0x66, 0x66, 0x66, 0xfe,
		0x00, 0x00, 0x00, 0xfe,
		0xff, 0xff, 0xff, 0x01,
		0x00, 0x00, 0x00, 0xfe
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo26BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 26;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xc0, 0xff, 0xff, 0x7f,
		0x40, 0x66, 0x66, 0x66,
		0xc0, 0xcc, 0xcc, 0x4c,
		0x00, 0x33, 0x33, 0x33,
		0x80, 0x99, 0x99, 0x19,
		0x80, 0x66, 0x66, 0xe6,
		0xc0, 0xcc, 0xcc, 0xcc,
		0x40, 0x33, 0x33, 0xb3,
		0x80, 0x99, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0xc0, 0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo26BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 26;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x01, 0xff, 0xff, 0xff,
		0x01, 0x99, 0x99, 0x99,
		0x01, 0x33, 0x33, 0x33,
		0x00, 0xcc, 0xcc, 0xcc,
		0x00, 0x66, 0x66, 0x66,
		0xff, 0x99, 0x99, 0x9a,
		0xff, 0x33, 0x33, 0x33,
		0xfe, 0xcc, 0xcc, 0xcd,
		0xfe, 0x66, 0x66, 0x66,
		0xfe, 0x00, 0x00, 0x00,
		0x01, 0xff, 0xff, 0xff,
		0xfe, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo26BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 26;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff, 0xc0,
		0x66, 0x66, 0x66, 0x40,
		0x4c, 0xcc, 0xcc, 0xc0,
		0x33, 0x33, 0x33, 0x00,
		0x19, 0x99, 0x99, 0x80,
		0xe6, 0x66, 0x66, 0x80,
		0xcc, 0xcc, 0xcc, 0xc0,
		0xb3, 0x33, 0x33, 0x40,
		0x99, 0x99, 0x99, 0x80,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xc0,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 27-Bits
//  1.0 0x03ffffff 0x07fffffe
//  0.8 0x03333332 0x06666664
//  0.6 0x02666666 0x04cccccc
//  0.4 0x01999999 0x03333332
//  0.2 0x00cccccd 0x0199999a
// -0.2 0xff333333 0xfe666666
// -0.4 0xfe666666 0xfccccccc
// -0.6 0xfd99999a 0xfb333334
// -0.8 0xfccccccd 0xf999999a
// -1.0 0xfc000000 0xf8000000
//  1.0 0x03ffffff 0x07fffffe
// -1.0 0xfc000000 0xf8000000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo27BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 27;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0x03,
		0x32, 0x33, 0x33, 0x03,
		0x66, 0x66, 0x66, 0x02,
		0x99, 0x99, 0x99, 0x01,
		0xcd, 0xcc, 0xcc, 0x00,
		0x33, 0x33, 0x33, 0xff,
		0x66, 0x66, 0x66, 0xfe,
		0x9a, 0x99, 0x99, 0xfd,
		0xcd, 0xcc, 0xcc, 0xfc,
		0x00, 0x00, 0x00, 0xfc,
		0xff, 0xff, 0xff, 0x03,
		0x00, 0x00, 0x00, 0xfc
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo27BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 27;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xe0, 0xff, 0xff, 0x7f,
		0x40, 0x66, 0x66, 0x66,
		0xc0, 0xcc, 0xcc, 0x4c,
		0x20, 0x33, 0x33, 0x33,
		0xa0, 0x99, 0x99, 0x19,
		0x60, 0x66, 0x66, 0xe6,
		0xc0, 0xcc, 0xcc, 0xcc,
		0x40, 0x33, 0x33, 0xb3,
		0xa0, 0x99, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0xe0, 0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo27BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 27;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x03, 0xff, 0xff, 0xff,
		0x03, 0x33, 0x33, 0x32,
		0x02, 0x66, 0x66, 0x66,
		0x01, 0x99, 0x99, 0x99,
		0x00, 0xcc, 0xcc, 0xcd,
		0xff, 0x33, 0x33, 0x33,
		0xfe, 0x66, 0x66, 0x66,
		0xfd, 0x99, 0x99, 0x9a,
		0xfc, 0xcc, 0xcc, 0xcd,
		0xfc, 0x00, 0x00, 0x00,
		0x03, 0xff, 0xff, 0xff,
		0xfc, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo27BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 27;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff, 0xe0,
		0x66, 0x66, 0x66, 0x40,
		0x4c, 0xcc, 0xcc, 0xc0,
		0x33, 0x33, 0x33, 0x20,
		0x19, 0x99, 0x99, 0xa0,
		0xe6, 0x66, 0x66, 0x60,
		0xcc, 0xcc, 0xcc, 0xc0,
		0xb3, 0x33, 0x33, 0x40,
		0x99, 0x99, 0x99, 0xa0,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xe0,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 28-Bits
//  1.0 0x07ffffff 0x7ffffff0
//  0.8 0x06666666 0x66666660
//  0.6 0x04cccccc 0x4cccccc0
//  0.4 0x03333333 0x33333330
//  0.2 0x01999999 0x19999990
// -0.2 0xfe666666 0xe6666660
// -0.4 0xfccccccd 0xccccccd0
// -0.6 0xfb333333 0xb3333330
// -0.8 0xf999999a 0x999999a0
// -1.0 0xf8000000 0x80000000
//  1.0 0x07ffffff 0x7ffffff0
// -1.0 0xf8000000 0x80000000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo28BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 28;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0x07,
		0x66, 0x66, 0x66, 0x06,
		0xcc, 0xcc, 0xcc, 0x04,
		0x33, 0x33, 0x33, 0x03,
		0x99, 0x99, 0x99, 0x01,
		0x66, 0x66, 0x66, 0xfe,
		0xcd, 0xcc, 0xcc, 0xfc,
		0x33, 0x33, 0x33, 0xfb,
		0x9a, 0x99, 0x99, 0xf9,
		0x00, 0x00, 0x00, 0xf8,
		0xff, 0xff, 0xff, 0x07,
		0x00, 0x00, 0x00, 0xf8
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo28BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 28;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xf0, 0xff, 0xff, 0x7f,
		0x60, 0x66, 0x66, 0x66,
		0xc0, 0xcc, 0xcc, 0x4c,
		0x30, 0x33, 0x33, 0x33,
		0x90, 0x99, 0x99, 0x19,
		0x60, 0x66, 0x66, 0xe6,
		0xd0, 0xcc, 0xcc, 0xcc,
		0x30, 0x33, 0x33, 0xb3,
		0xa0, 0x99, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0xf0, 0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo28BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 28;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x07, 0xff, 0xff, 0xff,
		0x06, 0x66, 0x66, 0x66,
		0x04, 0xcc, 0xcc, 0xcc,
		0x03, 0x33, 0x33, 0x33,
		0x01, 0x99, 0x99, 0x99,
		0xfe, 0x66, 0x66, 0x66,
		0xfc, 0xcc, 0xcc, 0xcd,
		0xfb, 0x33, 0x33, 0x33,
		0xf9, 0x99, 0x99, 0x9a,
		0xf8, 0x00, 0x00, 0x00,
		0x07, 0xff, 0xff, 0xff,
		0xf8, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo28BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 28;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff, 0xf0,
		0x66, 0x66, 0x66, 0x60,
		0x4c, 0xcc, 0xcc, 0xc0,
		0x33, 0x33, 0x33, 0x30,
		0x19, 0x99, 0x99, 0x90,
		0xe6, 0x66, 0x66, 0x60,
		0xcc, 0xcc, 0xcc, 0xd0,
		0xb3, 0x33, 0x33, 0x30,
		0x99, 0x99, 0x99, 0xa0,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xf0,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 29-Bits
//  1.0 0x0fffffff 0x7ffffff8
//  0.8 0x0ccccccc 0x66666660
//  0.6 0x09999999 0x4cccccc8
//  0.4 0x06666666 0x33333330
//  0.2 0x03333333 0x19999998
// -0.2 0xfccccccd 0xe6666668
// -0.4 0xf999999a 0xccccccd0
// -0.6 0xf6666666 0xb3333330
// -0.8 0xf3333333 0x99999998
// -1.0 0xf0000000 0x80000000
//  1.0 0x0fffffff 0x7ffffff8
// -1.0 0xf0000000 0x80000000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo29BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 29;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0x0f,
		0xcc, 0xcc, 0xcc, 0x0c,
		0x99, 0x99, 0x99, 0x09,
		0x66, 0x66, 0x66, 0x06,
		0x33, 0x33, 0x33, 0x03,
		0xcd, 0xcc, 0xcc, 0xfc,
		0x9a, 0x99, 0x99, 0xf9,
		0x66, 0x66, 0x66, 0xf6,
		0x33, 0x33, 0x33, 0xf3,
		0x00, 0x00, 0x00, 0xf0,
		0xff, 0xff, 0xff, 0x0f,
		0x00, 0x00, 0x00, 0xf0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo29BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 29;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xf8, 0xff, 0xff, 0x7f,
		0x60, 0x66, 0x66, 0x66,
		0xc8, 0xcc, 0xcc, 0x4c,
		0x30, 0x33, 0x33, 0x33,
		0x98, 0x99, 0x99, 0x19,
		0x68, 0x66, 0x66, 0xe6,
		0xd0, 0xcc, 0xcc, 0xcc,
		0x30, 0x33, 0x33, 0xb3,
		0x98, 0x99, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0xf8, 0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo29BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 29;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x0f, 0xff, 0xff, 0xff,
		0x0c, 0xcc, 0xcc, 0xcc,
		0x09, 0x99, 0x99, 0x99,
		0x06, 0x66, 0x66, 0x66,
		0x03, 0x33, 0x33, 0x33,
		0xfc, 0xcc, 0xcc, 0xcd,
		0xf9, 0x99, 0x99, 0x9a,
		0xf6, 0x66, 0x66, 0x66,
		0xf3, 0x33, 0x33, 0x33,
		0xf0, 0x00, 0x00, 0x00,
		0x0f, 0xff, 0xff, 0xff,
		0xf0, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo29BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 29;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff, 0xf8,
		0x66, 0x66, 0x66, 0x60,
		0x4c, 0xcc, 0xcc, 0xc8,
		0x33, 0x33, 0x33, 0x30,
		0x19, 0x99, 0x99, 0x98,
		0xe6, 0x66, 0x66, 0x68,
		0xcc, 0xcc, 0xcc, 0xd0,
		0xb3, 0x33, 0x33, 0x30,
		0x99, 0x99, 0x99, 0x98,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xf8,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 30-Bits
//  1.0 0x1fffffff 0x7ffffffc
//  0.8 0x19999999 0x66666664
//  0.6 0x13333333 0x4ccccccc
//  0.4 0x0ccccccc 0x33333330
//  0.2 0x06666666 0x19999998
// -0.2 0xf999999a 0xe6666668
// -0.4 0xf3333333 0xcccccccc
// -0.6 0xeccccccd 0xb3333334
// -0.8 0xe6666666 0x99999998
// -1.0 0xe0000000 0x80000000
//  1.0 0x1fffffff 0x7ffffffc
// -1.0 0xe0000000 0x80000000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo30BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 30;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0x1f,
		0x99, 0x99, 0x99, 0x19,
		0x33, 0x33, 0x33, 0x13,
		0xcc, 0xcc, 0xcc, 0x0c,
		0x66, 0x66, 0x66, 0x06,
		0x9a, 0x99, 0x99, 0xf9,
		0x33, 0x33, 0x33, 0xf3,
		0xcd, 0xcc, 0xcc, 0xec,
		0x66, 0x66, 0x66, 0xe6,
		0x00, 0x00, 0x00, 0xe0,
		0xff, 0xff, 0xff, 0x1f,
		0x00, 0x00, 0x00, 0xe0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo30BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 30;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xfc, 0xff, 0xff, 0x7f,
		0x64, 0x66, 0x66, 0x66,
		0xcc, 0xcc, 0xcc, 0x4c,
		0x30, 0x33, 0x33, 0x33,
		0x98, 0x99, 0x99, 0x19,
		0x68, 0x66, 0x66, 0xe6,
		0xcc, 0xcc, 0xcc, 0xcc,
		0x34, 0x33, 0x33, 0xb3,
		0x98, 0x99, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0xfc, 0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo30BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 30;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x1f, 0xff, 0xff, 0xff,
		0x19, 0x99, 0x99, 0x99,
		0x13, 0x33, 0x33, 0x33,
		0x0c, 0xcc, 0xcc, 0xcc,
		0x06, 0x66, 0x66, 0x66,
		0xf9, 0x99, 0x99, 0x9a,
		0xf3, 0x33, 0x33, 0x33,
		0xec, 0xcc, 0xcc, 0xcd,
		0xe6, 0x66, 0x66, 0x66,
		0xe0, 0x00, 0x00, 0x00,
		0x1f, 0xff, 0xff, 0xff,
		0xe0, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo30BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 30;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff, 0xfc,
		0x66, 0x66, 0x66, 0x64,
		0x4c, 0xcc, 0xcc, 0xcc,
		0x33, 0x33, 0x33, 0x30,
		0x19, 0x99, 0x99, 0x98,
		0xe6, 0x66, 0x66, 0x68,
		0xcc, 0xcc, 0xcc, 0xcc,
		0xb3, 0x33, 0x33, 0x34,
		0x99, 0x99, 0x99, 0x98,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xfc,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 31-Bits
//  1.0 0x3fffffff 0x7ffffffe
//  0.8 0x33333332 0x66666664
//  0.6 0x26666666 0x4ccccccc
//  0.4 0x19999999 0x33333332
//  0.2 0x0ccccccd 0x1999999a
// -0.2 0xf3333333 0xe6666666
// -0.4 0xe6666666 0xcccccccc
// -0.6 0xd999999a 0xb3333334
// -0.8 0xcccccccd 0x9999999a
// -1.0 0xc0000000 0x80000000
//  1.0 0x3fffffff 0x7ffffffe
// -1.0 0xc0000000 0x80000000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo31BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0x3f,
		0x32, 0x33, 0x33, 0x33,
		0x66, 0x66, 0x66, 0x26,
		0x99, 0x99, 0x99, 0x19,
		0xcd, 0xcc, 0xcc, 0x0c,
		0x33, 0x33, 0x33, 0xf3,
		0x66, 0x66, 0x66, 0xe6,
		0x9a, 0x99, 0x99, 0xd9,
		0xcd, 0xcc, 0xcc, 0xcc,
		0x00, 0x00, 0x00, 0xc0,
		0xff, 0xff, 0xff, 0x3f,
		0x00, 0x00, 0x00, 0xc0
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo31BitSampleInLittleEndianUnsigned32BitsLSB)
{
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0x7f,
		0x32, 0x33, 0x33, 0x73,
		0x66, 0x66, 0x66, 0x66,
		0x99, 0x99, 0x99, 0x59,
		0xcd, 0xcc, 0xcc, 0x4c,
		0x33, 0x33, 0x33, 0x33,
		0x66, 0x66, 0x66, 0x26,
		0x9a, 0x99, 0x99, 0x19,
		0xcd, 0xcc, 0xcc, 0x0c,
		0x00, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x00
	};
	
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo31BitSampleInLittleEndian32BitsMSB)
{
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xfe, 0xff, 0xff, 0x7f,
		0x64, 0x66, 0x66, 0x66,
		0xcc, 0xcc, 0xcc, 0x4c,
		0x32, 0x33, 0x33, 0x33,
		0x9a, 0x99, 0x99, 0x19,
		0x66, 0x66, 0x66, 0xe6,
		0xcc, 0xcc, 0xcc, 0xcc,
		0x34, 0x33, 0x33, 0xb3,
		0x9a, 0x99, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0xfe, 0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo31BitSampleInLittleEndianUnsigned32BitsMSB)
{
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xfe, 0xff, 0xff, 0xff,
		0x64, 0x66, 0x66, 0xe6,
		0xcc, 0xcc, 0xcc, 0xcc,
		0x32, 0x33, 0x33, 0xb3,
		0x9a, 0x99, 0x99, 0x99,
		0x66, 0x66, 0x66, 0x66,
		0xcc, 0xcc, 0xcc, 0x4c,
		0x34, 0x33, 0x33, 0x33,
		0x9a, 0x99, 0x99, 0x19,
		0x00, 0x00, 0x00, 0x00,
		0xfe, 0xff, 0xff, 0xff,
		0x00, 0x00, 0x00, 0x00
	};

	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo31BitSampleInBigEndian32BitsLSB)
{
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x3f, 0xff, 0xff, 0xff,
		0x33, 0x33, 0x33, 0x32,
		0x26, 0x66, 0x66, 0x66,
		0x19, 0x99, 0x99, 0x99,
		0x0c, 0xcc, 0xcc, 0xcd,
		0xf3, 0x33, 0x33, 0x33,
		0xe6, 0x66, 0x66, 0x66,
		0xd9, 0x99, 0x99, 0x9a,
		0xcc, 0xcc, 0xcc, 0xcd,
		0xc0, 0x00, 0x00, 0x00,
		0x3f, 0xff, 0xff, 0xff,
		0xc0, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo31BitSampleInBigEndianUnsigned32BitsLSB)
{
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] ={
		0x7f,0xff,0xff,0xff,
		0x73,0x33,0x33,0x32,
		0x66,0x66,0x66,0x66,
		0x59,0x99,0x99,0x99,
		0x4c,0xcc,0xcc,0xcd,
		0x33,0x33,0x33,0x33,
		0x26,0x66,0x66,0x66,
		0x19,0x99,0x99,0x9a,
		0x0c,0xcc,0xcc,0xcd,
		0x00,0x00,0x00,0x00,
		0x7f,0xff,0xff,0xff,
		0x00,0x00,0x00,0x00
	};

	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo31BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif

	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff, 0xfe,
		0x66, 0x66, 0x66, 0x64,
		0x4c, 0xcc, 0xcc, 0xcc,
		0x33, 0x33, 0x33, 0x32,
		0x19, 0x99, 0x99, 0x9a,
		0xe6, 0x66, 0x66, 0x66,
		0xcc, 0xcc, 0xcc, 0xcc,
		0xb3, 0x33, 0x33, 0x34,
		0x99, 0x99, 0x99, 0x9a,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xfe,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo31BitSampleInBigEndianUnsigned32BitsMSB)
{
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0xfe,
		0xe6, 0x66, 0x66, 0x64,
		0xcc, 0xcc, 0xcc, 0xcc,
		0xb3, 0x33, 0x33, 0x32,
		0x99, 0x99, 0x99, 0x9a,
		0x66, 0x66, 0x66, 0x66,
		0x4c, 0xcc, 0xcc, 0xcc,
		0x33, 0x33, 0x33, 0x34,
		0x19, 0x99, 0x99, 0x9a,
		0x00, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0xfe,
		0x00, 0x00, 0x00, 0x00
	};
		
	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// 32-Bits
//  1.0 0x7fffffff 0xfffffff0
//  0.8 0x66666666 0x66666660
//  0.6 0x4ccccccc 0xccccccc0
//  0.4 0x33333333 0x33333330
//  0.2 0x19999999 0x99999990
// -0.2 0xe6666666 0x66666660
// -0.4 0xcccccccd 0xccccccd0
// -0.6 0xb3333333 0x33333330
// -0.8 0x9999999a 0x999999a0
// -1.0 0x80000000 0x00000000
//  1.0 0x7fffffff 0xfffffff0
// -1.0 0x80000000 0x00000000
//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo32BitSampleInLittleEndian32BitsLSB)
{
	const tint c_noBits = 32;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0x7f,
		0x66, 0x66, 0x66, 0x66,
		0xcc, 0xcc, 0xcc, 0x4c,
		0x33, 0x33, 0x33, 0x33,
		0x99, 0x99, 0x99, 0x19,
		0x66, 0x66, 0x66, 0xe6,
		0xcd, 0xcc, 0xcc, 0xcc,
		0x33, 0x33, 0x33, 0xb3,
		0x9a, 0x99, 0x99, 0x99,
		0x00, 0x00, 0x00, 0x80,
		0xff, 0xff, 0xff, 0x7f,
		0x00, 0x00, 0x00, 0x80
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo32BitSampleInLittleEndianUnsigned32BitsLSB)
{
	const tint c_noBits = 32;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0xff,
		0x66, 0x66, 0x66, 0xe6,
		0xcc, 0xcc, 0xcc, 0xcc,
		0x33, 0x33, 0x33, 0xb3,
		0x99, 0x99, 0x99, 0x99,
		0x66, 0x66, 0x66, 0x66,
		0xcd, 0xcc, 0xcc, 0x4c,
		0x33, 0x33, 0x33, 0x33,
		0x9a, 0x99, 0x99, 0x19,
		0x00, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0xff,
		0x00, 0x00, 0x00, 0x00
	};

	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo32BitSampleInBigEndian32BitsMSB)
{
	const tint c_noBits = 32;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0x7f, 0xff, 0xff, 0xff,
		0x66, 0x66, 0x66, 0x66,
		0x4c, 0xcc, 0xcc, 0xcc,
		0x33, 0x33, 0x33, 0x33,
		0x19, 0x99, 0x99, 0x99,
		0xe6, 0x66, 0x66, 0x66,
		0xcc, 0xcc, 0xcc, 0xcd,
		0xb3, 0x33, 0x33, 0x33,
		0x99, 0x99, 0x99, 0x9a,
		0x80, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xff,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesTo32BitSampleInBigEndianUnsigned32BitsMSB)
{
	const tint c_noBits = 32;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		1.0, 0.8, 0.6, 0.4, 0.2, -0.2, -0.4, -0.6, -0.8, -1.0, 1.2, -1.2
	};
#endif
	
	const tubyte c_expectOutput[12 * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0xff,
		0xe6, 0x66, 0x66, 0x66,
		0xcc, 0xcc, 0xcc, 0xcc,
		0xb3, 0x33, 0x33, 0x33,
		0x99, 0x99, 0x99, 0x99,
		0x66, 0x66, 0x66, 0x66,
		0x4c, 0xcc, 0xcc, 0xcd,
		0x33, 0x33, 0x33, 0x33,
		0x19, 0x99, 0x99, 0x9a,
		0x00, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0xff,
		0x00, 0x00, 0x00, 0x00
	};

	testSampleUnsignedConvertion(c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
// Channel increment tests
//-------------------------------------------------------------------------------------------

void testSampleConvertionForChannelInterleave(tint noInChannels,tint noOutChannels,tint noBits,tint bytesPerSample,bool littleEndian,bool alignHigh,const sample_t *samples,const tubyte *expect)
{
    tubyte *out = new tubyte [12 * noOutChannels * bytesPerSample];
	memset(out,0,12 * noOutChannels * bytesPerSample);
	
    SampleConverter sampleConverter(noBits,bytesPerSample,littleEndian,alignHigh,true);
	sampleConverter.setNumberOfInputChannels(noInChannels);
	sampleConverter.setNumberOfOutputChannels(noOutChannels);
	
	EXPECT_TRUE(sampleConverter.isSupported());
	
    EXPECT_EQ(noInChannels,sampleConverter.numberOfInputChannels());
    EXPECT_EQ(noOutChannels,sampleConverter.numberOfOutputChannels());
	EXPECT_EQ(littleEndian,sampleConverter.isLittleEndian());
	EXPECT_EQ(alignHigh,sampleConverter.isAlignedHigh());
	EXPECT_EQ(noBits,sampleConverter.bits());
	EXPECT_EQ(bytesPerSample,sampleConverter.bytesPerSample());
	
	sampleConverter.convert(samples,out,12);
	
	testSampleOutputConvertion(expect,out,noBits,bytesPerSample,12 * noOutChannels);
	
	delete [] out;
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamples8BitsLSBWith2Channels)
{
	const tint c_noInChannels = 2;
	const tint c_noOutChannels = 2;
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 
		 0.8f, 0.7f,
		 0.6f, 0.7f,
		 0.4f, 0.7f,
		 0.2f, 0.7f,
		-0.2f, 0.7f,
		-0.4f, 0.7f,
		-0.6f, 0.7f,
		-0.8f, 0.7f,
		-1.0f, 0.7f,
		 1.2f, 0.7f,
		-1.2f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 
		 0.8, 0.7,
		 0.6, 0.7,
		 0.4, 0.7,
		 0.2, 0.7,
		-0.2, 0.7,
		-0.4, 0.7,
		-0.6, 0.7,
		-0.8, 0.7,
		-1.0, 0.7,
		 1.2, 0.7,
		-1.2, 0.7
	};
#endif
	
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0x3f, 0x00,
		0x32, 0x00,
		0x26, 0x00,
		0x19, 0x00,
		0x0d, 0x00,
		0xf3, 0x00,
		0xe6, 0x00,
		0xda, 0x00,
		0xcd, 0x00,
		0xc0, 0x00,
		0x3f, 0x00,
		0xc0, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamples8BitsMSBWith4Channels)
{
	const tint c_noInChannels = 4;
	const tint c_noOutChannels = 4;
	const tint c_noBits = 7;
	const tint c_bytesPerSample = 1;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 0.7f, 0.7f,
		 0.8f, 0.7f, 0.7f, 0.7f,
		 0.6f, 0.7f, 0.7f, 0.7f,
		 0.4f, 0.7f, 0.7f, 0.7f,
		 0.2f, 0.7f, 0.7f, 0.7f,
		-0.2f, 0.7f, 0.7f, 0.7f,
		-0.4f, 0.7f, 0.7f, 0.7f,
		-0.6f, 0.7f, 0.7f, 0.7f,
		-0.8f, 0.7f, 0.7f, 0.7f,
		-1.0f, 0.7f, 0.7f, 0.7f,
		 1.2f, 0.7f, 0.7f, 0.7f,
		-1.2f, 0.7f, 0.7f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 0.7, 0.7,
		 0.8, 0.7, 0.7, 0.7,
		 0.6, 0.7, 0.7, 0.7,
		 0.4, 0.7, 0.7, 0.7,
		 0.2, 0.7, 0.7, 0.7,
		-0.2, 0.7, 0.7, 0.7,
		-0.4, 0.7, 0.7, 0.7,
		-0.6, 0.7, 0.7, 0.7,
		-0.8, 0.7, 0.7, 0.7,
		-1.0, 0.7, 0.7, 0.7,
		 1.2, 0.7, 0.7, 0.7,
		-1.2, 0.7, 0.7, 0.7
	};
#endif
		
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0x7e, 0x00, 0x00, 0x00,
		0x64, 0x00, 0x00, 0x00,
		0x4c, 0x00, 0x00, 0x00,
		0x32, 0x00, 0x00, 0x00,
		0x1a, 0x00, 0x00, 0x00,
		0xe6, 0x00, 0x00, 0x00,
		0xcc, 0x00, 0x00, 0x00,
		0xb4, 0x00, 0x00, 0x00,
		0x9a, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00,
		0x7e, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesLittleEndian16BitsLSBWith2Channels)
{
	const tint c_noInChannels = 2;
	const tint c_noOutChannels = 2;
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 
		 0.8f, 0.7f,
		 0.6f, 0.7f,
		 0.4f, 0.7f,
		 0.2f, 0.7f,
		-0.2f, 0.7f,
		-0.4f, 0.7f,
		-0.6f, 0.7f,
		-0.8f, 0.7f,
		-1.0f, 0.7f,
		 1.2f, 0.7f,
		-1.2f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 
		 0.8, 0.7,
		 0.6, 0.7,
		 0.4, 0.7,
		 0.2, 0.7,
		-0.2, 0.7,
		-0.4, 0.7,
		-0.6, 0.7,
		-0.8, 0.7,
		-1.0, 0.7,
		 1.2, 0.7,
		-1.2, 0.7
	};
#endif
	
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0xff, 0x3f, 0x00, 0x00,
		0x32, 0x33, 0x00, 0x00,
		0x66, 0x26, 0x00, 0x00,
		0x99, 0x19, 0x00, 0x00,
		0xcd, 0x0c, 0x00, 0x00,
		0x33, 0xf3, 0x00, 0x00,
		0x66, 0xe6, 0x00, 0x00,
		0x9a, 0xd9, 0x00, 0x00,
		0xcd, 0xcc, 0x00, 0x00,
		0x00, 0xc0, 0x00, 0x00,
		0xff, 0x3f, 0x00, 0x00,
		0x00, 0xc0, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesLittleEndian16BitsMSBWith3Channels)
{
	const tint c_noInChannels = 4;
	const tint c_noOutChannels = 3;
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 0.7f, 0.7f,
		 0.8f, 0.7f, 0.7f, 0.7f,
		 0.6f, 0.7f, 0.7f, 0.7f,
		 0.4f, 0.7f, 0.7f, 0.7f,
		 0.2f, 0.7f, 0.7f, 0.7f,
		-0.2f, 0.7f, 0.7f, 0.7f,
		-0.4f, 0.7f, 0.7f, 0.7f,
		-0.6f, 0.7f, 0.7f, 0.7f,
		-0.8f, 0.7f, 0.7f, 0.7f,
		-1.0f, 0.7f, 0.7f, 0.7f,
		 1.2f, 0.7f, 0.7f, 0.7f,
		-1.2f, 0.7f, 0.7f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 0.7, 0.7,
		 0.8, 0.7, 0.7, 0.7,
		 0.6, 0.7, 0.7, 0.7,
		 0.4, 0.7, 0.7, 0.7,
		 0.2, 0.7, 0.7, 0.7,
		-0.2, 0.7, 0.7, 0.7,
		-0.4, 0.7, 0.7, 0.7,
		-0.6, 0.7, 0.7, 0.7,
		-0.8, 0.7, 0.7, 0.7,
		-1.0, 0.7, 0.7, 0.7,
		 1.2, 0.7, 0.7, 0.7,
		-1.2, 0.7, 0.7, 0.7
	};
#endif
		
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00,
		0x64, 0x66, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0x4c, 0x00, 0x00, 0x00, 0x00,
		0x32, 0x33, 0x00, 0x00, 0x00, 0x00,
		0x9a, 0x19, 0x00, 0x00, 0x00, 0x00,
		0x66, 0xe6, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00,
		0x34, 0xb3, 0x00, 0x00, 0x00, 0x00,
		0x9a, 0x99, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
		0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x80, 0x00, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesBigEndian16BitsLSBWith4Channels)
{
	const tint c_noInChannels = 2;
	const tint c_noOutChannels = 4;
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 
		 0.8f, 0.7f,
		 0.6f, 0.7f,
		 0.4f, 0.7f,
		 0.2f, 0.7f,
		-0.2f, 0.7f,
		-0.4f, 0.7f,
		-0.6f, 0.7f,
		-0.8f, 0.7f,
		-1.0f, 0.7f,
		 1.2f, 0.7f,
		-1.2f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 
		 0.8, 0.7,
		 0.6, 0.7,
		 0.4, 0.7,
		 0.2, 0.7,
		-0.2, 0.7,
		-0.4, 0.7,
		-0.6, 0.7,
		-0.8, 0.7,
		-1.0, 0.7,
		 1.2, 0.7,
		-1.2, 0.7
	};
#endif
		
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x33, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x26, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x19, 0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0c, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xf3, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xe6, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xd9, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesBigEndian16BitsMSBWith8Channels)
{
	const tint c_noInChannels = 4;
	const tint c_noOutChannels = 8;
	const tint c_noBits = 15;
	const tint c_bytesPerSample = 2;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 0.7f, 0.7f,
		 0.8f, 0.7f, 0.7f, 0.7f,
		 0.6f, 0.7f, 0.7f, 0.7f,
		 0.4f, 0.7f, 0.7f, 0.7f,
		 0.2f, 0.7f, 0.7f, 0.7f,
		-0.2f, 0.7f, 0.7f, 0.7f,
		-0.4f, 0.7f, 0.7f, 0.7f,
		-0.6f, 0.7f, 0.7f, 0.7f,
		-0.8f, 0.7f, 0.7f, 0.7f,
		-1.0f, 0.7f, 0.7f, 0.7f,
		 1.2f, 0.7f, 0.7f, 0.7f,
		-1.2f, 0.7f, 0.7f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 0.7, 0.7,
		 0.8, 0.7, 0.7, 0.7,
		 0.6, 0.7, 0.7, 0.7,
		 0.4, 0.7, 0.7, 0.7,
		 0.2, 0.7, 0.7, 0.7,
		-0.2, 0.7, 0.7, 0.7,
		-0.4, 0.7, 0.7, 0.7,
		-0.6, 0.7, 0.7, 0.7,
		-0.8, 0.7, 0.7, 0.7,
		-1.0, 0.7, 0.7, 0.7,
		 1.2, 0.7, 0.7, 0.7,
		-1.2, 0.7, 0.7, 0.7
	};
#endif
		
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x66, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x4c, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x33, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x19, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xe6, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xb3, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x99, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesLittleEndian24BitsLSBWith2Channels)
{
	const tint c_noInChannels = 2;
	const tint c_noOutChannels = 2;
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 
		 0.8f, 0.7f,
		 0.6f, 0.7f,
		 0.4f, 0.7f,
		 0.2f, 0.7f,
		-0.2f, 0.7f,
		-0.4f, 0.7f,
		-0.6f, 0.7f,
		-0.8f, 0.7f,
		-1.0f, 0.7f,
		 1.2f, 0.7f,
		-1.2f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 
		 0.8, 0.7,
		 0.6, 0.7,
		 0.4, 0.7,
		 0.2, 0.7,
		-0.2, 0.7,
		-0.4, 0.7,
		-0.6, 0.7,
		-0.8, 0.7,
		-1.0, 0.7,
		 1.2, 0.7,
		-1.2, 0.7
	};
#endif
		
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0xff, 0xff, 0x3f, 0x00, 0x00, 0x00,
		0x32, 0x33, 0x33, 0x00, 0x00, 0x00,
		0x66, 0x66, 0x26, 0x00, 0x00, 0x00,
		0x99, 0x99, 0x19, 0x00, 0x00, 0x00,
		0xcd, 0xcc, 0x0c, 0x00, 0x00, 0x00,
		0x33, 0x33, 0xf3, 0x00, 0x00, 0x00,
		0x66, 0x66, 0xe6, 0x00, 0x00, 0x00,
		0x9a, 0x99, 0xd9, 0x00, 0x00, 0x00,
		0xcd, 0xcc, 0xcc, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
		0xff, 0xff, 0x3f, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xc0, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesLittleEndian24BitsMSBWith4Channels)
{
	const tint c_noInChannels = 4;
	const tint c_noOutChannels = 4;
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 0.7f, 0.7f,
		 0.8f, 0.7f, 0.7f, 0.7f,
		 0.6f, 0.7f, 0.7f, 0.7f,
		 0.4f, 0.7f, 0.7f, 0.7f,
		 0.2f, 0.7f, 0.7f, 0.7f,
		-0.2f, 0.7f, 0.7f, 0.7f,
		-0.4f, 0.7f, 0.7f, 0.7f,
		-0.6f, 0.7f, 0.7f, 0.7f,
		-0.8f, 0.7f, 0.7f, 0.7f,
		-1.0f, 0.7f, 0.7f, 0.7f,
		 1.2f, 0.7f, 0.7f, 0.7f,
		-1.2f, 0.7f, 0.7f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 0.7, 0.7,
		 0.8, 0.7, 0.7, 0.7,
		 0.6, 0.7, 0.7, 0.7,
		 0.4, 0.7, 0.7, 0.7,
		 0.2, 0.7, 0.7, 0.7,
		-0.2, 0.7, 0.7, 0.7,
		-0.4, 0.7, 0.7, 0.7,
		-0.6, 0.7, 0.7, 0.7,
		-0.8, 0.7, 0.7, 0.7,
		-1.0, 0.7, 0.7, 0.7,
		 1.2, 0.7, 0.7, 0.7,
		-1.2, 0.7, 0.7, 0.7
	};
#endif
		
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0xfe, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x64, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcc, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x32, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9a, 0x99, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x66, 0x66, 0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x34, 0x33, 0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9a, 0x99, 0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xfe, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesBigEndian24BitsLSBWith6Channels)
{
	const tint c_noInChannels = 2;
	const tint c_noOutChannels = 6;
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 
		 0.8f, 0.7f,
		 0.6f, 0.7f,
		 0.4f, 0.7f,
		 0.2f, 0.7f,
		-0.2f, 0.7f,
		-0.4f, 0.7f,
		-0.6f, 0.7f,
		-0.8f, 0.7f,
		-1.0f, 0.7f,
		 1.2f, 0.7f,
		-1.2f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 
		 0.8, 0.7,
		 0.6, 0.7,
		 0.4, 0.7,
		 0.2, 0.7,
		-0.2, 0.7,
		-0.4, 0.7,
		-0.6, 0.7,
		-0.8, 0.7,
		-1.0, 0.7,
		 1.2, 0.7,
		-1.2, 0.7
	};
#endif
	
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0x3f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x33, 0x33, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x26, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x19, 0x99, 0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0c, 0xcc, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xf3, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xe6, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xd9, 0x99, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcc, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x3f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesBigEndian24BitsMSBWith8Channels)
{
	const tint c_noInChannels = 4;
	const tint c_noOutChannels = 8;
	const tint c_noBits = 23;
	const tint c_bytesPerSample = 3;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 0.7f, 0.7f,
		 0.8f, 0.7f, 0.7f, 0.7f,
		 0.6f, 0.7f, 0.7f, 0.7f,
		 0.4f, 0.7f, 0.7f, 0.7f,
		 0.2f, 0.7f, 0.7f, 0.7f,
		-0.2f, 0.7f, 0.7f, 0.7f,
		-0.4f, 0.7f, 0.7f, 0.7f,
		-0.6f, 0.7f, 0.7f, 0.7f,
		-0.8f, 0.7f, 0.7f, 0.7f,
		-1.0f, 0.7f, 0.7f, 0.7f,
		 1.2f, 0.7f, 0.7f, 0.7f,
		-1.2f, 0.7f, 0.7f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 0.7, 0.7,
		 0.8, 0.7, 0.7, 0.7,
		 0.6, 0.7, 0.7, 0.7,
		 0.4, 0.7, 0.7, 0.7,
		 0.2, 0.7, 0.7, 0.7,
		-0.2, 0.7, 0.7, 0.7,
		-0.4, 0.7, 0.7, 0.7,
		-0.6, 0.7, 0.7, 0.7,
		-0.8, 0.7, 0.7, 0.7,
		-1.0, 0.7, 0.7, 0.7,
		 1.2, 0.7, 0.7, 0.7,
		-1.2, 0.7, 0.7, 0.7
	};
#endif
		
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x66, 0x66, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x4c, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x33, 0x33, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x19, 0x99, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xe6, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xb3, 0x33, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x99, 0x99, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesLittleEndian32BitsLSBWith2Channels)
{
	const tint c_noInChannels = 2;
	const tint c_noOutChannels = 2;
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 
		 0.8f, 0.7f,
		 0.6f, 0.7f,
		 0.4f, 0.7f,
		 0.2f, 0.7f,
		-0.2f, 0.7f,
		-0.4f, 0.7f,
		-0.6f, 0.7f,
		-0.8f, 0.7f,
		-1.0f, 0.7f,
		 1.2f, 0.7f,
		-1.2f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 
		 0.8, 0.7,
		 0.6, 0.7,
		 0.4, 0.7,
		 0.2, 0.7,
		-0.2, 0.7,
		-0.4, 0.7,
		-0.6, 0.7,
		-0.8, 0.7,
		-1.0, 0.7,
		 1.2, 0.7,
		-1.2, 0.7
	};
#endif
	
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00,
		0x32, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00,
		0x66, 0x66, 0x66, 0x26, 0x00, 0x00, 0x00, 0x00,
		0x99, 0x99, 0x99, 0x19, 0x00, 0x00, 0x00, 0x00,
		0xcd, 0xcc, 0xcc, 0x0c, 0x00, 0x00, 0x00, 0x00,
		0x33, 0x33, 0x33, 0xf3, 0x00, 0x00, 0x00, 0x00,
		0x66, 0x66, 0x66, 0xe6, 0x00, 0x00, 0x00, 0x00,
		0x9a, 0x99, 0x99, 0xd9, 0x00, 0x00, 0x00, 0x00,
		0xcd, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00,
		0xff, 0xff, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesLittleEndian32BitsMSBWith3Channels)
{
	const tint c_noInChannels = 4;
	const tint c_noOutChannels = 3;
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = true;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 0.7f, 0.7f,
		 0.8f, 0.7f, 0.7f, 0.7f,
		 0.6f, 0.7f, 0.7f, 0.7f,
		 0.4f, 0.7f, 0.7f, 0.7f,
		 0.2f, 0.7f, 0.7f, 0.7f,
		-0.2f, 0.7f, 0.7f, 0.7f,
		-0.4f, 0.7f, 0.7f, 0.7f,
		-0.6f, 0.7f, 0.7f, 0.7f,
		-0.8f, 0.7f, 0.7f, 0.7f,
		-1.0f, 0.7f, 0.7f, 0.7f,
		 1.2f, 0.7f, 0.7f, 0.7f,
		-1.2f, 0.7f, 0.7f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 0.7, 0.7,
		 0.8, 0.7, 0.7, 0.7,
		 0.6, 0.7, 0.7, 0.7,
		 0.4, 0.7, 0.7, 0.7,
		 0.2, 0.7, 0.7, 0.7,
		-0.2, 0.7, 0.7, 0.7,
		-0.4, 0.7, 0.7, 0.7,
		-0.6, 0.7, 0.7, 0.7,
		-0.8, 0.7, 0.7, 0.7,
		-1.0, 0.7, 0.7, 0.7,
		 1.2, 0.7, 0.7, 0.7,
		-1.2, 0.7, 0.7, 0.7
	};
#endif
		
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0xfe, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x64, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcc, 0xcc, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x32, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9a, 0x99, 0x99, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x66, 0x66, 0x66, 0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x34, 0x33, 0x33, 0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x9a, 0x99, 0x99, 0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xfe, 0xff, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesBigEndian32BitsLSBWith4Channels)
{
	const tint c_noInChannels = 2;
	const tint c_noOutChannels = 4;
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = false;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 
		 0.8f, 0.7f,
		 0.6f, 0.7f,
		 0.4f, 0.7f,
		 0.2f, 0.7f,
		-0.2f, 0.7f,
		-0.4f, 0.7f,
		-0.6f, 0.7f,
		-0.8f, 0.7f,
		-1.0f, 0.7f,
		 1.2f, 0.7f,
		-1.2f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 
		 0.8, 0.7,
		 0.6, 0.7,
		 0.4, 0.7,
		 0.2, 0.7,
		-0.2, 0.7,
		-0.4, 0.7,
		-0.6, 0.7,
		-0.8, 0.7,
		-1.0, 0.7,
		 1.2, 0.7,
		-1.2, 0.7
	};
#endif
		
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x33, 0x33, 0x33, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x26, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x19, 0x99, 0x99, 0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0c, 0xcc, 0xcc, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xf3, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xe6, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xd9, 0x99, 0x99, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcc, 0xcc, 0xcd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------

TEST(SampleConverter,doubleSamplesBigEndian32BitsMSBWith5Channels)
{
	const tint c_noInChannels = 4;
	const tint c_noOutChannels = 5;
	const tint c_noBits = 31;
	const tint c_bytesPerSample = 4;
	const bool c_littleEndian = false;
	const bool c_alignHigh = true;

#if defined(SINGLE_FLOAT_SAMPLE)
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0f, 0.7f, 0.7f, 0.7f,
		 0.8f, 0.7f, 0.7f, 0.7f,
		 0.6f, 0.7f, 0.7f, 0.7f,
		 0.4f, 0.7f, 0.7f, 0.7f,
		 0.2f, 0.7f, 0.7f, 0.7f,
		-0.2f, 0.7f, 0.7f, 0.7f,
		-0.4f, 0.7f, 0.7f, 0.7f,
		-0.6f, 0.7f, 0.7f, 0.7f,
		-0.8f, 0.7f, 0.7f, 0.7f,
		-1.0f, 0.7f, 0.7f, 0.7f,
		 1.2f, 0.7f, 0.7f, 0.7f,
		-1.2f, 0.7f, 0.7f, 0.7f
	};
#else
    const sample_t c_sampleInput[12 * c_noInChannels] = {
		 1.0, 0.7, 0.7, 0.7,
		 0.8, 0.7, 0.7, 0.7,
		 0.6, 0.7, 0.7, 0.7,
		 0.4, 0.7, 0.7, 0.7,
		 0.2, 0.7, 0.7, 0.7,
		-0.2, 0.7, 0.7, 0.7,
		-0.4, 0.7, 0.7, 0.7,
		-0.6, 0.7, 0.7, 0.7,
		-0.8, 0.7, 0.7, 0.7,
		-1.0, 0.7, 0.7, 0.7,
		 1.2, 0.7, 0.7, 0.7,
		-1.2, 0.7, 0.7, 0.7
	};
#endif
		
	const tubyte c_expectOutput[12 * c_noOutChannels * c_bytesPerSample] = {
		0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x66, 0x66, 0x66, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x4c, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x33, 0x33, 0x33, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x19, 0x99, 0x99, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xe6, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xcc, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xb3, 0x33, 0x33, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x99, 0x99, 0x99, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	
	testSampleConvertionForChannelInterleave(c_noInChannels,c_noOutChannels,c_noBits,c_bytesPerSample,c_littleEndian,c_alignHigh,c_sampleInput,c_expectOutput);
}

//-------------------------------------------------------------------------------------------
