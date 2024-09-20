#include "gtest/gtest.h"

#include "common/inc/DiskOps.h"
#include "common/inc/BIOBufferedStream.h"
#include "engine/inc/Codec.h"
#include "engine/blueomega/inc/WaveEngine.h"
#include "engine/inc/FormatTypeFromFloat.h"
#include "engine/inc/BiQuadFilter.h"

using namespace omega;

//-------------------------------------------------------------------------------------------

void intToMemory(tuint32 v, tubyte *mem)
{
    mem[0] = static_cast<tubyte>(v & 0x000000ff);
    mem[1] = static_cast<tubyte>((v >> 8) & 0x000000ff);
    mem[2] = static_cast<tubyte>((v >> 16) & 0x000000ff);
    mem[3] = static_cast<tubyte>((v >> 24) & 0x000000ff);
}

//-------------------------------------------------------------------------------------------

void intToMemory(tint32 v, tubyte *mem)
{
    intToMemory(static_cast<tuint32>(v), mem);
}

//-------------------------------------------------------------------------------------------

void shortToMemory(tuint16 v, tubyte *mem)
{
    mem[0] = static_cast<tubyte>(v & 0x00ff);
    mem[1] = static_cast<tubyte>((v >> 8) & 0x00ff);
}

//-------------------------------------------------------------------------------------------

void shortToMemory(tint16 v, tubyte *mem)
{
    shortToMemory(static_cast<tuint16>(v), mem);
}

//-------------------------------------------------------------------------------------------

void populateHeaderFromCodec(engine::Codec *codec, tubyte hdr[44])
{
	intToMemory(RIFF_ID, &hdr[0]); // Chunk ID
	intToMemory(0, &hdr[4]); // Chunk size
	intToMemory(WAVE_ID, &hdr[8]); // Wave format
	
	intToMemory(FMT_ID, &hdr[12]); // fmt ID
	intToMemory(16, &hdr[16]);
	
	shortToMemory(static_cast<tuint16>(engine::blueomega::WaveInformation::e_formatPCM), &hdr[20]); // audio format
    shortToMemory(static_cast<tuint16>(codec->noChannels()), &hdr[22]); // number of channels
	intToMemory(codec->frequency(), &hdr[24]); // sample rate
	intToMemory(codec->frequency() * codec->noChannels() * 4, &hdr[28]); // bytes per second
    shortToMemory(static_cast<tuint16>(codec->noChannels() * 4), &hdr[32]); // block align
    shortToMemory(static_cast<tuint16>(32), &hdr[34]); // bits per sample
	
	intToMemory(DATA_ID, &hdr[36]);
	intToMemory(0, &hdr[40]);
}

//-------------------------------------------------------------------------------------------

bool saveWaveHeaderFromCodec(engine::Codec *codec, common::BIOStream *out)
{
	tubyte hdr[44];
	populateHeaderFromCodec(codec, hdr);
	return (out->write(hdr, 44) == 44);
}

//-------------------------------------------------------------------------------------------

bool saveWaveHeaderSize(engine::Codec *codec, int totalDataSize, common::BIOStream *out)
{
	bool res = false;
	tubyte hdr[44];
	
	populateHeaderFromCodec(codec, hdr);
	intToMemory(totalDataSize + 36, &hdr[4]);
	intToMemory(totalDataSize, &hdr[40]);
	if(out->seek(0, common::e_Seek_Start) && out->write(hdr, 44) == 44)
	{
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

TEST(WaveDSPTest, lowPass400Hz)
{
    QString inFilename = "/Users/bonez/Development/Temp/kiss_rose_44.1_24bit.wav";
    QString outFilename = "/Users/bonez/Development/Temp/lp_kiss_rose_1.wav";
	
	//QString inFilename = "D:\\Music\\Temp\\kiss_rose_44.1_24bit.wav";
	//QString outFilename = "D:\\Music\\Temp\\lp_kiss_rose_1.wav";

	ASSERT_TRUE(common::DiskOps::exist(inFilename));
	
	engine::Codec *codec = engine::Codec::get(inFilename);
    ASSERT_TRUE(codec != NULL);
	ASSERT_TRUE(codec->init());
	
	if(common::DiskOps::exist(outFilename))
	{
		common::DiskOps::remove(outFilename);
	}
	
	common::BIOBufferedStream *out = new common::BIOBufferedStream(common::e_BIOStream_FileCreate | common::e_BIOStream_FileWrite);
    ASSERT_TRUE(out != NULL);
	ASSERT_TRUE(out->open(outFilename));
	
	ASSERT_TRUE(saveWaveHeaderFromCodec(codec, out));
	
	int noChannels = codec->noChannels();
	
	engine::BiQuadFilter *lpFilter = new engine::BiQuadFilter [noChannels];
	for(int fIdx = 0; fIdx < noChannels; fIdx++)
	{
		lpFilter[fIdx] = engine::BiQuadFilter::filter(engine::BiQuadFilter::e_LowPass_FirstOrder, 200.0, static_cast<tfloat64>(codec->frequency()));
		ASSERT_FALSE(lpFilter[fIdx].type() == engine::BiQuadFilter::e_UnknownFilter);
	}
	
	int totalDataSize = 0;
	engine::RData data(2048, codec->noChannels(), codec->noChannels());
    while(codec->next(data))
	{
		for(int partIdx = 0; partIdx < data.noParts(); partIdx++)
		{
            sample_t *x = data.partData(partIdx);
			for(int idx = 0; idx < data.part(partIdx).length(); idx++)
			{
                int chIdx;
				sample_t y[8];
                tubyte yOut[8 * 4];
				
				for(chIdx = 0; chIdx < noChannels; chIdx++)
				{
                    y[chIdx] = lpFilter[chIdx].process(x[(idx * noChannels) + chIdx]);
				}
				
				for(chIdx = 0; chIdx < noChannels; chIdx++)
				{
                    engine::write32BitsLittleEndianFromSample(y[chIdx], reinterpret_cast<tchar *>(&yOut[chIdx << 2]));
				}
				ASSERT_EQ(out->write(yOut, noChannels * 4), noChannels * 4);
				totalDataSize += noChannels * 4;
			}
		}
        data.reset();
	}
	
	ASSERT_TRUE(saveWaveHeaderSize(codec, totalDataSize, out));

	delete [] lpFilter;

	out->close();
	delete out;

    codec->close();
    delete codec;
}

//-------------------------------------------------------------------------------------------
