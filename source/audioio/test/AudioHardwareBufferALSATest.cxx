#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "audioio/inc/AudioHardwareBufferALSA.h"

using namespace orcus;
using namespace audioio;
using namespace testing;

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned8BitFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S8,40,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(1,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(40,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(40 * 2 * 1,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned16BitLittleEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S16_LE,40,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(2,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(40,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(40 * 2 * 2,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned16BitBigEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S16_BE,80,4);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(2,buffer.sampleSize(0));
	EXPECT_EQ(4,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(80,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(80 * 4 * 2,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned24BitLittleEndianIn4BytesFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S24_LE,80,1);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(4,buffer.sampleSize(0));
	EXPECT_EQ(1,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(80,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(80 * 1 * 4,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned24BitBigEndianIn4BytesFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S24_BE,80,8);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(4,buffer.sampleSize(0));
	EXPECT_EQ(8,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(80,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(80 * 8 * 4,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned32BitLittleEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S32_LE,80,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(4,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(80,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(80 * 2 * 4,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned32BitBigEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S32_BE,80,3);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(4,buffer.sampleSize(0));
	EXPECT_EQ(3,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(80,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(80 * 3 * 4,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithFloatLittleEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_FLOAT_LE,160,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(4,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(160,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(160 * 2 * 4,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithFloatBigEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_FLOAT_BE,160,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(4,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(160,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(160 * 2 * 4,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithFloat64LittleEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_FLOAT64_LE,160,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(8,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(160,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(160 * 2 * 8,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithFloat64BigEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_FLOAT64_BE,160,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(8,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(160,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(160 * 2 * 8,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned24BitLittleEndianIn3BytesFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S24_3LE,160,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(3,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(160,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(160 * 2 * 3,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned24BitBigEndianIn3BytesFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S24_3BE,160,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(3,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(160,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(160 * 2 * 3,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned20BitLittleEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S20_3LE,160,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(3,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(160,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(160 * 2 * 3,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned20BitBigEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S20_3BE,160,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(3,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(160,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(160 * 2 * 3,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned18BitLittleEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S18_3LE,160,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(3,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(160,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(160 * 2 * 3,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------

TEST(AudioHardwareBufferALSA,bufferWithSigned18BitBigEndianFormat)
{
	AudioHardwareBufferALSA buffer(SND_PCM_FORMAT_S18_3BE,160,2);
	EXPECT_TRUE(buffer.buffer(0)!=0);
	EXPECT_TRUE(buffer.buffer(1)==0);
	EXPECT_EQ(3,buffer.sampleSize(0));
	EXPECT_EQ(2,buffer.numberOfChannelsInBuffer(0));
	EXPECT_EQ(0,buffer.numberOfChannelsInBuffer(1));
	EXPECT_EQ(160,buffer.bufferLength());
	EXPECT_EQ(1,buffer.numberOfBuffers());
	EXPECT_EQ(160 * 2 * 3,buffer.numberOfBytesInBuffer());
}

//-------------------------------------------------------------------------------------------
