#include "gtest/gtest.h"

#include "audioio/inc/ASIOData.h"
#include "engine/inc/FormatTypeFromFloat.h"

using namespace omega;
using namespace audioio;
using namespace testing;

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo16BitLSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamples[24] = {
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

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt16LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,24));
}

TEST(ASIOData,sampleTo16BitLSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[12] = {
		0xff, 0x7f, // 0
		0xcc, 0x4c, // 2
		0x99, 0x19, // 4
		0xcd, 0xcc, // 6
		0x9a, 0x99, // 8
		0xff, 0x7f  // 10
	};
	
	const tubyte c_expectIntegerSamplesB[12] = {
		0x66, 0x66, // 1
		0x33, 0x33, // 3
		0x66, 0xe6, // 5
		0x33, 0xb3, // 7
		0x00, 0x80, // 9
		0x00, 0x80  // 11
	};

	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt16LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
}

TEST(ASIOData,sampleTo16BitLSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[6] = {
		0xff, 0x7f, // 0
		0x99, 0x19, // 4
		0x9a, 0x99  // 8
	};
	
	const tubyte c_expectIntegerSamplesB[6] = {
		0x66, 0x66, // 1
		0x66, 0xe6, // 5
		0x00, 0x80  // 9
	};
	
	const tubyte c_expectIntegerSamplesC[6] = {
		0xcc, 0x4c, // 2
		0xcd, 0xcc, // 6
		0xff, 0x7f  // 10
	};
	
	const tubyte c_expectIntegerSamplesD[6] = {
		0x33, 0x33, // 3
		0x33, 0xb3, // 7
		0x00, 0x80  // 11
	};
	
	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt16LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,6));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,6));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,6));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,6));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo16BitMSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamples[24] = {
		0x7f, 0xff, // 0
		0x66, 0x66, // 1
		0x4c, 0xcc, // 2
		0x33, 0x33, // 3
		0x19, 0x99, // 4
		0xe6, 0x66, // 5
		0xcc, 0xcd, // 6
		0xb3, 0x33, // 7
		0x99, 0x9a, // 8
		0x80, 0x00, // 9
		0x7f, 0xff, // 10
		0x80, 0x00  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt16MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,24));
}

TEST(ASIOData,sampleTo16BitMSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[12] = {
		0x7f, 0xff, // 0
		0x4c, 0xcc, // 2
		0x19, 0x99, // 4
		0xcc, 0xcd, // 6
		0x99, 0x9a, // 8
		0x7f, 0xff, // 10
	};

	const tubyte c_expectIntegerSamplesB[12] = {
		0x66, 0x66, // 1
		0x33, 0x33, // 3
		0xe6, 0x66, // 5
		0xb3, 0x33, // 7
		0x80, 0x00, // 9
		0x80, 0x00  // 11
	};
	
	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt16MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
}

TEST(ASIOData,sampleTo16BitMSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[6] = {
		0x7f, 0xff, // 0
		0x19, 0x99, // 4
		0x99, 0x9a  // 8
	};
	
	const tubyte c_expectIntegerSamplesB[6] = {
		0x66, 0x66, // 1
		0xe6, 0x66, // 5
		0x80, 0x00  // 9
	};

	const tubyte c_expectIntegerSamplesC[6] = {
		0x4c, 0xcc, // 2
		0xcc, 0xcd, // 6
		0x7f, 0xff  // 10
	};
	
	const tubyte c_expectIntegerSamplesD[6] = {
		0x33, 0x33, // 3
		0xb3, 0x33, // 7
		0x80, 0x00  // 11
	};

	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt16MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,6));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,6));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,6));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,6));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo24BitLSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710887 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamples[36] = {
		0xff, 0xff, 0x7f, // 0
		0x66, 0x66, 0x66, // 1
		0xcc, 0xcc, 0x4c, // 2
		0x33, 0x33, 0x33, // 3
		0x99, 0x99, 0x19, // 4
		0x66, 0x66, 0xe6, // 5
		0xcd, 0xcc, 0xcc, // 6
		0x33, 0x33, 0xb3, // 7
		0x9a, 0x99, 0x99, // 8
		0x00, 0x00, 0x80, // 9
		0xff, 0xff, 0x7f, // 10
		0x00, 0x00, 0x80  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt24LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,36));
}

TEST(ASIOData,sampleTo24BitLSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710887 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamplesA[18] = {
		0xff, 0xff, 0x7f, // 0
		0xcc, 0xcc, 0x4c, // 2
		0x99, 0x99, 0x19, // 4
		0xcd, 0xcc, 0xcc, // 6
		0x9a, 0x99, 0x99, // 8
		0xff, 0xff, 0x7f, // 10
	};

	const tubyte c_expectIntegerSamplesB[18] = {
		0x66, 0x66, 0x66, // 1
		0x33, 0x33, 0x33, // 3
		0x66, 0x66, 0xe6, // 5
		0x33, 0x33, 0xb3, // 7
		0x00, 0x00, 0x80, // 9
		0x00, 0x00, 0x80  // 11
	};
	
	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt24LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,18));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,18));
}

TEST(ASIOData,sampleTo24BitLSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710887 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif
		
	const tubyte c_expectIntegerSamplesA[9] = {
		0xff, 0xff, 0x7f, // 0
		0x99, 0x99, 0x19, // 4
		0x9a, 0x99, 0x99  // 8
	};

	const tubyte c_expectIntegerSamplesB[9] = {
		0x66, 0x66, 0x66, // 1
		0x66, 0x66, 0xe6, // 5
		0x00, 0x00, 0x80  // 9
	};
	
	const tubyte c_expectIntegerSamplesC[9] = {
		0xcc, 0xcc, 0x4c, // 2
		0xcd, 0xcc, 0xcc, // 6
		0xff, 0xff, 0x7f  // 10
	};

	const tubyte c_expectIntegerSamplesD[9] = {
		0x33, 0x33, 0x33, // 3
		0x33, 0x33, 0xb3, // 7
		0x00, 0x00, 0x80  // 11
	};
	
	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt24LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,9));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,9));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,9));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,9));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo24BitMSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710887 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif
		
	const tubyte c_expectIntegerSamples[36] = {
		0x7f, 0xff, 0xff, // 0
		0x66, 0x66, 0x66, // 1
		0x4c, 0xcc, 0xcc, // 2
		0x33, 0x33, 0x33, // 3
		0x19, 0x99, 0x99, // 4
		0xe6, 0x66, 0x66, // 5
		0xcc, 0xcc, 0xcd, // 6
		0xb3, 0x33, 0x33, // 7
		0x99, 0x99, 0x9a, // 8
		0x80, 0x00, 0x00, // 9 
		0x7f, 0xff, 0xff, // 10
		0x80, 0x00, 0x00  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt24MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,36));
}

TEST(ASIOData,sampleTo24BitMSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710887 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamplesA[18] = {
		0x7f, 0xff, 0xff, // 0
		0x4c, 0xcc, 0xcc, // 2
		0x19, 0x99, 0x99, // 4
		0xcc, 0xcc, 0xcd, // 6
		0x99, 0x99, 0x9a, // 8
		0x7f, 0xff, 0xff  // 10
	};
	
	const tubyte c_expectIntegerSamplesB[18] = {
		0x66, 0x66, 0x66, // 1
		0x33, 0x33, 0x33, // 3
		0xe6, 0x66, 0x66, // 5
		0xb3, 0x33, 0x33, // 7
		0x80, 0x00, 0x00, // 9 
		0x80, 0x00, 0x00  // 11
	};

	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt24MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,18));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,18));
}

TEST(ASIOData,sampleTo24BitMSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710887 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamplesA[9] = {
		0x7f, 0xff, 0xff, // 0
		0x19, 0x99, 0x99, // 4
		0x99, 0x99, 0x9a, // 8
	};
	
	const tubyte c_expectIntegerSamplesB[9] = {
		0x66, 0x66, 0x66, // 1
		0xe6, 0x66, 0x66, // 5
		0x80, 0x00, 0x00, // 9 
	};

	const tubyte c_expectIntegerSamplesC[9] = {
		0x4c, 0xcc, 0xcc, // 2
		0xcc, 0xcc, 0xcd, // 6
		0x7f, 0xff, 0xff, // 10
	};

	const tubyte c_expectIntegerSamplesD[9] = {
		0x33, 0x33, 0x33, // 3
		0xb3, 0x33, 0x33, // 7
		0x80, 0x00, 0x00  // 11
	};

	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt24MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,9));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,9));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,9));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,9));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo32BitLSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:   0x7FFFFFFF =  2147483647 <-  4294967294 =  4294967294.0 / 4294967294.0 =  1.0
		 0.8, //  1:   0x66666666 =  1717986918 <-  3435973836 =  3435973835.2 / 4294967294.0 =  0.8 ( 3435973835.2  3435973834=1.2  3435973836=0.8)
		 0.6, //  2:   0x4CCCCCCC =  1288490188 <-  2576980376 =  2576980376.4 / 4294967294.0 =  0.6 ( 2576980376.4  2576980376=0.4  2576980378=1.6)
		 0.4, //  3:   0x33333333 =   858993459 <-  1717986918 =  1717986917.6 / 4294967294.0 =  0.4 ( 1717986917.6  1717986916=1.6  1717986918=0.4)
		 0.2, //  4:   0x19999999 =   429496729 <-   858993458 =   858993458.8 / 4294967294.0 =  0.2 (  858993458.8   858993458=0.8   858993460=1.2)
		-0.2, //  5:   0xE6666666 =  -429496730 <-  -858993460 =  -858993459.2 / 4294967296.0 = -0.2 ( -858993459.2  -858993458=1.2  -858993460=0.8)
		-0.4, //  6:   0xCCCCCCCD =  -858993459 <- -1717986918 = -1717986918.4 / 4294967296.0 = -0.4 (-1717986918.4 -1717986918=0.4 -1717986920=1.6)
		-0.6, //  7:   0xB3333333 = -1288490189 <- -2576980378 = -2576980377.6 / 4294967296.0 = -0.6 (-2576980377.6 -2576980376=1.6 -2576980378=0.4)
		-0.8, //  8:   0x9999999A = -1717986918 <- -3435973836 = -3435973836.8 / 4294967296.0 = -0.8 (-3435973836.8 -3435973836=0.8 -3435973838=1.2)
		-1.0, //  9:   0x80000000 = -2147483648 <- -4294967296 = -4294967296.0 / 4294967296.0 = -1.0
		 1.2, // 10:   0x7FFFFFFF =  2147483647 =  1.2
		-1.2  // 11:   0x80000000 = -2147483648 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamples[48] = {
		0xff, 0xff, 0xff, 0x7f, // 0
		0x66, 0x66, 0x66, 0x66, // 1
		0xcc, 0xcc, 0xcc, 0x4c, // 2
		0x33, 0x33, 0x33, 0x33, // 3
		0x99, 0x99, 0x99, 0x19, // 4
		0x66, 0x66, 0x66, 0xe6, // 5
		0xcd, 0xcc, 0xcc, 0xcc, // 6
		0x33, 0x33, 0x33, 0xb3, // 7
		0x9a, 0x99, 0x99, 0x99, // 8
		0x00, 0x00, 0x00, 0x80, // 9
		0xff, 0xff, 0xff, 0x7f, // 10
		0x00, 0x00, 0x00, 0x80  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt32LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,48));
}

TEST(ASIOData,sampleTo32BitLSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:   0x7FFFFFFF =  2147483647 <-  4294967294 =  4294967294.0 / 4294967294.0 =  1.0
		 0.8, //  1:   0x66666666 =  1717986918 <-  3435973836 =  3435973835.2 / 4294967294.0 =  0.8 ( 3435973835.2  3435973834=1.2  3435973836=0.8)
		 0.6, //  2:   0x4CCCCCCC =  1288490188 <-  2576980376 =  2576980376.4 / 4294967294.0 =  0.6 ( 2576980376.4  2576980376=0.4  2576980378=1.6)
		 0.4, //  3:   0x33333333 =   858993459 <-  1717986918 =  1717986917.6 / 4294967294.0 =  0.4 ( 1717986917.6  1717986916=1.6  1717986918=0.4)
		 0.2, //  4:   0x19999999 =   429496729 <-   858993458 =   858993458.8 / 4294967294.0 =  0.2 (  858993458.8   858993458=0.8   858993460=1.2)
		-0.2, //  5:   0xE6666666 =  -429496730 <-  -858993460 =  -858993459.2 / 4294967296.0 = -0.2 ( -858993459.2  -858993458=1.2  -858993460=0.8)
		-0.4, //  6:   0xCCCCCCCD =  -858993459 <- -1717986918 = -1717986918.4 / 4294967296.0 = -0.4 (-1717986918.4 -1717986918=0.4 -1717986920=1.6)
		-0.6, //  7:   0xB3333333 = -1288490189 <- -2576980378 = -2576980377.6 / 4294967296.0 = -0.6 (-2576980377.6 -2576980376=1.6 -2576980378=0.4)
		-0.8, //  8:   0x9999999A = -1717986918 <- -3435973836 = -3435973836.8 / 4294967296.0 = -0.8 (-3435973836.8 -3435973836=0.8 -3435973838=1.2)
		-1.0, //  9:   0x80000000 = -2147483648 <- -4294967296 = -4294967296.0 / 4294967296.0 = -1.0
		 1.2, // 10:   0x7FFFFFFF =  2147483647 =  1.2
		-1.2  // 11:   0x80000000 = -2147483648 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamplesA[24] ={
		0xff,0xff,0xff,0x7f, // 0
		0xcc,0xcc,0xcc,0x4c, // 2
		0x99,0x99,0x99,0x19, // 4
		0xcd,0xcc,0xcc,0xcc, // 6
		0x9a,0x99,0x99,0x99, // 8
		0xff,0xff,0xff,0x7f, // 10
	};

	const tubyte c_expectIntegerSamplesB[24] ={
		0x66,0x66,0x66,0x66, // 1
		0x33,0x33,0x33,0x33, // 3
		0x66,0x66,0x66,0xe6, // 5
		0x33,0x33,0x33,0xb3, // 7
		0x00,0x00,0x00,0x80, // 9
		0x00,0x00,0x00,0x80  // 11
	};
	
	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt32LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,24));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,24));
}

TEST(ASIOData,sampleTo32BitLSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:   0x7FFFFFFF =  2147483647 <-  4294967294 =  4294967294.0 / 4294967294.0 =  1.0
		 0.8, //  1:   0x66666666 =  1717986918 <-  3435973836 =  3435973835.2 / 4294967294.0 =  0.8 ( 3435973835.2  3435973834=1.2  3435973836=0.8)
		 0.6, //  2:   0x4CCCCCCC =  1288490188 <-  2576980376 =  2576980376.4 / 4294967294.0 =  0.6 ( 2576980376.4  2576980376=0.4  2576980378=1.6)
		 0.4, //  3:   0x33333333 =   858993459 <-  1717986918 =  1717986917.6 / 4294967294.0 =  0.4 ( 1717986917.6  1717986916=1.6  1717986918=0.4)
		 0.2, //  4:   0x19999999 =   429496729 <-   858993458 =   858993458.8 / 4294967294.0 =  0.2 (  858993458.8   858993458=0.8   858993460=1.2)
		-0.2, //  5:   0xE6666666 =  -429496730 <-  -858993460 =  -858993459.2 / 4294967296.0 = -0.2 ( -858993459.2  -858993458=1.2  -858993460=0.8)
		-0.4, //  6:   0xCCCCCCCD =  -858993459 <- -1717986918 = -1717986918.4 / 4294967296.0 = -0.4 (-1717986918.4 -1717986918=0.4 -1717986920=1.6)
		-0.6, //  7:   0xB3333333 = -1288490189 <- -2576980378 = -2576980377.6 / 4294967296.0 = -0.6 (-2576980377.6 -2576980376=1.6 -2576980378=0.4)
		-0.8, //  8:   0x9999999A = -1717986918 <- -3435973836 = -3435973836.8 / 4294967296.0 = -0.8 (-3435973836.8 -3435973836=0.8 -3435973838=1.2)
		-1.0, //  9:   0x80000000 = -2147483648 <- -4294967296 = -4294967296.0 / 4294967296.0 = -1.0
		 1.2, // 10:   0x7FFFFFFF =  2147483647 =  1.2
		-1.2  // 11:   0x80000000 = -2147483648 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamplesA[12] ={
		0xff,0xff,0xff,0x7f, // 0
		0x99,0x99,0x99,0x19, // 4
		0x9a,0x99,0x99,0x99  // 8
	};

	const tubyte c_expectIntegerSamplesB[12] ={
		0x66,0x66,0x66,0x66, // 1
		0x66,0x66,0x66,0xe6, // 5
		0x00,0x00,0x00,0x80  // 9
	};

	const tubyte c_expectIntegerSamplesC[12] ={
		0xcc,0xcc,0xcc,0x4c, // 2
		0xcd,0xcc,0xcc,0xcc, // 6
		0xff,0xff,0xff,0x7f  // 10
	};

	const tubyte c_expectIntegerSamplesD[12] ={
		0x33,0x33,0x33,0x33, // 3
		0x33,0x33,0x33,0xb3, // 7
		0x00,0x00,0x00,0x80  // 11
	};

	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt32LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,12));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,12));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo32BitMSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:   0x7FFFFFFF =  2147483647 <-  4294967294 =  4294967294.0 / 4294967294.0 =  1.0
		 0.8, //  1:   0x66666666 =  1717986918 <-  3435973836 =  3435973835.2 / 4294967294.0 =  0.8 ( 3435973835.2  3435973834=1.2  3435973836=0.8)
		 0.6, //  2:   0x4CCCCCCC =  1288490188 <-  2576980376 =  2576980376.4 / 4294967294.0 =  0.6 ( 2576980376.4  2576980376=0.4  2576980378=1.6)
		 0.4, //  3:   0x33333333 =   858993459 <-  1717986918 =  1717986917.6 / 4294967294.0 =  0.4 ( 1717986917.6  1717986916=1.6  1717986918=0.4)
		 0.2, //  4:   0x19999999 =   429496729 <-   858993458 =   858993458.8 / 4294967294.0 =  0.2 (  858993458.8   858993458=0.8   858993460=1.2)
		-0.2, //  5:   0xE6666666 =  -429496730 <-  -858993460 =  -858993459.2 / 4294967296.0 = -0.2 ( -858993459.2  -858993458=1.2  -858993460=0.8)
		-0.4, //  6:   0xCCCCCCCD =  -858993459 <- -1717986918 = -1717986918.4 / 4294967296.0 = -0.4 (-1717986918.4 -1717986918=0.4 -1717986920=1.6)
		-0.6, //  7:   0xB3333333 = -1288490189 <- -2576980378 = -2576980377.6 / 4294967296.0 = -0.6 (-2576980377.6 -2576980376=1.6 -2576980378=0.4)
		-0.8, //  8:   0x9999999A = -1717986918 <- -3435973836 = -3435973836.8 / 4294967296.0 = -0.8 (-3435973836.8 -3435973836=0.8 -3435973838=1.2)
		-1.0, //  9:   0x80000000 = -2147483648 <- -4294967296 = -4294967296.0 / 4294967296.0 = -1.0
		 1.2, // 10:   0x7FFFFFFF =  2147483647 =  1.2
		-1.2  // 11:   0x80000000 = -2147483648 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamples[48] = {
		0x7f, 0xff, 0xff, 0xff, // 0
		0x66, 0x66, 0x66, 0x66, // 1
		0x4c, 0xcc, 0xcc, 0xcc, // 2
		0x33, 0x33, 0x33, 0x33, // 3
		0x19, 0x99, 0x99, 0x99, // 4
		0xe6, 0x66, 0x66, 0x66, // 5
		0xcc, 0xcc, 0xcc, 0xcd, // 6
		0xb3, 0x33, 0x33, 0x33, // 7
		0x99, 0x99, 0x99, 0x9a, // 8
		0x80, 0x00, 0x00, 0x00, // 9
		0x7f, 0xff, 0xff, 0xff, // 10
		0x80, 0x00, 0x00, 0x00  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt32MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,48));
}

TEST(ASIOData,sampleTo32BitMSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:   0x7FFFFFFF =  2147483647 <-  4294967294 =  4294967294.0 / 4294967294.0 =  1.0
		 0.8, //  1:   0x66666666 =  1717986918 <-  3435973836 =  3435973835.2 / 4294967294.0 =  0.8 ( 3435973835.2  3435973834=1.2  3435973836=0.8)
		 0.6, //  2:   0x4CCCCCCC =  1288490188 <-  2576980376 =  2576980376.4 / 4294967294.0 =  0.6 ( 2576980376.4  2576980376=0.4  2576980378=1.6)
		 0.4, //  3:   0x33333333 =   858993459 <-  1717986918 =  1717986917.6 / 4294967294.0 =  0.4 ( 1717986917.6  1717986916=1.6  1717986918=0.4)
		 0.2, //  4:   0x19999999 =   429496729 <-   858993458 =   858993458.8 / 4294967294.0 =  0.2 (  858993458.8   858993458=0.8   858993460=1.2)
		-0.2, //  5:   0xE6666666 =  -429496730 <-  -858993460 =  -858993459.2 / 4294967296.0 = -0.2 ( -858993459.2  -858993458=1.2  -858993460=0.8)
		-0.4, //  6:   0xCCCCCCCD =  -858993459 <- -1717986918 = -1717986918.4 / 4294967296.0 = -0.4 (-1717986918.4 -1717986918=0.4 -1717986920=1.6)
		-0.6, //  7:   0xB3333333 = -1288490189 <- -2576980378 = -2576980377.6 / 4294967296.0 = -0.6 (-2576980377.6 -2576980376=1.6 -2576980378=0.4)
		-0.8, //  8:   0x9999999A = -1717986918 <- -3435973836 = -3435973836.8 / 4294967296.0 = -0.8 (-3435973836.8 -3435973836=0.8 -3435973838=1.2)
		-1.0, //  9:   0x80000000 = -2147483648 <- -4294967296 = -4294967296.0 / 4294967296.0 = -1.0
		 1.2, // 10:   0x7FFFFFFF =  2147483647 =  1.2
		-1.2  // 11:   0x80000000 = -2147483648 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamplesA[24] ={
		0x7f,0xff,0xff,0xff, // 0
		0x4c,0xcc,0xcc,0xcc, // 2
		0x19,0x99,0x99,0x99, // 4
		0xcc,0xcc,0xcc,0xcd, // 6
		0x99,0x99,0x99,0x9a, // 8
		0x7f,0xff,0xff,0xff  // 10
	};

	const tubyte c_expectIntegerSamplesB[24] ={
		0x66,0x66,0x66,0x66, // 1
		0x33,0x33,0x33,0x33, // 3
		0xe6,0x66,0x66,0x66, // 5
		0xb3,0x33,0x33,0x33, // 7
		0x80,0x00,0x00,0x00, // 9
		0x80,0x00,0x00,0x00  // 11
	};

	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt32MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,24));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,24));
}

TEST(ASIOData,sampleTo32BitMSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:   0x7FFFFFFF =  2147483647 <-  4294967294 =  4294967294.0 / 4294967294.0 =  1.0
		 0.8, //  1:   0x66666666 =  1717986918 <-  3435973836 =  3435973835.2 / 4294967294.0 =  0.8 ( 3435973835.2  3435973834=1.2  3435973836=0.8)
		 0.6, //  2:   0x4CCCCCCC =  1288490188 <-  2576980376 =  2576980376.4 / 4294967294.0 =  0.6 ( 2576980376.4  2576980376=0.4  2576980378=1.6)
		 0.4, //  3:   0x33333333 =   858993459 <-  1717986918 =  1717986917.6 / 4294967294.0 =  0.4 ( 1717986917.6  1717986916=1.6  1717986918=0.4)
		 0.2, //  4:   0x19999999 =   429496729 <-   858993458 =   858993458.8 / 4294967294.0 =  0.2 (  858993458.8   858993458=0.8   858993460=1.2)
		-0.2, //  5:   0xE6666666 =  -429496730 <-  -858993460 =  -858993459.2 / 4294967296.0 = -0.2 ( -858993459.2  -858993458=1.2  -858993460=0.8)
		-0.4, //  6:   0xCCCCCCCD =  -858993459 <- -1717986918 = -1717986918.4 / 4294967296.0 = -0.4 (-1717986918.4 -1717986918=0.4 -1717986920=1.6)
		-0.6, //  7:   0xB3333333 = -1288490189 <- -2576980378 = -2576980377.6 / 4294967296.0 = -0.6 (-2576980377.6 -2576980376=1.6 -2576980378=0.4)
		-0.8, //  8:   0x9999999A = -1717986918 <- -3435973836 = -3435973836.8 / 4294967296.0 = -0.8 (-3435973836.8 -3435973836=0.8 -3435973838=1.2)
		-1.0, //  9:   0x80000000 = -2147483648 <- -4294967296 = -4294967296.0 / 4294967296.0 = -1.0
		 1.2, // 10:   0x7FFFFFFF =  2147483647 =  1.2
		-1.2  // 11:   0x80000000 = -2147483648 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamplesA[12] ={
		0x7f,0xff,0xff,0xff, // 0
		0x19,0x99,0x99,0x99, // 4
		0x99,0x99,0x99,0x9a  // 8
	};

	const tubyte c_expectIntegerSamplesB[12] ={
		0x66,0x66,0x66,0x66, // 1
		0xe6,0x66,0x66,0x66, // 5
		0x80,0x00,0x00,0x00  // 9
	};

	const tubyte c_expectIntegerSamplesC[12] ={
		0x4c,0xcc,0xcc,0xcc, // 2
		0xcc,0xcc,0xcc,0xcd, // 6
		0x7f,0xff,0xff,0xff  // 10
	};

	const tubyte c_expectIntegerSamplesD[12] ={
		0x33,0x33,0x33,0x33, // 3
		0xb3,0x33,0x33,0x33, // 7
		0x80,0x00,0x00,0x00  // 11
	};

	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt32MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,12));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,12));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleToFloatLSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif
	
	const tfloat32 c_expectFloatSample[12] = {
		 1.0f, // 0
		 0.8f, // 1
		 0.6f, // 2
		 0.4f, // 3
		 0.2f, // 4
		-1.0f, // 5
		-0.8f, // 6
		-0.6f, // 7
		-0.4f, // 8
		-0.2f, // 9
		 1.0f, // 10
		-1.0f  // 11
	};
	
	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTFloat32LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	tfloat32 outData[12];
	engine::writeLittleEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outData),12);
	for(int i=0;i<12;i++)
	{
		EXPECT_FLOAT_EQ(c_expectFloatSample[i],outData[i]);
	}
}

TEST(ASIOData,sampleToFloatLSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif

	const tfloat32 c_expectFloatSampleA[6] = {
		 1.0f, // 0
		 0.6f, // 2
		 0.2f, // 4
		-0.8f, // 6
		-0.4f, // 8
		 1.0f  // 10
	};

	const tfloat32 c_expectFloatSampleB[6] = {
		 0.8f, // 1
		 0.4f, // 3
		-1.0f, // 5
		-0.6f, // 7
		-0.2f, // 9
		-1.0f  // 11
	};
	
	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTFloat32LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	tfloat32 outDataA[6];
	tfloat32 outDataB[6];
	engine::writeLittleEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outDataA),6);
	engine::writeLittleEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(1,0)),reinterpret_cast<tbyte *>(outDataB),6);
	for(int i=0;i<6;i++)
	{
		EXPECT_FLOAT_EQ(c_expectFloatSampleA[i],outDataA[i]);
		EXPECT_FLOAT_EQ(c_expectFloatSampleB[i],outDataB[i]);
	}
}

TEST(ASIOData,sampleToFloatLSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif

	const tfloat32 c_expectFloatSampleA[3] = {
		 1.0f, // 0
		 0.2f, // 4
		-0.4f  // 8
	};
	
	const tfloat32 c_expectFloatSampleB[3] = {
		 0.8f, // 1
		-1.0f, // 5
		-0.2f  // 9
	};
	
	const tfloat32 c_expectFloatSampleC[3] = {
		 0.6f, // 2
		-0.8f, // 6
		 1.0f  // 10
	};
	
	const tfloat32 c_expectFloatSampleD[3] = {
		 0.4f, // 3
		-0.6f, // 7
		-1.0f  // 11
	};
	
	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTFloat32LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	tfloat32 outDataA[3];
	tfloat32 outDataB[3];
	tfloat32 outDataC[3];
	tfloat32 outDataD[3];
	engine::writeLittleEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outDataA),3);
	engine::writeLittleEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(1,0)),reinterpret_cast<tbyte *>(outDataB),3);
	engine::writeLittleEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(2,0)),reinterpret_cast<tbyte *>(outDataC),3);
	engine::writeLittleEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(3,0)),reinterpret_cast<tbyte *>(outDataD),3);
	for(int i=0;i<3;i++)
	{
		EXPECT_FLOAT_EQ(c_expectFloatSampleA[i],outDataA[i]);
		EXPECT_FLOAT_EQ(c_expectFloatSampleB[i],outDataB[i]);
		EXPECT_FLOAT_EQ(c_expectFloatSampleC[i],outDataC[i]);
		EXPECT_FLOAT_EQ(c_expectFloatSampleD[i],outDataD[i]);
	}
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleToFloatMSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif
	
	const tfloat32 c_expectFloatSample[12] = {
		 1.0f, // 0
		 0.8f, // 1
		 0.6f, // 2
		 0.4f, // 3
		 0.2f, // 4
		-1.0f, // 5
		-0.8f, // 6
		-0.6f, // 7
		-0.4f, // 8
		-0.2f, // 9
		 1.0f, // 10
		-1.0f  // 11
	};
	
	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTFloat32MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	tfloat32 outData[12];
	engine::writeBigEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outData),12);
	for(int i=0;i<12;i++)
	{
		EXPECT_FLOAT_EQ(c_expectFloatSample[i],outData[i]);
	}
}

TEST(ASIOData,sampleToFloatMSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif

	const tfloat32 c_expectFloatSampleA[6] = {
		 1.0f, // 0
		 0.6f, // 2
		 0.2f, // 4
		-0.8f, // 6
		-0.4f, // 8
		 1.0f  // 10
	};

	const tfloat32 c_expectFloatSampleB[6] = {
		 0.8f, // 1
		 0.4f, // 3
		-1.0f, // 5
		-0.6f, // 7
		-0.2f, // 9
		-1.0f  // 11
	};
	
	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTFloat32MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	tfloat32 outDataA[6];
	tfloat32 outDataB[6];
	engine::writeBigEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outDataA),6);
	engine::writeBigEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(1,0)),reinterpret_cast<tbyte *>(outDataB),6);
	for(int i=0;i<6;i++)
	{
		EXPECT_FLOAT_EQ(c_expectFloatSampleA[i],outDataA[i]);
		EXPECT_FLOAT_EQ(c_expectFloatSampleB[i],outDataB[i]);
	}
}

TEST(ASIOData,sampleToFloatMSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif

	const tfloat32 c_expectFloatSampleA[3] = {
		 1.0f, // 0
		 0.2f, // 4
		-0.4f  // 8
	};
	
	const tfloat32 c_expectFloatSampleB[3] = {
		 0.8f, // 1
		-1.0f, // 5
		-0.2f  // 9
	};
	
	const tfloat32 c_expectFloatSampleC[3] = {
		 0.6f, // 2
		-0.8f, // 6
		 1.0f  // 10
	};
	
	const tfloat32 c_expectFloatSampleD[3] = {
		 0.4f, // 3
		-0.6f, // 7
		-1.0f  // 11
	};
	
	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTFloat32MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	tfloat32 outDataA[3];
	tfloat32 outDataB[3];
	tfloat32 outDataC[3];
	tfloat32 outDataD[3];
	engine::writeBigEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outDataA),3);
	engine::writeBigEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(1,0)),reinterpret_cast<tbyte *>(outDataB),3);
	engine::writeBigEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(2,0)),reinterpret_cast<tbyte *>(outDataC),3);
	engine::writeBigEndian32BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(3,0)),reinterpret_cast<tbyte *>(outDataD),3);
	for(int i=0;i<3;i++)
	{
		EXPECT_FLOAT_EQ(c_expectFloatSampleA[i],outDataA[i]);
		EXPECT_FLOAT_EQ(c_expectFloatSampleB[i],outDataB[i]);
		EXPECT_FLOAT_EQ(c_expectFloatSampleC[i],outDataC[i]);
		EXPECT_FLOAT_EQ(c_expectFloatSampleD[i],outDataD[i]);
	}
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleToDoubleLSBConvertionMono)
{
	static tfloat64 c_Tolerance = 0.0000001;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif
	
	const tfloat64 c_expectFloatSample[12] = {
		 1.0, // 0
		 0.8, // 1
		 0.6, // 2
		 0.4, // 3
		 0.2, // 4
		-1.0, // 5
		-0.8, // 6
		-0.6, // 7
		-0.4, // 8
		-0.2, // 9
		 1.0, // 10
		-1.0  // 11
	};
	
	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTFloat64LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	tfloat64 outData[12];
	engine::writeLittleEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outData),12);
	for(int i=0;i<12;i++)
	{
		EXPECT_NEAR(c_expectFloatSample[i],outData[i],c_Tolerance);
	}
}

TEST(ASIOData,sampleToDoubleLSBConvertionStereo)
{
	static tfloat64 c_Tolerance = 0.0000001;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif
	
	const tfloat64 c_expectFloatSampleA[6] = {
		 1.0, // 0
		 0.6, // 2
		 0.2, // 4
		-0.8, // 6
		-0.4, // 8
		 1.0  // 10
	};

	const tfloat64 c_expectFloatSampleB[6] = {
		 0.8, // 1
		 0.4, // 3
		-1.0, // 5
		-0.6, // 7
		-0.2, // 9
		-1.0  // 11
	};
	
	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTFloat64LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	tfloat64 outDataA[6];
	tfloat64 outDataB[6];
	engine::writeLittleEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outDataA),6);
	engine::writeLittleEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(1,0)),reinterpret_cast<tbyte *>(outDataB),6);
	for(int i=0;i<6;i++)
	{
		EXPECT_NEAR(c_expectFloatSampleA[i],outDataA[i],c_Tolerance);
		EXPECT_NEAR(c_expectFloatSampleB[i],outDataB[i],c_Tolerance);
	}
}

TEST(ASIOData,sampleToDoubleLSBConvertionQuadrophonic)
{
	static tfloat64 c_Tolerance = 0.0000001;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif
	
	const tfloat64 c_expectFloatSampleA[3] = {
		 1.0, // 0
		 0.2, // 4
		-0.4  // 8
	};
	
	const tfloat64 c_expectFloatSampleB[3] = {
		 0.8, // 1
		-1.0, // 5
		-0.2  // 9
	};
	
	const tfloat64 c_expectFloatSampleC[3] = {
		 0.6, // 2
		-0.8, // 6
		 1.0  // 10
	};
	
	const tfloat64 c_expectFloatSampleD[3] = {
		 0.4, // 3
		-0.6, // 7
		-1.0  // 11
	};
	
	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTFloat64LSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	tfloat64 outDataA[3];
	tfloat64 outDataB[3];
	tfloat64 outDataC[3];
	tfloat64 outDataD[3];
	engine::writeLittleEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outDataA),3);
	engine::writeLittleEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(1,0)),reinterpret_cast<tbyte *>(outDataB),3);
	engine::writeLittleEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(2,0)),reinterpret_cast<tbyte *>(outDataC),3);
	engine::writeLittleEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(3,0)),reinterpret_cast<tbyte *>(outDataD),3);
	for(int i=0;i<3;i++)
	{
		EXPECT_NEAR(c_expectFloatSampleA[i],outDataA[i],c_Tolerance);
		EXPECT_NEAR(c_expectFloatSampleB[i],outDataB[i],c_Tolerance);
		EXPECT_NEAR(c_expectFloatSampleC[i],outDataC[i],c_Tolerance);
		EXPECT_NEAR(c_expectFloatSampleD[i],outDataD[i],c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleToDoubleMSBConvertionMono)
{
	static tfloat64 c_Tolerance = 0.0000001;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif
	
	const tfloat64 c_expectFloatSample[12] = {
		 1.0, // 0
		 0.8, // 1
		 0.6, // 2
		 0.4, // 3
		 0.2, // 4
		-1.0, // 5
		-0.8, // 6
		-0.6, // 7
		-0.4, // 8
		-0.2, // 9
		 1.0, // 10
		-1.0  // 11
	};
	
	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTFloat64MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	tfloat64 outData[12];
	engine::writeBigEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outData),12);
	for(int i=0;i<12;i++)
	{
		EXPECT_NEAR(c_expectFloatSample[i],outData[i],c_Tolerance);
	}
}

TEST(ASIOData,sampleToDoubleMSBConvertionStereo)
{
	static tfloat64 c_Tolerance = 0.0000001;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif
	
	const tfloat64 c_expectFloatSampleA[6] = {
		 1.0, // 0
		 0.6, // 2
		 0.2, // 4
		-0.8, // 6
		-0.4, // 8
		 1.0  // 10
	};

	const tfloat64 c_expectFloatSampleB[6] = {
		 0.8, // 1
		 0.4, // 3
		-1.0, // 5
		-0.6, // 7
		-0.2, // 9
		-1.0  // 11
	};
	
	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTFloat64MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	tfloat64 outDataA[6];
	tfloat64 outDataB[6];
	engine::writeBigEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outDataA),6);
	engine::writeBigEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(1,0)),reinterpret_cast<tbyte *>(outDataB),6);
	for(int i=0;i<6;i++)
	{
		EXPECT_NEAR(c_expectFloatSampleA[i],outDataA[i],c_Tolerance);
		EXPECT_NEAR(c_expectFloatSampleB[i],outDataB[i],c_Tolerance);
	}
}

TEST(ASIOData,sampleToDoubleMSBConvertionQuadrophonic)
{
	static tfloat64 c_Tolerance = 0.0000001;

#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0,  0.8,  0.6,  0.4,  0.2,
		-1.0, -0.8, -0.6, -0.4, -0.2,
		 1.2, -1.2
	};
#endif
	
	const tfloat64 c_expectFloatSampleA[3] = {
		 1.0, // 0
		 0.2, // 4
		-0.4  // 8
	};
	
	const tfloat64 c_expectFloatSampleB[3] = {
		 0.8, // 1
		-1.0, // 5
		-0.2  // 9
	};
	
	const tfloat64 c_expectFloatSampleC[3] = {
		 0.6, // 2
		-0.8, // 6
		 1.0  // 10
	};
	
	const tfloat64 c_expectFloatSampleD[3] = {
		 0.4, // 3
		-0.6, // 7
		-1.0  // 11
	};
	
	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTFloat64MSB);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	tfloat64 outDataA[3];
	tfloat64 outDataB[3];
	tfloat64 outDataC[3];
	tfloat64 outDataD[3];
	engine::writeBigEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(0,0)),reinterpret_cast<tbyte *>(outDataA),3);
	engine::writeBigEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(1,0)),reinterpret_cast<tbyte *>(outDataB),3);
	engine::writeBigEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(2,0)),reinterpret_cast<tbyte *>(outDataC),3);
	engine::writeBigEndian64BitsAsNative(reinterpret_cast<const tbyte *>(data.asioData(3,0)),reinterpret_cast<tbyte *>(outDataD),3);
	for(int i=0;i<3;i++)
	{
		EXPECT_NEAR(c_expectFloatSampleA[i],outDataA[i],c_Tolerance);
		EXPECT_NEAR(c_expectFloatSampleB[i],outDataB[i],c_Tolerance);
		EXPECT_NEAR(c_expectFloatSampleC[i],outDataC[i],c_Tolerance);
		EXPECT_NEAR(c_expectFloatSampleD[i],outDataD[i],c_Tolerance);
	}
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo16BitIn32BitLSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamples[48] = {
		0xff, 0x7f, 0x00, 0x00, // 0
		0x66, 0x66, 0x00, 0x00, // 1
		0xcc, 0x4c, 0x00, 0x00, // 2
		0x33, 0x33, 0x00, 0x00, // 3
		0x99, 0x19, 0x00, 0x00, // 4
		0x66, 0xe6, 0xff, 0xff, // 5
		0xcd, 0xcc, 0xff, 0xff, // 6
		0x33, 0xb3, 0xff, 0xff, // 7
		0x9a, 0x99, 0xff, 0xff, // 8
		0x00, 0x80, 0xff, 0xff, // 9
		0xff, 0x7f, 0x00, 0x00, // 10
		0x00, 0x80, 0xff, 0xff  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt32LSB16);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,48));
}

TEST(ASIOData,sampleTo16BitIn32BitLSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[24] = {
		0xff, 0x7f, 0x00, 0x00, // 0
		0xcc, 0x4c, 0x00, 0x00, // 2
		0x99, 0x19, 0x00, 0x00, // 4
		0xcd, 0xcc, 0xff, 0xff, // 6
		0x9a, 0x99, 0xff, 0xff, // 8
		0xff, 0x7f, 0x00, 0x00, // 10
	};

	const tubyte c_expectIntegerSamplesB[24] = {
		0x66, 0x66, 0x00, 0x00, // 1
		0x33, 0x33, 0x00, 0x00, // 3
		0x66, 0xe6, 0xff, 0xff, // 5
		0x33, 0xb3, 0xff, 0xff, // 7
		0x00, 0x80, 0xff, 0xff, // 9
		0x00, 0x80, 0xff, 0xff  // 11
	};

	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt32LSB16);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,24));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,24));
}

TEST(ASIOData,sampleTo16BitIn32BitLSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[12] = {
		0xff, 0x7f, 0x00, 0x00, // 0
		0x99, 0x19, 0x00, 0x00, // 4
		0x9a, 0x99, 0xff, 0xff  // 8
	};

	const tubyte c_expectIntegerSamplesB[12] = {
		0x66, 0x66, 0x00, 0x00, // 1
		0x66, 0xe6, 0xff, 0xff, // 5
		0x00, 0x80, 0xff, 0xff  // 9
	};
	
	const tubyte c_expectIntegerSamplesC[12] = {
		0xcc, 0x4c, 0x00, 0x00, // 2
		0xcd, 0xcc, 0xff, 0xff, // 6
		0xff, 0x7f, 0x00, 0x00  // 10
	};
	
	const tubyte c_expectIntegerSamplesD[12] = {
		0x33, 0x33, 0x00, 0x00, // 3
		0x33, 0xb3, 0xff, 0xff, // 7
		0x00, 0x80, 0xff, 0xff  // 11
	};

	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt32LSB16);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,12));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,12));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo16BitIn32BitMSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamples[48] = {
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

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt32MSB16);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,48));
}

TEST(ASIOData,sampleTo16BitIn32BitMSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[24] = {
		0x00, 0x00, 0x7f, 0xff, // 0
		0x00, 0x00, 0x4c, 0xcc, // 2
		0x00, 0x00, 0x19, 0x99, // 4
		0xff, 0xff, 0xcc, 0xcd, // 6
		0xff, 0xff, 0x99, 0x9a, // 8
		0x00, 0x00, 0x7f, 0xff  // 10
	};
	
	const tubyte c_expectIntegerSamplesB[24] = {
		0x00, 0x00, 0x66, 0x66, // 1
		0x00, 0x00, 0x33, 0x33, // 3
		0xff, 0xff, 0xe6, 0x66, // 5
		0xff, 0xff, 0xb3, 0x33, // 7
		0xff, 0xff, 0x80, 0x00, // 9
		0xff, 0xff, 0x80, 0x00  // 11
	};

	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt32MSB16);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,24));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,24));
}

TEST(ASIOData,sampleTo16BitIn32BitMSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7fff =  32767 <-  65534 =  65534.0 / 65534.0 =  1.0
		 0.8, //  1:  0x6666 =  26214 <-  52428 =  52427.2 / 65534.0 =  0.8 ( 52427.2  52426=1.2  52428=0.8)
		 0.6, //  2:  0x4ccc =  19660 <-  39320 =  39320.4 / 65534.0 =  0.6 ( 39320.4  39320=0.4  39322=1.6)
		 0.4, //  3:  0x3333 =  13107 <-  26214 =  26213.6 / 65534.0 =  0.4 ( 26213.6  26212=1.6  26214=0.4)
		 0.2, //  4:  0x1999 =   6553 <-  13106 =  13106.8 / 65534.0 =  0.2 ( 13106.8  13106=0.8  13108=1.2)
		-0.2, //  5:  0xe666 =  -6554 <- -13108 = -13107.2 / 65536.0 = -0.2 (-13107.2 -13108=0.8 -13106=1.2)
		-0.4, //  6:  0xcccd = -13107 <- -26214 = -26214.4 / 65536.0 = -0.4 (-26214.4 -26214=0.4 -26216=1.6)
		-0.6, //  7:  0xb333 = -19661 <- -39322 = -39321.6 / 65536.0 = -0.6 (-39321.6 -39320=1.6 -39322=0.4)
		-0.8, //  8:  0x999a = -26214 <- -52428 = -52428.8 / 65536.0 = -0.8 (-52428.8 -52428=0.8 -52430=1.2)
		-1.0, //  9:  0x8000 = -32768 <- -65536 = -65536.0 / 65536.0 = -1.0
		 1.2, // 10:  0x7fff =  32767 =  1.2
		-1.2  // 11:  0x8000 = -32768 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[12] = {
		0x00, 0x00, 0x7f, 0xff, // 0
		0x00, 0x00, 0x19, 0x99, // 4
		0xff, 0xff, 0x99, 0x9a  // 8
	};

	const tubyte c_expectIntegerSamplesB[12] = {
		0x00, 0x00, 0x66, 0x66, // 1
		0xff, 0xff, 0xe6, 0x66, // 5
		0xff, 0xff, 0x80, 0x00  // 9
	};

	const tubyte c_expectIntegerSamplesC[12] = {
		0x00, 0x00, 0x4c, 0xcc, // 2
		0xff, 0xff, 0xcc, 0xcd, // 6
		0x00, 0x00, 0x7f, 0xff  // 10
	};

	const tubyte c_expectIntegerSamplesD[12] = {
		0x00, 0x00, 0x33, 0x33, // 3
		0xff, 0xff, 0xb3, 0x33, // 7
		0xff, 0xff, 0x80, 0x00  // 11
	};


	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt32MSB16);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,12));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,12));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo18BitIn32BitLSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: 0x0003FFFF =  262143 =  524286 =  524286.0 / 524286.0 =  1.0
		 0.8, //  1: 0x00033332 =  209714 =  419428 =  419428.8 / 524286.0 =  0.8 (  419428.8  419428=0.8  419430=1.2 )
		 0.6, //  2: 0x00026666 =  157286 =  314572 =  314571.6 / 524286.0 =  0.6 (  314571.6  314570=1.6  314572=0.4 )
		 0.4, //  3: 0x00019999 =  104857 =  209714 =  209714.4 / 524286.0 =  0.4 (  209714.4  209714=0.4  209716=1.6 )
		 0.2, //  4: 0x0000CCCD =   52429 =  104858 =  104857.2 / 524286.0 =  0.2 (  104857.2  104856=1.2  104858=0.8 )
		-0.2, //  5: 0xFFFF3333 =  -52429 = -104858 = -104857.6 / 524288.0 = -0.2 ( -104857.6 -104856=1.6 -104858=0.4 )
		-0.4, //  6: 0xFFFE6666 = -104858 = -209716 = -209715.2 / 524288.0 = -0.4 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.6, //  7: 0xFFFD999A = -157286 = -314572 = -314572.8 / 524288.0 = -0.6 ( -314572.8 -314572=0.8 -314574=1.2 )
		-0.8, //  8: 0xFFFCCCCD = -209715 = -419430 = -419430.4 / 524288.0 = -0.8 ( -419430.4 -419430=0.4 -419432=1.6 )
		-1.0, //  9: 0xFFFC0000 = -262144 = -524288 = -524288.0 / 524288.0 = -1.0
		 1.2, // 10: 0x0003FFFF =  1.2
		-1.2  // 11: 0xFFFC0000 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamples[48] = {
		0xff, 0xff, 0x01, 0x00, // 0
		0x99, 0x99, 0x01, 0x00, // 1
		0x33, 0x33, 0x01, 0x00, // 2
		0xcc, 0xcc, 0x00, 0x00, // 3
		0x66, 0x66, 0x00, 0x00, // 4
		0x9a, 0x99, 0xff, 0xff, // 5
		0x33, 0x33, 0xff, 0xff, // 6
		0xcd, 0xcc, 0xfe, 0xff, // 7
		0x66, 0x66, 0xfe, 0xff, // 8
		0x00, 0x00, 0xfe, 0xff, // 9
		0xff, 0xff, 0x01, 0x00, // 10
		0x00, 0x00, 0xfe, 0xff  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt32LSB18);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,48));
}

TEST(ASIOData,sampleTo18BitIn32BitLSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: 0x0003FFFF =  262143 =  524286 =  524286.0 / 524286.0 =  1.0
		 0.8, //  1: 0x00033332 =  209714 =  419428 =  419428.8 / 524286.0 =  0.8 (  419428.8  419428=0.8  419430=1.2 )
		 0.6, //  2: 0x00026666 =  157286 =  314572 =  314571.6 / 524286.0 =  0.6 (  314571.6  314570=1.6  314572=0.4 )
		 0.4, //  3: 0x00019999 =  104857 =  209714 =  209714.4 / 524286.0 =  0.4 (  209714.4  209714=0.4  209716=1.6 )
		 0.2, //  4: 0x0000CCCD =   52429 =  104858 =  104857.2 / 524286.0 =  0.2 (  104857.2  104856=1.2  104858=0.8 )
		-0.2, //  5: 0xFFFF3333 =  -52429 = -104858 = -104857.6 / 524288.0 = -0.2 ( -104857.6 -104856=1.6 -104858=0.4 )
		-0.4, //  6: 0xFFFE6666 = -104858 = -209716 = -209715.2 / 524288.0 = -0.4 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.6, //  7: 0xFFFD999A = -157286 = -314572 = -314572.8 / 524288.0 = -0.6 ( -314572.8 -314572=0.8 -314574=1.2 )
		-0.8, //  8: 0xFFFCCCCD = -209715 = -419430 = -419430.4 / 524288.0 = -0.8 ( -419430.4 -419430=0.4 -419432=1.6 )
		-1.0, //  9: 0xFFFC0000 = -262144 = -524288 = -524288.0 / 524288.0 = -1.0
		 1.2, // 10: 0x0003FFFF =  1.2
		-1.2  // 11: 0xFFFC0000 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamplesA[24] ={
		0xff,0xff,0x01,0x00, // 0
		0x33,0x33,0x01,0x00, // 2
		0x66,0x66,0x00,0x00, // 4
		0x33,0x33,0xff,0xff, // 6
		0x66,0x66,0xfe,0xff, // 8
		0xff,0xff,0x01,0x00, // 10
	};

	const tubyte c_expectIntegerSamplesB[24] ={
		0x99,0x99,0x01,0x00, // 1
		0xcc,0xcc,0x00,0x00, // 3
		0x9a,0x99,0xff,0xff, // 5
		0xcd,0xcc,0xfe,0xff, // 7
		0x00,0x00,0xfe,0xff, // 9
		0x00,0x00,0xfe,0xff  // 11
	};
	
	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt32LSB18);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,24));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,24));
}

TEST(ASIOData,sampleTo18BitIn32BitLSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: 0x0003FFFF =  262143 =  524286 =  524286.0 / 524286.0 =  1.0
		 0.8, //  1: 0x00033332 =  209714 =  419428 =  419428.8 / 524286.0 =  0.8 (  419428.8  419428=0.8  419430=1.2 )
		 0.6, //  2: 0x00026666 =  157286 =  314572 =  314571.6 / 524286.0 =  0.6 (  314571.6  314570=1.6  314572=0.4 )
		 0.4, //  3: 0x00019999 =  104857 =  209714 =  209714.4 / 524286.0 =  0.4 (  209714.4  209714=0.4  209716=1.6 )
		 0.2, //  4: 0x0000CCCD =   52429 =  104858 =  104857.2 / 524286.0 =  0.2 (  104857.2  104856=1.2  104858=0.8 )
		-0.2, //  5: 0xFFFF3333 =  -52429 = -104858 = -104857.6 / 524288.0 = -0.2 ( -104857.6 -104856=1.6 -104858=0.4 )
		-0.4, //  6: 0xFFFE6666 = -104858 = -209716 = -209715.2 / 524288.0 = -0.4 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.6, //  7: 0xFFFD999A = -157286 = -314572 = -314572.8 / 524288.0 = -0.6 ( -314572.8 -314572=0.8 -314574=1.2 )
		-0.8, //  8: 0xFFFCCCCD = -209715 = -419430 = -419430.4 / 524288.0 = -0.8 ( -419430.4 -419430=0.4 -419432=1.6 )
		-1.0, //  9: 0xFFFC0000 = -262144 = -524288 = -524288.0 / 524288.0 = -1.0
		 1.2, // 10: 0x0003FFFF =  1.2
		-1.2  // 11: 0xFFFC0000 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamplesA[12] ={
		0xff,0xff,0x01,0x00, // 0
		0x66,0x66,0x00,0x00, // 4
		0x66,0x66,0xfe,0xff  // 8
	};

	const tubyte c_expectIntegerSamplesB[12] ={
		0x99,0x99,0x01,0x00, // 1
		0x9a,0x99,0xff,0xff, // 5
		0x00,0x00,0xfe,0xff  // 9
	};

	const tubyte c_expectIntegerSamplesC[12] ={
		0x33,0x33,0x01,0x00, // 2
		0x33,0x33,0xff,0xff, // 6
		0xff,0xff,0x01,0x00  // 10
	};

	const tubyte c_expectIntegerSamplesD[12] ={
		0xcc,0xcc,0x00,0x00, // 3
		0xcd,0xcc,0xfe,0xff, // 7
		0x00,0x00,0xfe,0xff  // 11
	};

	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt32LSB18);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,12));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,12));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo18BitIn32BitMSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: 0x0003FFFF =  262143 =  524286 =  524286.0 / 524286.0 =  1.0
		 0.8, //  1: 0x00033332 =  209714 =  419428 =  419428.8 / 524286.0 =  0.8 (  419428.8  419428=0.8  419430=1.2 )
		 0.6, //  2: 0x00026666 =  157286 =  314572 =  314571.6 / 524286.0 =  0.6 (  314571.6  314570=1.6  314572=0.4 )
		 0.4, //  3: 0x00019999 =  104857 =  209714 =  209714.4 / 524286.0 =  0.4 (  209714.4  209714=0.4  209716=1.6 )
		 0.2, //  4: 0x0000CCCD =   52429 =  104858 =  104857.2 / 524286.0 =  0.2 (  104857.2  104856=1.2  104858=0.8 )
		-0.2, //  5: 0xFFFF3333 =  -52429 = -104858 = -104857.6 / 524288.0 = -0.2 ( -104857.6 -104856=1.6 -104858=0.4 )
		-0.4, //  6: 0xFFFE6666 = -104858 = -209716 = -209715.2 / 524288.0 = -0.4 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.6, //  7: 0xFFFD999A = -157286 = -314572 = -314572.8 / 524288.0 = -0.6 ( -314572.8 -314572=0.8 -314574=1.2 )
		-0.8, //  8: 0xFFFCCCCD = -209715 = -419430 = -419430.4 / 524288.0 = -0.8 ( -419430.4 -419430=0.4 -419432=1.6 )
		-1.0, //  9: 0xFFFC0000 = -262144 = -524288 = -524288.0 / 524288.0 = -1.0
		 1.2, // 10: 0x0003FFFF =  1.2
		-1.2  // 11: 0xFFFC0000 = -1.2
	};
#endif

	const tubyte c_expectIntegerSamples[48] = {
		0x00, 0x01, 0xff, 0xff, // 0
		0x00, 0x01, 0x99, 0x99, // 1
		0x00, 0x01, 0x33, 0x33, // 2
		0x00, 0x00, 0xcc, 0xcc, // 3
		0x00, 0x00, 0x66, 0x66, // 4
		0xff, 0xff, 0x99, 0x9a, // 5
		0xff, 0xff, 0x33, 0x33, // 6
		0xff, 0xfe, 0xcc, 0xcd, // 7
		0xff, 0xfe, 0x66, 0x66, // 8
		0xff, 0xfe, 0x00, 0x00, // 9
		0x00, 0x01, 0xff, 0xff, // 10
		0xff, 0xfe, 0x00, 0x00  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt32MSB18);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,48));
}

TEST(ASIOData,sampleTo18BitIn32BitMSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: 0x0003FFFF =  262143 =  524286 =  524286.0 / 524286.0 =  1.0
		 0.8, //  1: 0x00033332 =  209714 =  419428 =  419428.8 / 524286.0 =  0.8 (  419428.8  419428=0.8  419430=1.2 )
		 0.6, //  2: 0x00026666 =  157286 =  314572 =  314571.6 / 524286.0 =  0.6 (  314571.6  314570=1.6  314572=0.4 )
		 0.4, //  3: 0x00019999 =  104857 =  209714 =  209714.4 / 524286.0 =  0.4 (  209714.4  209714=0.4  209716=1.6 )
		 0.2, //  4: 0x0000CCCD =   52429 =  104858 =  104857.2 / 524286.0 =  0.2 (  104857.2  104856=1.2  104858=0.8 )
		-0.2, //  5: 0xFFFF3333 =  -52429 = -104858 = -104857.6 / 524288.0 = -0.2 ( -104857.6 -104856=1.6 -104858=0.4 )
		-0.4, //  6: 0xFFFE6666 = -104858 = -209716 = -209715.2 / 524288.0 = -0.4 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.6, //  7: 0xFFFD999A = -157286 = -314572 = -314572.8 / 524288.0 = -0.6 ( -314572.8 -314572=0.8 -314574=1.2 )
		-0.8, //  8: 0xFFFCCCCD = -209715 = -419430 = -419430.4 / 524288.0 = -0.8 ( -419430.4 -419430=0.4 -419432=1.6 )
		-1.0, //  9: 0xFFFC0000 = -262144 = -524288 = -524288.0 / 524288.0 = -1.0
		 1.2, // 10: 0x0003FFFF =  1.2
		-1.2  // 11: 0xFFFC0000 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[24] ={
		0x00,0x01,0xff,0xff, // 0
		0x00,0x01,0x33,0x33, // 2
		0x00,0x00,0x66,0x66, // 4
		0xff,0xff,0x33,0x33, // 6
		0xff,0xfe,0x66,0x66, // 8
		0x00,0x01,0xff,0xff  // 10
	};

	const tubyte c_expectIntegerSamplesB[24] ={
		0x00,0x01,0x99,0x99, // 1
		0x00,0x00,0xcc,0xcc, // 3
		0xff,0xff,0x99,0x9a, // 5
		0xff,0xfe,0xcc,0xcd, // 7
		0xff,0xfe,0x00,0x00, // 9
		0xff,0xfe,0x00,0x00  // 11
	};

	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt32MSB18);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,24));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,24));
}

TEST(ASIOData,sampleTo18BitIn32BitMSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: 0x0003FFFF =  262143 =  524286 =  524286.0 / 524286.0 =  1.0
		 0.8, //  1: 0x00033332 =  209714 =  419428 =  419428.8 / 524286.0 =  0.8 (  419428.8  419428=0.8  419430=1.2 )
		 0.6, //  2: 0x00026666 =  157286 =  314572 =  314571.6 / 524286.0 =  0.6 (  314571.6  314570=1.6  314572=0.4 )
		 0.4, //  3: 0x00019999 =  104857 =  209714 =  209714.4 / 524286.0 =  0.4 (  209714.4  209714=0.4  209716=1.6 )
		 0.2, //  4: 0x0000CCCD =   52429 =  104858 =  104857.2 / 524286.0 =  0.2 (  104857.2  104856=1.2  104858=0.8 )
		-0.2, //  5: 0xFFFF3333 =  -52429 = -104858 = -104857.6 / 524288.0 = -0.2 ( -104857.6 -104856=1.6 -104858=0.4 )
		-0.4, //  6: 0xFFFE6666 = -104858 = -209716 = -209715.2 / 524288.0 = -0.4 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.6, //  7: 0xFFFD999A = -157286 = -314572 = -314572.8 / 524288.0 = -0.6 ( -314572.8 -314572=0.8 -314574=1.2 )
		-0.8, //  8: 0xFFFCCCCD = -209715 = -419430 = -419430.4 / 524288.0 = -0.8 ( -419430.4 -419430=0.4 -419432=1.6 )
		-1.0, //  9: 0xFFFC0000 = -262144 = -524288 = -524288.0 / 524288.0 = -1.0
		 1.2, // 10: 0x0003FFFF =  1.2
		-1.2  // 11: 0xFFFC0000 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[12] ={
		0x00,0x01,0xff,0xff, // 0
		0x00,0x00,0x66,0x66, // 4
		0xff,0xfe,0x66,0x66  // 8
	};

	const tubyte c_expectIntegerSamplesB[12] ={
		0x00,0x01,0x99,0x99, // 1
		0xff,0xff,0x99,0x9a, // 5
		0xff,0xfe,0x00,0x00  // 9
	};

	const tubyte c_expectIntegerSamplesC[12] ={
		0x00,0x01,0x33,0x33, // 2
		0xff,0xff,0x33,0x33, // 6
		0x00,0x01,0xff,0xff  // 10
	};

	const tubyte c_expectIntegerSamplesD[12] ={
		0x00,0x00,0xcc,0xcc, // 3
		0xff,0xfe,0xcc,0xcd, // 7
		0xff,0xfe,0x00,0x00  // 11
	};
	
	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt32MSB18);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,12));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,12));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo20BitIn32BitLSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: = 0x0007FFFF =  524287 =  1048574 =  1048574.0 / 1048574.0 =  1.0
		 0.8, //  1: = 0x00066666 =  419430 =   838860 =   838859.2 / 1048574.0 =  0.8 (  838859.2  838858=1.2  838860=0.8 )
		 0.6, //  2: = 0x0004CCCC =  314572 =   629144 =   629144.4 / 1048574.0 =  0.6 (  629144.4  629144=0.4  629146=1.6 )
		 0.4, //  3: = 0x00033333 =  209715 =   419430 =   419429.6 / 1048574.0 =  0.4 (  419429.6  419428=1.6  419430=0.4 )
		 0.2, //  4: = 0x00019999 =  104857 =   209714 =   209714.8 / 1048574.0 =  0.2 (  209714.8  209714=0.8  209716=1.2 )
		-0.2, //  5: = 0xFFFE6666 = -104858 =  -209716 =  -209715.2 / 1048576.0 = -0.2 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.4, //  6: = 0xFFFCCCCD = -209715 =  -419430 =  -419430.4 / 1048576.0 = -0.4 ( -419430.4 -419430=0.4 -419432=1.6 )
		-0.6, //  7: = 0xFFFB3333 = -314573 =  -629146 =  -629145.6 / 1048576.0 = -0.6 ( -629145.6 -629144=1.6 -629146=0.4 )
		-0.8, //  8: = 0xFFF9999A = -419430 =  -838860 =  -838860.8 / 1048576.0 = -0.8 ( -838860.8 -838860=0.8 -838862=1.2 )
		-1.0, //  9: = 0xFFF80000 = -524288 = -1048576 = -1048576.0 / 1048576.0 = -1.0
		 1.2, // 10: = 0x0007FFFF =  1.2
		-1.2  // 11: = 0xFFF80000 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamples[48] = {
		0xff, 0xff, 0x07, 0x00, // 0
		0x66, 0x66, 0x06, 0x00, // 1
		0xcc, 0xcc, 0x04, 0x00, // 2
		0x33, 0x33, 0x03, 0x00, // 3
		0x99, 0x99, 0x01, 0x00, // 4
		0x66, 0x66, 0xfe, 0xff, // 5
		0xcd, 0xcc, 0xfc, 0xff, // 6
		0x33, 0x33, 0xfb, 0xff, // 7
		0x9a, 0x99, 0xf9, 0xff, // 8
		0x00, 0x00, 0xf8, 0xff, // 9
		0xff, 0xff, 0x07, 0x00, // 10
		0x00, 0x00, 0xf8, 0xff  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt32LSB20);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,48));
}

TEST(ASIOData,sampleTo20BitIn32BitLSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: = 0x0007FFFF =  524287 =  1048574 =  1048574.0 / 1048574.0 =  1.0
		 0.8, //  1: = 0x00066666 =  419430 =   838860 =   838859.2 / 1048574.0 =  0.8 (  838859.2  838858=1.2  838860=0.8 )
		 0.6, //  2: = 0x0004CCCC =  314572 =   629144 =   629144.4 / 1048574.0 =  0.6 (  629144.4  629144=0.4  629146=1.6 )
		 0.4, //  3: = 0x00033333 =  209715 =   419430 =   419429.6 / 1048574.0 =  0.4 (  419429.6  419428=1.6  419430=0.4 )
		 0.2, //  4: = 0x00019999 =  104857 =   209714 =   209714.8 / 1048574.0 =  0.2 (  209714.8  209714=0.8  209716=1.2 )
		-0.2, //  5: = 0xFFFE6666 = -104858 =  -209716 =  -209715.2 / 1048576.0 = -0.2 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.4, //  6: = 0xFFFCCCCD = -209715 =  -419430 =  -419430.4 / 1048576.0 = -0.4 ( -419430.4 -419430=0.4 -419432=1.6 )
		-0.6, //  7: = 0xFFFB3333 = -314573 =  -629146 =  -629145.6 / 1048576.0 = -0.6 ( -629145.6 -629144=1.6 -629146=0.4 )
		-0.8, //  8: = 0xFFF9999A = -419430 =  -838860 =  -838860.8 / 1048576.0 = -0.8 ( -838860.8 -838860=0.8 -838862=1.2 )
		-1.0, //  9: = 0xFFF80000 = -524288 = -1048576 = -1048576.0 / 1048576.0 = -1.0
		 1.2, // 10: = 0x0007FFFF =  1.2
		-1.2  // 11: = 0xFFF80000 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[24] = {
		0xff, 0xff, 0x07, 0x00, // 0
		0xcc, 0xcc, 0x04, 0x00, // 2
		0x99, 0x99, 0x01, 0x00, // 4
		0xcd, 0xcc, 0xfc, 0xff, // 6
		0x9a, 0x99, 0xf9, 0xff, // 8
		0xff, 0xff, 0x07, 0x00  // 10
	};

	const tubyte c_expectIntegerSamplesB[24] = {
		0x66, 0x66, 0x06, 0x00, // 1
		0x33, 0x33, 0x03, 0x00, // 3
		0x66, 0x66, 0xfe, 0xff, // 5
		0x33, 0x33, 0xfb, 0xff, // 7
		0x00, 0x00, 0xf8, 0xff, // 9
		0x00, 0x00, 0xf8, 0xff  // 11
	};

	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt32LSB20);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,24));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,24));
}

TEST(ASIOData,sampleTo20BitIn32BitLSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: = 0x0007FFFF =  524287 =  1048574 =  1048574.0 / 1048574.0 =  1.0
		 0.8, //  1: = 0x00066666 =  419430 =   838860 =   838859.2 / 1048574.0 =  0.8 (  838859.2  838858=1.2  838860=0.8 )
		 0.6, //  2: = 0x0004CCCC =  314572 =   629144 =   629144.4 / 1048574.0 =  0.6 (  629144.4  629144=0.4  629146=1.6 )
		 0.4, //  3: = 0x00033333 =  209715 =   419430 =   419429.6 / 1048574.0 =  0.4 (  419429.6  419428=1.6  419430=0.4 )
		 0.2, //  4: = 0x00019999 =  104857 =   209714 =   209714.8 / 1048574.0 =  0.2 (  209714.8  209714=0.8  209716=1.2 )
		-0.2, //  5: = 0xFFFE6666 = -104858 =  -209716 =  -209715.2 / 1048576.0 = -0.2 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.4, //  6: = 0xFFFCCCCD = -209715 =  -419430 =  -419430.4 / 1048576.0 = -0.4 ( -419430.4 -419430=0.4 -419432=1.6 )
		-0.6, //  7: = 0xFFFB3333 = -314573 =  -629146 =  -629145.6 / 1048576.0 = -0.6 ( -629145.6 -629144=1.6 -629146=0.4 )
		-0.8, //  8: = 0xFFF9999A = -419430 =  -838860 =  -838860.8 / 1048576.0 = -0.8 ( -838860.8 -838860=0.8 -838862=1.2 )
		-1.0, //  9: = 0xFFF80000 = -524288 = -1048576 = -1048576.0 / 1048576.0 = -1.0
		 1.2, // 10: = 0x0007FFFF =  1.2
		-1.2  // 11: = 0xFFF80000 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[12] = {
		0xff, 0xff, 0x07, 0x00, // 0
		0x99, 0x99, 0x01, 0x00, // 4
		0x9a, 0x99, 0xf9, 0xff  // 8
	};

	const tubyte c_expectIntegerSamplesB[12] = {
		0x66, 0x66, 0x06, 0x00, // 1
		0x66, 0x66, 0xfe, 0xff, // 5
		0x00, 0x00, 0xf8, 0xff  // 9
	};
	
	const tubyte c_expectIntegerSamplesC[12] = {
		0xcc, 0xcc, 0x04, 0x00, // 2
		0xcd, 0xcc, 0xfc, 0xff, // 6
		0xff, 0xff, 0x07, 0x00  // 10
	};
	
	const tubyte c_expectIntegerSamplesD[12] = {
		0x33, 0x33, 0x03, 0x00, // 3
		0x33, 0x33, 0xfb, 0xff, // 7
		0x00, 0x00, 0xf8, 0xff  // 11
	};

	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt32LSB20);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,12));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,12));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo20BitIn32BitMSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: = 0x0007FFFF =  524287 =  1048574 =  1048574.0 / 1048574.0 =  1.0
		 0.8, //  1: = 0x00066666 =  419430 =   838860 =   838859.2 / 1048574.0 =  0.8 (  838859.2  838858=1.2  838860=0.8 )
		 0.6, //  2: = 0x0004CCCC =  314572 =   629144 =   629144.4 / 1048574.0 =  0.6 (  629144.4  629144=0.4  629146=1.6 )
		 0.4, //  3: = 0x00033333 =  209715 =   419430 =   419429.6 / 1048574.0 =  0.4 (  419429.6  419428=1.6  419430=0.4 )
		 0.2, //  4: = 0x00019999 =  104857 =   209714 =   209714.8 / 1048574.0 =  0.2 (  209714.8  209714=0.8  209716=1.2 )
		-0.2, //  5: = 0xFFFE6666 = -104858 =  -209716 =  -209715.2 / 1048576.0 = -0.2 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.4, //  6: = 0xFFFCCCCD = -209715 =  -419430 =  -419430.4 / 1048576.0 = -0.4 ( -419430.4 -419430=0.4 -419432=1.6 )
		-0.6, //  7: = 0xFFFB3333 = -314573 =  -629146 =  -629145.6 / 1048576.0 = -0.6 ( -629145.6 -629144=1.6 -629146=0.4 )
		-0.8, //  8: = 0xFFF9999A = -419430 =  -838860 =  -838860.8 / 1048576.0 = -0.8 ( -838860.8 -838860=0.8 -838862=1.2 )
		-1.0, //  9: = 0xFFF80000 = -524288 = -1048576 = -1048576.0 / 1048576.0 = -1.0
		 1.2, // 10: = 0x0007FFFF =  1.2
		-1.2  // 11: = 0xFFF80000 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamples[48] = {
		0x00, 0x07, 0xff, 0xff, // 0
		0x00, 0x06, 0x66, 0x66, // 1
		0x00, 0x04, 0xcc, 0xcc, // 2
		0x00, 0x03, 0x33, 0x33, // 3
		0x00, 0x01, 0x99, 0x99, // 4
		0xff, 0xfe, 0x66, 0x66, // 5
		0xff, 0xfc, 0xcc, 0xcd, // 6
		0xff, 0xfb, 0x33, 0x33, // 7
		0xff, 0xf9, 0x99, 0x9a, // 8
		0xff, 0xf8, 0x00, 0x00, // 9
		0x00, 0x07, 0xff, 0xff, // 10
		0xff, 0xf8, 0x00, 0x00  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt32MSB20);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,48));
}

TEST(ASIOData,sampleTo20BitIn32BitMSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: = 0x0007FFFF =  524287 =  1048574 =  1048574.0 / 1048574.0 =  1.0
		 0.8, //  1: = 0x00066666 =  419430 =   838860 =   838859.2 / 1048574.0 =  0.8 (  838859.2  838858=1.2  838860=0.8 )
		 0.6, //  2: = 0x0004CCCC =  314572 =   629144 =   629144.4 / 1048574.0 =  0.6 (  629144.4  629144=0.4  629146=1.6 )
		 0.4, //  3: = 0x00033333 =  209715 =   419430 =   419429.6 / 1048574.0 =  0.4 (  419429.6  419428=1.6  419430=0.4 )
		 0.2, //  4: = 0x00019999 =  104857 =   209714 =   209714.8 / 1048574.0 =  0.2 (  209714.8  209714=0.8  209716=1.2 )
		-0.2, //  5: = 0xFFFE6666 = -104858 =  -209716 =  -209715.2 / 1048576.0 = -0.2 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.4, //  6: = 0xFFFCCCCD = -209715 =  -419430 =  -419430.4 / 1048576.0 = -0.4 ( -419430.4 -419430=0.4 -419432=1.6 )
		-0.6, //  7: = 0xFFFB3333 = -314573 =  -629146 =  -629145.6 / 1048576.0 = -0.6 ( -629145.6 -629144=1.6 -629146=0.4 )
		-0.8, //  8: = 0xFFF9999A = -419430 =  -838860 =  -838860.8 / 1048576.0 = -0.8 ( -838860.8 -838860=0.8 -838862=1.2 )
		-1.0, //  9: = 0xFFF80000 = -524288 = -1048576 = -1048576.0 / 1048576.0 = -1.0
		 1.2, // 10: = 0x0007FFFF =  1.2
		-1.2  // 11: = 0xFFF80000 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[24] = {
		0x00, 0x07, 0xff, 0xff, // 0
		0x00, 0x04, 0xcc, 0xcc, // 2
		0x00, 0x01, 0x99, 0x99, // 4
		0xff, 0xfc, 0xcc, 0xcd, // 6
		0xff, 0xf9, 0x99, 0x9a, // 8
		0x00, 0x07, 0xff, 0xff  // 10
	};

	const tubyte c_expectIntegerSamplesB[24] = {
		0x00, 0x06, 0x66, 0x66, // 1
		0x00, 0x03, 0x33, 0x33, // 3
		0xff, 0xfe, 0x66, 0x66, // 5
		0xff, 0xfb, 0x33, 0x33, // 7
		0xff, 0xf8, 0x00, 0x00, // 9
		0xff, 0xf8, 0x00, 0x00  // 11
	};

	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt32MSB20);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,24));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,24));
}

TEST(ASIOData,sampleTo20BitIn32BitMSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0: = 0x0007FFFF =  524287 =  1048574 =  1048574.0 / 1048574.0 =  1.0
		 0.8, //  1: = 0x00066666 =  419430 =   838860 =   838859.2 / 1048574.0 =  0.8 (  838859.2  838858=1.2  838860=0.8 )
		 0.6, //  2: = 0x0004CCCC =  314572 =   629144 =   629144.4 / 1048574.0 =  0.6 (  629144.4  629144=0.4  629146=1.6 )
		 0.4, //  3: = 0x00033333 =  209715 =   419430 =   419429.6 / 1048574.0 =  0.4 (  419429.6  419428=1.6  419430=0.4 )
		 0.2, //  4: = 0x00019999 =  104857 =   209714 =   209714.8 / 1048574.0 =  0.2 (  209714.8  209714=0.8  209716=1.2 )
		-0.2, //  5: = 0xFFFE6666 = -104858 =  -209716 =  -209715.2 / 1048576.0 = -0.2 ( -209715.2 -209714=1.2 -209716=0.8 )
		-0.4, //  6: = 0xFFFCCCCD = -209715 =  -419430 =  -419430.4 / 1048576.0 = -0.4 ( -419430.4 -419430=0.4 -419432=1.6 )
		-0.6, //  7: = 0xFFFB3333 = -314573 =  -629146 =  -629145.6 / 1048576.0 = -0.6 ( -629145.6 -629144=1.6 -629146=0.4 )
		-0.8, //  8: = 0xFFF9999A = -419430 =  -838860 =  -838860.8 / 1048576.0 = -0.8 ( -838860.8 -838860=0.8 -838862=1.2 )
		-1.0, //  9: = 0xFFF80000 = -524288 = -1048576 = -1048576.0 / 1048576.0 = -1.0
		 1.2, // 10: = 0x0007FFFF =  1.2
		-1.2  // 11: = 0xFFF80000 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[12] = {
		0x00, 0x07, 0xff, 0xff, // 0
		0x00, 0x01, 0x99, 0x99, // 4
		0xff, 0xf9, 0x99, 0x9a  // 8
	};
	
	const tubyte c_expectIntegerSamplesB[12] = {
		0x00, 0x06, 0x66, 0x66, // 1
		0xff, 0xfe, 0x66, 0x66, // 5
		0xff, 0xf8, 0x00, 0x00  // 9
	};
	
	const tubyte c_expectIntegerSamplesC[12] = {
		0x00, 0x04, 0xcc, 0xcc, // 2
		0xff, 0xfc, 0xcc, 0xcd, // 6
		0x00, 0x07, 0xff, 0xff  // 10
	};
	
	const tubyte c_expectIntegerSamplesD[12] = {
		0x00, 0x03, 0x33, 0x33, // 3
		0xff, 0xfb, 0x33, 0x33, // 7
		0xff, 0xf8, 0x00, 0x00  // 11
	};

	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt32MSB20);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,12));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,12));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo24BitIn32BitLSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710886 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamples[48] = {
		0xff, 0xff, 0x7f, 0x00, // 0
		0x66, 0x66, 0x66, 0x00, // 1
		0xcc, 0xcc, 0x4c, 0x00, // 2
		0x33, 0x33, 0x33, 0x00, // 3
		0x99, 0x99, 0x19, 0x00, // 4
		0x66, 0x66, 0xe6, 0xff, // 5
		0xcd, 0xcc, 0xcc, 0xff, // 6
		0x33, 0x33, 0xb3, 0xff, // 7
		0x9a, 0x99, 0x99, 0xff, // 8
		0x00, 0x00, 0x80, 0xff, // 9
		0xff, 0xff, 0x7f, 0x00, // 10
		0x00, 0x00, 0x80, 0xff  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt32LSB24);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,48));
}

TEST(ASIOData,sampleTo24BitIn32BitLSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710886 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[24] = {
		0xff, 0xff, 0x7f, 0x00, // 0
		0xcc, 0xcc, 0x4c, 0x00, // 2
		0x99, 0x99, 0x19, 0x00, // 4
		0xcd, 0xcc, 0xcc, 0xff, // 6
		0x9a, 0x99, 0x99, 0xff, // 8
		0xff, 0xff, 0x7f, 0x00  // 10
	};

	const tubyte c_expectIntegerSamplesB[24] = {
		0x66, 0x66, 0x66, 0x00, // 1
		0x33, 0x33, 0x33, 0x00, // 3
		0x66, 0x66, 0xe6, 0xff, // 5
		0x33, 0x33, 0xb3, 0xff, // 7
		0x00, 0x00, 0x80, 0xff, // 9
		0x00, 0x00, 0x80, 0xff  // 11
	};

	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt32LSB24);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,24));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,24));
}

TEST(ASIOData,sampleTo24BitIn32BitLSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710886 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[12] = {
		0xff, 0xff, 0x7f, 0x00, // 0
		0x99, 0x99, 0x19, 0x00, // 4
		0x9a, 0x99, 0x99, 0xff, // 8
	};

	const tubyte c_expectIntegerSamplesB[12] = {
		0x66, 0x66, 0x66, 0x00, // 1
		0x66, 0x66, 0xe6, 0xff, // 5
		0x00, 0x00, 0x80, 0xff, // 9
	};

	const tubyte c_expectIntegerSamplesC[12] = {
		0xcc, 0xcc, 0x4c, 0x00, // 2
		0xcd, 0xcc, 0xcc, 0xff, // 6
		0xff, 0xff, 0x7f, 0x00, // 10
	};

	const tubyte c_expectIntegerSamplesD[12] = {
		0x33, 0x33, 0x33, 0x00, // 3
		0x33, 0x33, 0xb3, 0xff, // 7
		0x00, 0x00, 0x80, 0xff  // 11
	};

	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt32LSB24);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,12));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,12));
}

//-------------------------------------------------------------------------------------------

TEST(ASIOData,sampleTo24BitIn32BitMSBConvertionMono)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710886 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamples[48] = {
		0x00, 0x7f, 0xff, 0xff, // 0
		0x00, 0x66, 0x66, 0x66, // 1
		0x00, 0x4c, 0xcc, 0xcc, // 2
		0x00, 0x33, 0x33, 0x33, // 3
		0x00, 0x19, 0x99, 0x99, // 4
		0xff, 0xe6, 0x66, 0x66, // 5
		0xff, 0xcc, 0xcc, 0xcd, // 6
		0xff, 0xb3, 0x33, 0x33, // 7
		0xff, 0x99, 0x99, 0x9a, // 8
		0xff, 0x80, 0x00, 0x00, // 9
		0x00, 0x7f, 0xff, 0xff, // 10
		0xff, 0x80, 0x00, 0x00  // 11
	};

	ASIOData data(12,1,1);
	data.setSampleType(ASIOSTInt32MSB24);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 12;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamples,48));
}

TEST(ASIOData,sampleTo24BitIn32BitMSBConvertionStereo)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710886 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[24] = {
		0x00, 0x7f, 0xff, 0xff, // 0
		0x00, 0x4c, 0xcc, 0xcc, // 2
		0x00, 0x19, 0x99, 0x99, // 4
		0xff, 0xcc, 0xcc, 0xcd, // 6
		0xff, 0x99, 0x99, 0x9a, // 8
		0x00, 0x7f, 0xff, 0xff // 10
	};

	const tubyte c_expectIntegerSamplesB[24] = {
		0x00, 0x66, 0x66, 0x66, // 1
		0x00, 0x33, 0x33, 0x33, // 3
		0xff, 0xe6, 0x66, 0x66, // 5
		0xff, 0xb3, 0x33, 0x33, // 7
		0xff, 0x80, 0x00, 0x00, // 9
		0xff, 0x80, 0x00, 0x00  // 11
	};

	ASIOData data(6,2,2);
	data.setSampleType(ASIOSTInt32MSB24);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 6;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,24));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,24));
}

TEST(ASIOData,sampleTo24BitIn32BitMSBConvertionQuadrophonic)
{
#if defined(SINGLE_FLOAT_SAMPLE)
	const sample_t c_sampleInput[12] = {
		1.0f, 0.8f, 0.6f, 0.4f, 0.2f, -0.2f, -0.4f, -0.6f, -0.8f, -1.0f, 1.2f, -1.2f
	};
#else
	const sample_t c_sampleInput[12] = {
		 1.0, //  0:  0x7FFFFF =  8388607 <-  16777214 =  16777214.0 / 16777214.0 =  1.0
		 0.8, //  1:  0x666666 =  6710886 <-  13421772 =  13421771.2 / 16777214.0 =  0.8 ( 13421771.2  13421770=1.2  13421772=0.8)
		 0.6, //  2:  0x4CCCCC =  5033164 <-  10066328 =  10066328.4 / 16777214.0 =  0.6 ( 10066328.4  10066328=0.4  10066330=1.6)
		 0.4, //  3:  0x333333 =  3355443 <-   6710886 =   6710885.6 / 16777214.0 =  0.4 (  6710885.6   6710884=1.6   6710886=0.4)
		 0.2, //  4:  0x199999 =  1677721 <-   3355442 =   3355442.8 / 16777214.0 =  0.2 (  3355442.8   3355442=0.8   3355444=1.2)
		-0.2, //  5:  0xE66666 = -1677722 <-  -3355444 =  -3355443.2 / 16777216.0 = -0.2 ( -3355443.2  -3355442=1.2  -3355444=0.8)
		-0.4, //  6:  0xCCCCCD = -3355443 <-  -6710886 =  -6710886.4 / 16777216.0 = -0.4 ( -6710886.4  -6710886=0.4  -6710888=1.6)
		-0.6, //  7:  0xB33333 = -5033165 <- -10066330 = -10066329.6 / 16777216.0 = -0.6 (-10066329.6 -10066328=1.6 -10066330=0.4)
		-0.8, //  8:  0x99999A = -6710886 <- -13421772 = -13421772.8 / 16777216.0 = -0.8 (-13421772.8 -13421772=0.8 -13421774=1.2)
		-1.0, //  9:  0x800000 = -8388608 <- -16777216 = -16777216.0 / 16777216.0 = -1.0
		 1.2, // 10:  0x7FFFFF =  8388607 =  1.2
		-1.2  // 11:  0x800000 = -8388608 = -1.2
	};
#endif
	
	const tubyte c_expectIntegerSamplesA[12] = {
		0x00, 0x7f, 0xff, 0xff, // 0
		0x00, 0x19, 0x99, 0x99, // 4
		0xff, 0x99, 0x99, 0x9a  // 8
	};

	const tubyte c_expectIntegerSamplesB[12] = {
		0x00, 0x66, 0x66, 0x66, // 1
		0xff, 0xe6, 0x66, 0x66, // 5
		0xff, 0x80, 0x00, 0x00  // 9
	};

	const tubyte c_expectIntegerSamplesC[12] = {
		0x00, 0x4c, 0xcc, 0xcc, // 2
		0xff, 0xcc, 0xcc, 0xcd, // 6
		0x00, 0x7f, 0xff, 0xff  // 10
	};

	const tubyte c_expectIntegerSamplesD[12] = {
		0x00, 0x33, 0x33, 0x33, // 3
		0xff, 0xb3, 0x33, 0x33, // 7
		0xff, 0x80, 0x00, 0x00  // 11
	};

	ASIOData data(3,4,4);
	data.setSampleType(ASIOSTInt32MSB24);
	
	engine::RData::Part& part = data.nextPart();
	memcpy(data.partDataOut(0),c_sampleInput,12 * sizeof(sample_t));
	part.length() = 3;
	part.done() = true;
	
	data.convert();
	
	ASSERT_EQ(0,memcmp(data.asioData(0,0),c_expectIntegerSamplesA,12));
	ASSERT_EQ(0,memcmp(data.asioData(1,0),c_expectIntegerSamplesB,12));
	ASSERT_EQ(0,memcmp(data.asioData(2,0),c_expectIntegerSamplesC,12));
	ASSERT_EQ(0,memcmp(data.asioData(3,0),c_expectIntegerSamplesD,12));
}

//-------------------------------------------------------------------------------------------
