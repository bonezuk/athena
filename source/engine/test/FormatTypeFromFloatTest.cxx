#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "engine/inc/FormatTypeFromFloat.h"

using namespace omega;
using namespace engine;
using namespace testing;

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write32BitsLittleEndianFromSampleAsMinusOne)
{
	tubyte expect[4] = { 0x00, 0x00, 0x00, 0x80 };
	tbyte x[4];
	write32BitsLittleEndianFromSample(-1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,4 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write32BitsLittleEndianFromSampleAsPlusOne)
{
	tubyte expect[4] = { 0xff, 0xff, 0xff, 0x7f };
	tbyte x[4];
	write32BitsLittleEndianFromSample(1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,4 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write32BitsLittleEndianFromSampleAsZero)
{
	tubyte expect[4] = { 0x00, 0x00, 0x00, 0x00 };
	tbyte x[4];
	write32BitsLittleEndianFromSample(0.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,4 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write32BitsLittleEndianFromSampleAsNegativeHalf)
{
	tubyte expect[4] = { 0x00, 0x00, 0x00, 0xc0 };
	tbyte x[4];
	write32BitsLittleEndianFromSample(-0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,4 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write32BitsLittleEndianFromSampleAsPlusHalf)
{
	tubyte expect[4] = { 0x00, 0x00, 0x00, 0x40 };
	tbyte x[4];
    write32BitsLittleEndianFromSample(0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,4 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write32BitsBigEndianFromSampleAsMinusOne)
{
	tubyte expect[4] = { 0x80, 0x00, 0x00, 0x00 };
	tbyte x[4];
	write32BitsBigEndianFromSample(-1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,4 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write32BitsBigEndianFromSampleAsPlusOne)
{
	tubyte expect[4] = { 0x7f, 0xff, 0xff, 0xff };
	tbyte x[4];
	write32BitsBigEndianFromSample(1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,4 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write32BitsBigEndianFromSampleAsZero)
{
	tubyte expect[4] = { 0x00, 0x00, 0x00, 0x00 };
	tbyte x[4];
	write32BitsBigEndianFromSample(0.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,4 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write32BitsBigEndianFromSampleAsNegativeHalf)
{
	tubyte expect[4] = { 0xc0, 0x00, 0x00, 0x00 };
	tbyte x[4];
	write32BitsBigEndianFromSample(-0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,4 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write32BitsBigEndianFromSampleAsPlusHalf)
{
	tubyte expect[4] = { 0x40, 0x00, 0x00, 0x00 };
	tbyte x[4];
    write32BitsBigEndianFromSample(0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,4 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write24BitsLittleEndianFromSampleAsMinusOne)
{
	tubyte expect[3] = { 0x00, 0x00, 0x80 };
	tbyte x[3];
	write24BitsLittleEndianFromSample(-1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,3 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write24BitsLittleEndianFromSampleAsPlusOne)
{
	tubyte expect[3] = { 0xff, 0xff, 0x7f };
	tbyte x[3];
	write24BitsLittleEndianFromSample(1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,3 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write24BitsLittleEndianFromSampleAsZero)
{
	tubyte expect[3] = { 0x00, 0x00, 0x00 };
	tbyte x[3];
	write24BitsLittleEndianFromSample(0.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,3 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write24BitsLittleEndianFromSampleAsNegativeHalf)
{
	tubyte expect[3] = { 0x00, 0x00, 0xc0 };
	tbyte x[3];
	write24BitsLittleEndianFromSample(-0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,3 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write24BitsLittleEndianFromSampleAsPlusHalf)
{
	tubyte expect[3] = { 0x00, 0x00, 0x40 };
	tbyte x[3];
	write24BitsLittleEndianFromSample(0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,3 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write24BitsBigEndianFromSampleAsMinusOne)
{
	tubyte expect[3] = { 0x80, 0x00, 0x00 };
	tbyte x[3];
	write24BitsBigEndianFromSample(-1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,3 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write24BitsBigEndianFromSampleAsPlusOne)
{
	tubyte expect[3] = { 0x7f, 0xff, 0xff };
	tbyte x[3];
	write24BitsBigEndianFromSample(1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,3 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write24BitsBigEndianFromSampleAsZero)
{
	tubyte expect[3] = { 0x00, 0x00, 0x00 };
	tbyte x[3];
	write24BitsBigEndianFromSample(0.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,3 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write24BitsBigEndianFromSampleAsNegativeHalf)
{
	tubyte expect[3] = { 0xc0, 0x00, 0x00 };
	tbyte x[3];
	write24BitsBigEndianFromSample(-0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,3 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write24BitsBigEndianFromSampleAsPlusHalf)
{
	tubyte expect[3] = { 0x40, 0x00, 0x00 };
	tbyte x[3];
    write24BitsBigEndianFromSample(0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,3 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write16BitsLittleEndianFromSampleAsMinusOne)
{
	tubyte expect[2] = { 0x00, 0x80 };
	tbyte x[2];
	write16BitsLittleEndianFromSample(-1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,2 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write16BitsLittleEndianFromSampleAsPlusOne)
{
	tubyte expect[2] = { 0xff, 0x7f };
	tbyte x[2];
	write16BitsLittleEndianFromSample(1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,2 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write16BitsLittleEndianFromSampleAsZero)
{
	tubyte expect[2] = { 0x00, 0x00 };
	tbyte x[2];
	write16BitsLittleEndianFromSample(0.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,2 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write16BitsLittleEndianFromSampleAsNegativeHalf)
{
	tubyte expect[2] = { 0x00, 0xc0 };
	tbyte x[2];
	write16BitsLittleEndianFromSample(-0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,2 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write16BitsLittleEndianFromSampleAsPlusHalf)
{
	const tfloat32 c_Adjust = 0.00001f;
	tubyte expect[2] = { 0xff, 0x3f };
	tbyte x[2];
	write16BitsLittleEndianFromSample(0.5f - c_Adjust,x);
	EXPECT_EQ(0,memcmp(expect,x,2 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write16BitsBigEndianFromSampleAsMinusOne)
{
	tubyte expect[2] = { 0x80, 0x00 };
	tbyte x[2];
	write16BitsBigEndianFromSample(-1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,2 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write16BitsBigEndianFromSampleAsPlusOne)
{
	tubyte expect[2] = { 0x7f, 0xff };
	tbyte x[2];
	write16BitsBigEndianFromSample(1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,2 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write16BitsBigEndianFromSampleAsZero)
{
	tubyte expect[2] = { 0x00, 0x00 };
	tbyte x[2];
	write16BitsBigEndianFromSample(0.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,2 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write16BitsBigEndianFromSampleAsNegativeHalf)
{
	tubyte expect[2] = { 0xc0, 0x00 };
	tbyte x[2];
	write16BitsBigEndianFromSample(-0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,2 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write16BitsBigEndianFromSampleAsPlusHalf)
{
	const tfloat32 c_Adjust = 0.00001f;
	tubyte expect[2] = { 0x3f, 0xff };
	tbyte x[2];
	write16BitsBigEndianFromSample(0.5f - c_Adjust,x);
	EXPECT_EQ(0,memcmp(expect,x,2 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write8BitsFromSampleAsMinusOne)
{
	tubyte expect[1] = { 0x80 };
	tbyte x[1];
	write8BitsFromSample(-1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,1 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write8BitsFromSampleAsPlusOne)
{
	tubyte expect[1] = { 0x7f };
	tbyte x[1];
	write8BitsFromSample(1.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,1 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write8BitsFromSampleAsZero)
{
	tubyte expect[1] = { 0x00 };
	tbyte x[1];
	write8BitsFromSample(0.0f,x);
	EXPECT_EQ(0,memcmp(expect,x,1 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write8BitsFromSampleAsNegativeHalf)
{
	tubyte expect[1] = { 0xc0 };
	tbyte x[1];
	write8BitsFromSample(-0.5f,x);
	EXPECT_EQ(0,memcmp(expect,x,1 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,write8BitsFromSampleAsPlusHalf)
{
	const tfloat32 c_Adjust = 0.00001f;
	tubyte expect[1] = { 0x3f };
	tbyte x[1];
	write8BitsFromSample(0.5f - c_Adjust,x);
	EXPECT_EQ(0,memcmp(expect,x,1 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleToIntegerFor4Bits)
{
	const tfloat32 c_Adjust = 0.00001f;
	
	// Positive
    EXPECT_EQ(0,sampleToInteger(0.0f,4));
	EXPECT_EQ(0,sampleToInteger((1.0f / 14.0f) - c_Adjust,4));
	
	EXPECT_EQ(1,sampleToInteger((1.0f / 14.0f) + c_Adjust,4));
	EXPECT_EQ(1,sampleToInteger(2.0f / 14.0f,4));
	EXPECT_EQ(1,sampleToInteger((3.0f / 14.0f) - c_Adjust,4));
	
	EXPECT_EQ(2,sampleToInteger((3.0f / 14.0f) + c_Adjust,4));
	EXPECT_EQ(2,sampleToInteger(4.0f / 14.0f,4));
	EXPECT_EQ(2,sampleToInteger((5.0f / 14.0f) - c_Adjust,4));
	
	EXPECT_EQ(3,sampleToInteger((5.0f / 14.0f) + c_Adjust,4));
	EXPECT_EQ(3,sampleToInteger(6.0f / 14.0f,4));
	EXPECT_EQ(3,sampleToInteger((7.0f / 14.0f) - c_Adjust,4));
	
	EXPECT_EQ(4,sampleToInteger((7.0f / 14.0f) + c_Adjust,4));
	EXPECT_EQ(4,sampleToInteger(8.0f / 14.0f,4));
	EXPECT_EQ(4,sampleToInteger((9.0f / 14.0f) - c_Adjust,4));
	
	EXPECT_EQ(5,sampleToInteger((9.0f / 14.0f) + c_Adjust,4));
	EXPECT_EQ(5,sampleToInteger(10.0f / 14.0f,4));
	EXPECT_EQ(5,sampleToInteger((11.0f / 14.0f) - c_Adjust,4));
	
	EXPECT_EQ(6,sampleToInteger((11.0f / 14.0f) + c_Adjust,4));
	EXPECT_EQ(6,sampleToInteger(12.0f / 14.0f,4));
	EXPECT_EQ(6,sampleToInteger((13.0f / 14.0f) - c_Adjust,4));
	
	EXPECT_EQ(7,sampleToInteger((13.0f / 14.0f) + c_Adjust,4));
	EXPECT_EQ(7,sampleToInteger(1.0f,4));
	
	// Negative
	EXPECT_EQ(0,sampleToInteger(0.0f - ((1.0f / 16.0f) - c_Adjust),4));
	
	EXPECT_EQ(-1,sampleToInteger(0.0f - ((1.0f / 16.0f) + c_Adjust),4));
	EXPECT_EQ(-1,sampleToInteger(0.0f - (2.0f / 16.0f),4));
	EXPECT_EQ(-1,sampleToInteger(0.0f - ((3.0f / 16.0f) - c_Adjust),4));
	
	EXPECT_EQ(-2,sampleToInteger(0.0f - ((3.0f / 16.0f) + c_Adjust),4));
	EXPECT_EQ(-2,sampleToInteger(0.0f - (4.0f / 16.0f),4));
	EXPECT_EQ(-2,sampleToInteger(0.0f - ((5.0f / 16.0f) - c_Adjust),4));
	
	EXPECT_EQ(-3,sampleToInteger(0.0f - ((5.0f / 16.0f) + c_Adjust),4));
	EXPECT_EQ(-3,sampleToInteger(0.0f - (6.0f / 16.0f),4));
	EXPECT_EQ(-3,sampleToInteger(0.0f - ((7.0f / 16.0f) - c_Adjust),4));
	
	EXPECT_EQ(-4,sampleToInteger(0.0f - ((7.0f / 16.0f) + c_Adjust),4));
	EXPECT_EQ(-4,sampleToInteger(0.0f - (8.0f / 16.0f),4));
	EXPECT_EQ(-4,sampleToInteger(0.0f - ((9.0f / 16.0f) - c_Adjust),4));
	
	EXPECT_EQ(-5,sampleToInteger(0.0f - ((9.0f / 16.0f) + c_Adjust),4));
	EXPECT_EQ(-5,sampleToInteger(0.0f - (10.0f / 16.0f),4));
	EXPECT_EQ(-5,sampleToInteger(0.0f - ((11.0f / 16.0f) - c_Adjust),4));
	
	EXPECT_EQ(-6,sampleToInteger(0.0f - ((11.0f / 16.0f) + c_Adjust),4));
	EXPECT_EQ(-6,sampleToInteger(0.0f - (12.0f / 16.0f),4));
	EXPECT_EQ(-6,sampleToInteger(0.0f - ((13.0f / 16.0f) - c_Adjust),4));
	
	EXPECT_EQ(-7,sampleToInteger(0.0f - ((13.0f / 16.0f) + c_Adjust),4));
	EXPECT_EQ(-7,sampleToInteger(0.0f - (14.0f / 16.0f),4));
	EXPECT_EQ(-7,sampleToInteger(0.0f - ((15.0f / 16.0f) - c_Adjust),4));
	
	EXPECT_EQ(-8,sampleToInteger(0.0f - ((15.0f / 16.0f) + c_Adjust),4));
	EXPECT_EQ(-8,sampleToInteger(-1.0f,4));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLittleEndian16BitsAsLittleEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutput[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	tbyte dest[12];
	
	writeLittleEndian16BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input), reinterpret_cast<tbyte *>(dest),6);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLittleEndian16BitsAsBigEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutput[12] = {
		0x34, 0x12, 0x78, 0x56,
		0xbc, 0x9a, 0xf0, 0xde,
		0xec, 0xfd, 0x2f, 0xa8
	};
	
	tbyte dest[12];
	
	writeLittleEndian16BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,6);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian16BitsAsBigEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutput[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	tbyte dest[12];
	
	writeBigEndian16BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,6);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian16BitsAsLittleEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutput[12] = {
		0x34, 0x12, 0x78, 0x56,
		0xbc, 0x9a, 0xf0, 0xde,
		0xec, 0xfd, 0x2f, 0xa8
	};
	
	tbyte dest[12];
	
	writeBigEndian16BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,6);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeNative16BitsAsLittleEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutputLittle[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutputBig[12] = {
		0x34, 0x12, 0x78, 0x56,
		0xbc, 0x9a, 0xf0, 0xde,
		0xec, 0xfd, 0x2f, 0xa8
	};
	
	tbyte dest[12];
	
	writeNative16BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,6);
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeNative16BitsAsBigEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutputBig[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutputLittle[12] = {
		0x34, 0x12, 0x78, 0x56,
		0xbc, 0x9a, 0xf0, 0xde,
		0xec, 0xfd, 0x2f, 0xa8
	};
	
	tbyte dest[12];
	
	writeNative16BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,6);
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLittleEndian24BitsAsLittleEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};
	
	tbyte dest[12];
	
	writeLittleEndian24BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,4);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLittleEndian24BitsAsBigEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[12] = {
		0x56, 0x34, 0x12,
		0xbc, 0x9a, 0x78,
		0x98, 0x37, 0xef,
		0xf6, 0xa2, 0x89
	};
	
	tbyte dest[12];
	
	writeLittleEndian24BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,4);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian24BitsAsLittleEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[12] = {
		0x56, 0x34, 0x12,
		0xbc, 0x9a, 0x78,
		0x98, 0x37, 0xef,
		0xf6, 0xa2, 0x89
	};
	
	tbyte dest[12];
	
	writeBigEndian24BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,4);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian24BitsAsBigEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};
	
	tbyte dest[12];
	
	writeBigEndian24BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,4);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeNative24BitsAsLittleEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputLittle[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputBig[12] = {
		0x56, 0x34, 0x12,
		0xbc, 0x9a, 0x78,
		0x98, 0x37, 0xef,
		0xf6, 0xa2, 0x89
	};
	
	tbyte dest[12];
	
	writeNative24BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,4);
	
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeNative24BitsAsBigEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputBig[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputLittle[12] = {
		0x56, 0x34, 0x12,
		0xbc, 0x9a, 0x78,
		0x98, 0x37, 0xef,
		0xf6, 0xa2, 0x89
	};
	
	tbyte dest[12];
	
	writeNative24BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,4);
	
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLittleEndian32BitsAsLittleEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37, 
		0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37,
		0x98, 0x89, 0xa2, 0xf6
	};
	
	tbyte dest[12];
	
	writeLittleEndian32BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,3);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLitteEndian32BitsAsBigEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37, 
		0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[12] = {
		0x78, 0x56, 0x34, 0x12,
		0x37, 0xef, 0xbc, 0x9a,
		0xf6, 0xa2, 0x89, 0x98
	};
	
	tbyte dest[12];
	
	writeLittleEndian32BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,3);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian32BitsAsLittleEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37, 
		0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[12] = {
		0x78, 0x56, 0x34, 0x12,
		0x37, 0xef, 0xbc, 0x9a,
		0xf6, 0xa2, 0x89, 0x98
	};
	
	tbyte dest[12];
	
	writeBigEndian32BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,3);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian32BitsAsBigEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37, 
		0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37,
		0x98, 0x89, 0xa2, 0xf6
	};
	
	tbyte dest[12];
	
	writeBigEndian32BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,3);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,12 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeNative32BitsAsLittleEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37, 
		0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputLittle[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37,
		0x98, 0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputBig[12] = {
		0x78, 0x56, 0x34, 0x12,
		0x37, 0xef, 0xbc, 0x9a,
		0xf6, 0xa2, 0x89, 0x98
	};
	
	tbyte dest[12];
	
	writeNative32BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,3);
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeNative32BitsAsBigEndian)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37, 
		0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputBig[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37,
		0x98, 0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputLittle[12] = {
		0x78, 0x56, 0x34, 0x12,
		0x37, 0xef, 0xbc, 0x9a,
		0xf6, 0xa2, 0x89, 0x98
	};
	
	tbyte dest[12];
	
	writeNative32BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,3);
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLittleEndian64BitsAsLittleEndian)
{
	const tubyte input[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};
	
	tbyte dest[16];
	
	writeLittleEndian64BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,2);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,16 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLitteEndian64BitsAsBigEndian)
{
	const tubyte input[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[16] = {
		0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
		0xf6, 0xa2, 0x89, 0x98, 0x37, 0xef, 0xbc, 0x9a
	};
	
	tbyte dest[16];
	
	writeLittleEndian64BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,2);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,16 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian64BitsAsLittleEndian)
{
	const tubyte input[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[16] = {
		0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
		0xf6, 0xa2, 0x89, 0x98, 0x37, 0xef, 0xbc, 0x9a
	};
	
	tbyte dest[16];
	
	writeBigEndian64BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,2);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,16 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian64BitsAsBigEndian)
{
	const tubyte input[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutput[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};
	
	tbyte dest[16];
	
	writeBigEndian64BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,2);
	
	EXPECT_EQ(0,memcmp(c_expectOutput,dest,16 * sizeof(tbyte)));
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeNative64BitsAsLittleEndian)
{
	const tubyte input[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputLittle[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputBig[16] = {
		0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
		0xf6, 0xa2, 0x89, 0x98, 0x37, 0xef, 0xbc, 0x9a
	};

	tbyte dest[16];
	
	writeNative64BitsAsLittleEndian(reinterpret_cast<const tbyte *>(input),dest,2);
	
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,16 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,16 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeNative64BitsAsBigEndian)
{
	const tubyte input[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputLittle[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputBig[16] = {
		0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
		0xf6, 0xa2, 0x89, 0x98, 0x37, 0xef, 0xbc, 0x9a
	};

	tbyte dest[16];
	
	writeNative64BitsAsBigEndian(reinterpret_cast<const tbyte *>(input),dest,2);
	
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,16 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,16 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------


TEST(FormatTypeFromFloat,writeLittleEndian16BitsAsNative)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutputLittle[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutputBig[12] = {
		0x34, 0x12, 0x78, 0x56,
		0xbc, 0x9a, 0xf0, 0xde,
		0xec, 0xfd, 0x2f, 0xa8
	};
	
	tbyte dest[12];
	
	writeLittleEndian16BitsAsNative(reinterpret_cast<const tbyte *>(input),dest,6);
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian16BitsAsNative)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutputLittle[12] = {
		0x12, 0x34, 0x56, 0x78,
		0x9a, 0xbc, 0xde, 0xf0,
		0xfd, 0xec, 0xa8, 0x2f
	};
	
	const tubyte c_expectOutputBig[12] = {
		0x34, 0x12, 0x78, 0x56,
		0xbc, 0x9a, 0xf0, 0xde,
		0xec, 0xfd, 0x2f, 0xa8
	};
	
	tbyte dest[12];
	
	writeBigEndian16BitsAsNative(reinterpret_cast<const tbyte *>(input),dest,6);
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLittleEndian24BitsAsNative)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputLittle[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputBig[12] = {
		0x56, 0x34, 0x12,
		0xbc, 0x9a, 0x78,
		0x98, 0x37, 0xef,
		0xf6, 0xa2, 0x89
	};
	
	tbyte dest[12];
	
	writeLittleEndian24BitsAsNative(reinterpret_cast<const tbyte *>(input),dest,4);
	
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian24BitsAsNative)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputBig[12] = {
		0x12, 0x34, 0x56,
		0x78, 0x9a, 0xbc,
		0xef, 0x37, 0x98,
		0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputLittle[12] = {
		0x56, 0x34, 0x12,
		0xbc, 0x9a, 0x78,
		0x98, 0x37, 0xef,
		0xf6, 0xa2, 0x89
	};
	
	tbyte dest[12];
	
	writeBigEndian24BitsAsNative(reinterpret_cast<const tbyte *>(input),dest,4);
	
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLittleEndian32BitsAsNative)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37, 
		0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputLittle[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37,
		0x98, 0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputBig[12] = {
		0x78, 0x56, 0x34, 0x12,
		0x37, 0xef, 0xbc, 0x9a,
		0xf6, 0xa2, 0x89, 0x98
	};
	
	tbyte dest[12];
	
	writeLittleEndian32BitsAsNative(reinterpret_cast<const tbyte *>(input),dest,3);
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian32BitsAsNative)
{
	const tubyte input[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37, 
		0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputBig[12] = {
		0x12, 0x34, 0x56, 0x78, 
		0x9a, 0xbc, 0xef, 0x37,
		0x98, 0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputLittle[12] = {
		0x78, 0x56, 0x34, 0x12,
		0x37, 0xef, 0xbc, 0x9a,
		0xf6, 0xa2, 0x89, 0x98
	};
	
	tbyte dest[12];
	
	writeBigEndian32BitsAsNative(reinterpret_cast<const tbyte *>(input),dest,3);
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,12 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,12 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeLittleEndian64BitsAsNative)
{
	const tubyte input[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputLittle[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputBig[16] = {
		0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
		0xf6, 0xa2, 0x89, 0x98, 0x37, 0xef, 0xbc, 0x9a
	};

	tbyte dest[16];
	
	writeLittleEndian64BitsAsNative(reinterpret_cast<const tbyte *>(input),dest,2);
	
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,16 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,16 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeBigEndian64BitsAsNative)
{
	const tubyte input[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};
	
	const tubyte c_expectOutputBig[16] = {
		0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
		0x9a, 0xbc, 0xef, 0x37, 0x98, 0x89, 0xa2, 0xf6
	};

	const tubyte c_expectOutputLittle[16] = {
		0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12,
		0xf6, 0xa2, 0x89, 0x98, 0x37, 0xef, 0xbc, 0x9a
	};

	tbyte dest[16];
	
	writeBigEndian64BitsAsNative(reinterpret_cast<const tbyte *>(input),dest,2);
	
	if(isLittleEndian())
	{
		EXPECT_EQ(0,memcmp(c_expectOutputLittle,dest,16 * sizeof(tbyte)));
	}
	else
	{
		EXPECT_EQ(0,memcmp(c_expectOutputBig,dest,16 * sizeof(tbyte)));
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,writeFloat32ToSample)
{
	const tfloat64 c_Tolerance = 0.0000001;

	const tfloat32 c_input[10] = {
		 0.2f,  0.4f,  0.6f,  0.8f,  1.0f,
		-0.2f, -0.4f, -0.6f, -0.8f, -1.0f
	};
	
	const tfloat64 c_expectOutput[10] = {
		 0.2,  0.4,  0.6,  0.8,  1.0,
		-0.2, -0.4, -0.6, -0.8, -1.0
	};
	
	tfloat64 out[10];
	
	writeFloat32ToSample(c_input,out,10);
	
    for(tint i=0;i<10;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],out[i],c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFrom8BitInteger32)
{
	const tfloat64 c_Tolerance = 0.00000001;
	
	const tint32 c_input[13] = {
		-128, -90, -60, -30, -1, 0,
		1, 30, 60, 90, 127, -1000, 1000
	};
	
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_expectOutput[13] = {
		-128.0f / 128.0f, // 0
		 -90.0f / 128.0f, // 1
		 -60.0f / 128.0f, // 2
		 -30.0f / 128.0f, // 3
		  -1.0f / 128.0f, // 4
		   0.0f,         // 5
		   1.0f / 127.0f, // 6
		  30.0f / 127.0f, // 7
		  60.0f / 127.0f, // 8
		  90.0f / 127.0f, // 9
		 127.0f / 127.0f, // 10
		 -1.0f,          // 11
		  1.0f           // 12
	};
#else
	const sample_t c_expectOutput[13] = {
		-128.0 / 128.0, // 0
		 -90.0 / 128.0, // 1
		 -60.0 / 128.0, // 2
		 -30.0 / 128.0, // 3
		  -1.0 / 128.0, // 4
		   0.0,         // 5
		   1.0 / 127.0, // 6
		  30.0 / 127.0, // 7
		  60.0 / 127.0, // 8
		  90.0 / 127.0, // 9
		 127.0 / 127.0, // 10
		 -1.0,          // 11
		  1.0           // 12
	};
#endif
	
	for(tint i=0;i<13;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],sampleFrom8Bit(c_input[i]),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFrom16BitSigned)
{
	const tfloat64 c_Tolerance = 0.00000001;

	const tint16 c_input[11] = {
		-32768, // 0
		-20000, // 1
		-10000, // 2
		  -500, // 3
		    -1, // 4
		     0, // 5
		     1, // 6
		   500, // 7
		 10000, // 8
		 20000, // 9
		 32767  // 10
	};

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_expectOutput[11] = {
		-32768.0f / 32768.0f, // 0
		-20000.0f / 32768.0f, // 1
		-10000.0f / 32768.0f, // 2
		  -500.0f / 32768.0f, // 3
		    -1.0f / 32768.0f, // 4
		     0.0f,           // 5
		     1.0f / 32767.0f, // 6
		   500.0f / 32767.0f, // 7
		 10000.0f / 32767.0f, // 8
		 20000.0f / 32767.0f, // 9
		 32767.0f / 32767.0f  // 10
	};
#else
	const sample_t c_expectOutput[11] = {
		-32768.0 / 32768.0, // 0
		-20000.0 / 32768.0, // 1
		-10000.0 / 32768.0, // 2
		  -500.0 / 32768.0, // 3
		    -1.0 / 32768.0, // 4
		     0.0,           // 5
		     1.0 / 32767.0, // 6
		   500.0 / 32767.0, // 7
		 10000.0 / 32767.0, // 8
		 20000.0 / 32767.0, // 9
		 32767.0 / 32767.0  // 10
	};
#endif

	for(tint i=0;i<11;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],sampleFrom16Bit(c_input[i]),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFrom16BitUnsigned)
{
	const tfloat64 c_Tolerance = 0.00000001;

	const tint16 c_input[11] = {
		-32768, // 0
		-20000, // 1
		-10000, // 2
		  -500, // 3
		    -1, // 4
		     0, // 5
		     1, // 6
		   500, // 7
		 10000, // 8
		 20000, // 9
		 32767  // 10
	};

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_expectOutput[11] = {
		-32768.0f / 32768.0f, // 0
		-20000.0f / 32768.0f, // 1
		-10000.0f / 32768.0f, // 2
		  -500.0f / 32768.0f, // 3
		    -1.0f / 32768.0f, // 4
		     0.0f,           // 5
		     1.0f / 32767.0f, // 6
		   500.0f / 32767.0f, // 7
		 10000.0f / 32767.0f, // 8
		 20000.0f / 32767.0f, // 9
		 32767.0f / 32767.0f  // 10
	};
#else
	const sample_t c_expectOutput[11] = {
		-32768.0 / 32768.0, // 0
		-20000.0 / 32768.0, // 1
		-10000.0 / 32768.0, // 2
		  -500.0 / 32768.0, // 3
		    -1.0 / 32768.0, // 4
		     0.0,           // 5
		     1.0 / 32767.0, // 6
		   500.0 / 32767.0, // 7
		 10000.0 / 32767.0, // 8
		 20000.0 / 32767.0, // 9
		 32767.0 / 32767.0  // 10
	};
#endif	

	for(tint i=0;i<11;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],sampleFrom16Bit(static_cast<tuint16>(c_input[i])),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFrom16BitInteger32)
{
	const tfloat64 c_Tolerance = 0.00000001;

	const tint32 c_input[13] = {
		-32768, // 0
		-20000, // 1
		-10000, // 2
		  -500, // 3
		    -1, // 4
		     0, // 5
		     1, // 6
		   500, // 7
		 10000, // 8
		 20000, // 9
		 32767, // 10
		 -100000, // 11
		  100000  // 12
	};

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_expectOutput[13] = {
		-32768.0f / 32768.0f, // 0
		-20000.0f / 32768.0f, // 1
		-10000.0f / 32768.0f, // 2
		  -500.0f / 32768.0f, // 3
		    -1.0f / 32768.0f, // 4
		     0.0f,           // 5
		     1.0f / 32767.0f, // 6
		   500.0f / 32767.0f, // 7
		 10000.0f / 32767.0f, // 8
		 20000.0f / 32767.0f, // 9
		 32767.0f / 32767.0f, // 10
		 -1.0f,
		  1.0f
	};
#else
	const sample_t c_expectOutput[13] = {
		-32768.0 / 32768.0, // 0
		-20000.0 / 32768.0, // 1
		-10000.0 / 32768.0, // 2
		  -500.0 / 32768.0, // 3
		    -1.0 / 32768.0, // 4
		     0.0,           // 5
		     1.0 / 32767.0, // 6
		   500.0 / 32767.0, // 7
		 10000.0 / 32767.0, // 8
		 20000.0 / 32767.0, // 9
		 32767.0 / 32767.0, // 10
		 -1.0,
		  1.0
	};
#endif

	for(tint i=0;i<13;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],sampleFrom16Bit(c_input[i]),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFrom24BitSigned)
{
	const tfloat64 c_Tolerance = 0.00000001;
	
	const tint32 c_input[13] = {
		-8388608, // 0
		-2000000, // 1
		-1000000, // 2
		    -500, // 3
		      -1, // 4
		       0, // 5
		       1, // 6
		     500, // 7
		 2000000, // 8
		 1000000, // 9		
		 8388607, // 10
		-9000000, // 11
		 9000000  // 12
	};
	
	const tfloat64 c_expectOutput[13] = {
		-8388608.0 / 8388608.0, // 0
		-2000000.0 / 8388608.0, // 1
		-1000000.0 / 8388608.0, // 2
		    -500.0 / 8388608.0, // 3
		      -1.0 / 8388608.0, // 4
		       0.0,             // 5
		       1.0 / 8388607.0, // 6
		     500.0 / 8388607.0, // 7
		 2000000.0 / 8388607.0, // 8
		 1000000.0 / 8388607.0, // 9		
		 8388607.0 / 8388607.0, // 10
		 -1.0,
		  1.0
	};
	
	for(tint i=0;i<13;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],sampleFrom24Bit(c_input[i]),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFrom24BitUnsigned)
{
	const tfloat64 c_Tolerance = 0.00000001;
	
	const tint32 c_input[13] = {
		-8388608, // 0
		-2000000, // 1
		-1000000, // 2
		    -500, // 3
		      -1, // 4
		       0, // 5
		       1, // 6
		     500, // 7
		 2000000, // 8
		 1000000, // 9		
		 8388607, // 10
		-9000000, // 11
		 9000000  // 12
	};
	
	const tfloat64 c_expectOutput[13] = {
		-8388608.0 / 8388608.0, // 0
		-2000000.0 / 8388608.0, // 1
		-1000000.0 / 8388608.0, // 2
		    -500.0 / 8388608.0, // 3
		      -1.0 / 8388608.0, // 4
		       0.0,             // 5
		       1.0 / 8388607.0, // 6
		     500.0 / 8388607.0, // 7
		 2000000.0 / 8388607.0, // 8
		 1000000.0 / 8388607.0, // 9		
		 8388607.0 / 8388607.0, // 10
		 -1.0,
		  1.0
	};
	
	for(tint i=0;i<13;i++)
	{
        EXPECT_NEAR(c_expectOutput[i],sampleFrom24Bit(static_cast<tuint32>(c_input[i])),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFrom32BitSigned)
{
	const tfloat64 c_Tolerance = 0.00000001;
	
	const tint32 c_input[11] = {
		 0x7fffffff, // 0
		 1000000000, // 1
		    5000000, // 2
		        500, // 3
		          1, // 4
		          0, // 5
		         -1, // 6
		       -500, // 7
		   -5000000, // 8
		-1000000000, // 9
		-2147483648  // 10
	};

	const tfloat64 c_expectOutput[11] = {
		 2147483647.0 / 2147483647.0, // 0
		 1000000000.0 / 2147483647.0, // 1
		    5000000.0 / 2147483647.0, // 2
		        500.0 / 2147483647.0, // 3
		          1.0 / 2147483647.0, // 4
		          0.0,                // 5
		         -1.0 / 2147483648.0, // 6
		       -500.0 / 2147483648.0, // 7
		   -5000000.0 / 2147483648.0, // 8
		-1000000000.0 / 2147483648.0, // 9	
		-2147483648.0 / 2147483648.0  // 10
	};
	
	for(tint i=0;i<11;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],sampleFrom32Bit(c_input[i]),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFrom32BitUnsigned)
{
	const tfloat64 c_Tolerance = 0.00000001;
	
	const tint32 c_input[11] = {
		 0x7fffffff, // 0
		 1000000000, // 1
		    5000000, // 2
		        500, // 3
		          1, // 4
		          0, // 5
		         -1, // 6
		       -500, // 7
		   -5000000, // 8
		-1000000000, // 9	
		-2147483648  // 10
	};

	const tfloat64 c_expectOutput[11] = {
		 2147483647.0 / 2147483647.0, // 0
		 1000000000.0 / 2147483647.0, // 1
		    5000000.0 / 2147483647.0, // 2
		        500.0 / 2147483647.0, // 3
		          1.0 / 2147483647.0, // 4
		          0.0,                // 5
		         -1.0 / 2147483648.0, // 6
		       -500.0 / 2147483648.0, // 7
		   -5000000.0 / 2147483648.0, // 8
		-1000000000.0 / 2147483648.0, // 9	
		-2147483648.0 / 2147483648.0  // 10
	};
	
	for(tint i=0;i<11;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],sampleFrom32Bit(static_cast<tuint32>(c_input[i])),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFromIntegerUsing8Bits)
{
	const tfloat64 c_Tolerance = 0.00000001;
	
	const tint32 c_input[13] = {
		-128, -90, -60, -30, -1, 0,
		1, 30, 60, 90, 127, -1000, 1000
	};
	
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_expectOutput[13] = {
		-128.0f / 128.0f, // 0
		 -90.0f / 128.0f, // 1
		 -60.0f / 128.0f, // 2
		 -30.0f / 128.0f, // 3
		  -1.0f / 128.0f, // 4
		   0.0f,         // 5
		   1.0f / 127.0f, // 6
		  30.0f / 127.0f, // 7
		  60.0f / 127.0f, // 8
		  90.0f / 127.0f, // 9
		 127.0f / 127.0f, // 10
		 -1.0f,          // 11
		  1.0f           // 12
	};
#else
	const sample_t c_expectOutput[13] = {
		-128.0 / 128.0, // 0
		 -90.0 / 128.0, // 1
		 -60.0 / 128.0, // 2
		 -30.0 / 128.0, // 3
		  -1.0 / 128.0, // 4
		   0.0,         // 5
		   1.0 / 127.0, // 6
		  30.0 / 127.0, // 7
		  60.0 / 127.0, // 8
		  90.0 / 127.0, // 9
		 127.0 / 127.0, // 10
		 -1.0,          // 11
		  1.0           // 12
	};
#endif
	
	SampleFromInteger s(8);
	
	for(tint i=0;i<13;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],s.convert(c_input[i]),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFromIntegerUsing16Bits)
{
	const tfloat64 c_Tolerance = 0.00000001;

	const tint32 c_input[13] = {
		-32768, // 0
		-20000, // 1
		-10000, // 2
		  -500, // 3
		    -1, // 4
		     0, // 5
		     1, // 6
		   500, // 7
		 10000, // 8
		 20000, // 9
		 32767, // 10
		 -100000, // 11
		  100000  // 12
	};

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_expectOutput[13] = {
		-32768.0f / 32768.0f, // 0
		-20000.0f / 32768.0f, // 1
		-10000.0f / 32768.0f, // 2
		  -500.0f / 32768.0f, // 3
		    -1.0f / 32768.0f, // 4
		     0.0f,           // 5
		     1.0f / 32767.0f, // 6
		   500.0f / 32767.0f, // 7
		 10000.0f / 32767.0f, // 8
		 20000.0f / 32767.0f, // 9
		 32767.0f / 32767.0f, // 10
		 -1.0f,
		  1.0f
	};
#else
	const sample_t c_expectOutput[13] = {
		-32768.0 / 32768.0, // 0
		-20000.0 / 32768.0, // 1
		-10000.0 / 32768.0, // 2
		  -500.0 / 32768.0, // 3
		    -1.0 / 32768.0, // 4
		     0.0,           // 5
		     1.0 / 32767.0, // 6
		   500.0 / 32767.0, // 7
		 10000.0 / 32767.0, // 8
		 20000.0 / 32767.0, // 9
		 32767.0 / 32767.0, // 10
		 -1.0,
		  1.0
	};
#endif

	SampleFromInteger s(16);

	for(tint i=0;i<13;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],s.convert(c_input[i]),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFromIntegerUsing24Bits)
{
	const tfloat64 c_Tolerance = 0.00000001;
	
	const tint32 c_input[13] = {
		-8388608, // 0
		-2000000, // 1
		-1000000, // 2
		    -500, // 3
		      -1, // 4
		       0, // 5
		       1, // 6
		     500, // 7
		 2000000, // 8
		 1000000, // 9		
		 8388607, // 10
		-9000000, // 11
		 9000000  // 12
	};
	
	const tfloat64 c_expectOutput[13] = {
		-8388608.0 / 8388608.0, // 0
		-2000000.0 / 8388608.0, // 1
		-1000000.0 / 8388608.0, // 2
		    -500.0 / 8388608.0, // 3
		      -1.0 / 8388608.0, // 4
		       0.0,             // 5
		       1.0 / 8388607.0, // 6
		     500.0 / 8388607.0, // 7
		 2000000.0 / 8388607.0, // 8
		 1000000.0 / 8388607.0, // 9		
		 8388607.0 / 8388607.0, // 10
		 -1.0,
		  1.0
	};
	
	SampleFromInteger s(24);
	
	for(tint i=0;i<13;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],s.convert(c_input[i]),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(FormatTypeFromFloat,sampleFromIntegerUsing32Bits)
{
	const tfloat64 c_Tolerance = 0.00000001;
	
	const tint32 c_input[11] = {
		 0x7fffffff, // 0
		 1000000000, // 1
		    5000000, // 2
		        500, // 3
		          1, // 4
		          0, // 5
		         -1, // 6
		       -500, // 7
		   -5000000, // 8
		-1000000000, // 9	
		-2147483648  // 10
	};

	const tfloat64 c_expectOutput[11] = {
		 2147483647.0 / 2147483647.0, // 0
		 1000000000.0 / 2147483647.0, // 1
		    5000000.0 / 2147483647.0, // 2
		        500.0 / 2147483647.0, // 3
		          1.0 / 2147483647.0, // 4
		          0.0,                // 5
		         -1.0 / 2147483648.0, // 6
		       -500.0 / 2147483648.0, // 7
		   -5000000.0 / 2147483648.0, // 8
		-1000000000.0 / 2147483648.0, // 9	
		-2147483648.0 / 2147483648.0  // 10
	};
	
	SampleFromInteger s(32);
	
	for(tint i=0;i<11;i++)
	{
		EXPECT_NEAR(c_expectOutput[i],s.convert(c_input[i]),c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------
