#include "gtest/gtest.h"

#include "common/inc/DiskOps.h"
#include "common/inc/BIOBufferedStream.h"
#include "engine/inc/Codec.h"
#include "engine/blueomega/inc/WaveEngine.h"
#include "engine/inc/FormatTypeFromFloat.h"
#include "engine/inc/BiQuadFilter.h"
#include "engine/inc/RemezFIR.h"

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
	double lpCoef[] = {
		  1.70498064e-03, -1.19593878e-02, -1.11220822e-03,  4.32097252e-05,
		 -8.54285829e-04,  8.31535403e-04, -2.73085553e-04, -4.99412243e-04,
		  9.54724697e-04, -7.68524891e-04,  4.58420880e-05,  7.34090187e-04,
		 -1.03559304e-03,  6.34681438e-04,  2.17254290e-04, -9.49791345e-04,
		  1.05405692e-03, -4.36200921e-04, -5.02864577e-04,  1.12834562e-03,
		 -9.99002015e-04,  1.77815757e-04,  7.93837162e-04, -1.25288003e-03,
		  8.65805682e-04,  1.30076692e-04, -1.07008035e-03,  1.30517237e-03,
		 -6.50876951e-04, -4.73984406e-04,  1.31000571e-03, -1.27349891e-03,
		  3.58652420e-04,  8.33410469e-04, -1.49177088e-03,  1.14466257e-03,
		  2.16344203e-06, -1.18568395e-03,  1.59258255e-03, -9.16532678e-04,
		 -4.12580119e-04,  1.50166134e-03, -1.59307875e-03,  5.91058747e-04,
		  8.50238516e-04, -1.75421457e-03,  1.48285844e-03, -1.83788298e-04,
		 -1.28215351e-03,  1.91481888e-03, -1.25231848e-03, -2.91999380e-04,
		  1.68517801e-03, -1.96434835e-03,  8.99517690e-04,  8.23566146e-04,
		 -2.03959970e-03,  1.89147479e-03, -4.36402677e-04, -1.36402667e-03,
		  2.28069941e-03, -1.65968742e-03, -1.18346031e-04,  1.87640722e-03,
		 -2.40899133e-03,  1.29628444e-03,  7.50906709e-04, -2.33888121e-03,
		  2.37198227e-03, -7.83996736e-04, -1.40943487e-03,  2.70172208e-03,
		 -2.17179989e-03,  1.44716614e-04,  2.06902369e-03, -2.91633336e-03,
		  1.79835382e-03,  6.04664631e-04, -2.66899667e-03,  2.97063286e-03,
		 -1.23561485e-03, -1.40968368e-03,  3.17704641e-03, -2.81887878e-03,
		  5.06841295e-04,  2.23687016e-03, -3.53853858e-03,  2.44976580e-03,
		  3.66518219e-04, -3.03327637e-03,  3.70729340e-03, -1.85105205e-03,
		 -1.35031533e-03,  3.74103533e-03, -3.64275356e-03,  1.02859158e-03,
		  2.39558738e-03, -4.29964269e-03,  3.31105173e-03,  2.34522078e-06,
		 -3.44607176e-03,  4.65068183e-03, -2.69239770e-03, -1.20461086e-03,
		  4.43332346e-03, -4.73380215e-03,  1.77697679e-03,  2.53676118e-03,
		 -5.28401973e-03,  4.49771500e-03, -5.63684298e-04, -3.94094857e-03,
		  5.92394460e-03, -3.89732092e-03, -9.25630061e-04,  5.34010656e-03,
		 -6.26803073e-03,  2.89585371e-03,  2.65734717e-03, -6.65537505e-03,
		  6.24062457e-03, -1.46584441e-03, -4.59085026e-03,  7.79302120e-03,
		 -5.75321094e-03, -4.13046967e-04,  6.66100865e-03, -8.64886123e-03,
		  4.72427057e-03,  2.75731338e-03, -8.80674513e-03,  9.10500861e-03,
		 -3.04774723e-03, -5.58733019e-03,  1.09490322e-02, -9.02702682e-03,
		  5.96990253e-04,  8.95360416e-03, -1.30105586e-02,  8.23034382e-03,
		  2.82925142e-03, -1.29621222e-02,  1.49161435e-02, -6.44005362e-03,
		 -7.60439970e-03,  1.78762346e-02, -1.65908272e-02,  3.13998600e-03,
		  1.45334364e-02, -2.43611901e-02,  1.79672288e-02,  2.87182146e-03,
		 -2.57723150e-02,  3.44011489e-02, -1.89928913e-02, -1.55327266e-02,
		  4.93342399e-02, -5.66095197e-02,  1.96254440e-02,  5.97562835e-02,
		 -1.57902146e-01,  2.38655295e-01,  7.30159905e-01,  2.38655295e-01,
		 -1.57902146e-01,  5.97562835e-02,  1.96254440e-02, -5.66095197e-02,
		  4.93342399e-02, -1.55327266e-02, -1.89928913e-02,  3.44011489e-02,
		 -2.57723150e-02,  2.87182146e-03,  1.79672288e-02, -2.43611901e-02,
		  1.45334364e-02,  3.13998600e-03, -1.65908272e-02,  1.78762346e-02,
		 -7.60439970e-03, -6.44005362e-03,  1.49161435e-02, -1.29621222e-02,
		  2.82925142e-03,  8.23034382e-03, -1.30105586e-02,  8.95360416e-03,
		  5.96990253e-04, -9.02702682e-03,  1.09490322e-02, -5.58733019e-03,
		 -3.04774723e-03,  9.10500861e-03, -8.80674513e-03,  2.75731338e-03,
		  4.72427057e-03, -8.64886123e-03,  6.66100865e-03, -4.13046967e-04,
		 -5.75321094e-03,  7.79302120e-03, -4.59085026e-03, -1.46584441e-03,
		  6.24062457e-03, -6.65537505e-03,  2.65734717e-03,  2.89585371e-03,
		 -6.26803073e-03,  5.34010656e-03, -9.25630061e-04, -3.89732092e-03,
		  5.92394460e-03, -3.94094857e-03, -5.63684298e-04,  4.49771500e-03,
		 -5.28401973e-03,  2.53676118e-03,  1.77697679e-03, -4.73380215e-03,
		  4.43332346e-03, -1.20461086e-03, -2.69239770e-03,  4.65068183e-03,
		 -3.44607176e-03,  2.34522078e-06,  3.31105173e-03, -4.29964269e-03,
		  2.39558738e-03,  1.02859158e-03, -3.64275356e-03,  3.74103533e-03,
		 -1.35031533e-03, -1.85105205e-03,  3.70729340e-03, -3.03327637e-03,
		  3.66518219e-04,  2.44976580e-03, -3.53853858e-03,  2.23687016e-03,
		  5.06841295e-04, -2.81887878e-03,  3.17704641e-03, -1.40968368e-03,
		 -1.23561485e-03,  2.97063286e-03, -2.66899667e-03,  6.04664631e-04,
		  1.79835382e-03, -2.91633336e-03,  2.06902369e-03,  1.44716614e-04,
		 -2.17179989e-03,  2.70172208e-03, -1.40943487e-03, -7.83996736e-04,
		  2.37198227e-03, -2.33888121e-03,  7.50906709e-04,  1.29628444e-03,
		 -2.40899133e-03,  1.87640722e-03, -1.18346031e-04, -1.65968742e-03,
		  2.28069941e-03, -1.36402667e-03, -4.36402677e-04,  1.89147479e-03,
		 -2.03959970e-03,  8.23566146e-04,  8.99517690e-04, -1.96434835e-03,
		  1.68517801e-03, -2.91999380e-04, -1.25231848e-03,  1.91481888e-03,
		 -1.28215351e-03, -1.83788298e-04,  1.48285844e-03, -1.75421457e-03,
		  8.50238516e-04,  5.91058747e-04, -1.59307875e-03,  1.50166134e-03,
		 -4.12580119e-04, -9.16532678e-04,  1.59258255e-03, -1.18568395e-03,
		  2.16344203e-06,  1.14466257e-03, -1.49177088e-03,  8.33410469e-04,
		  3.58652420e-04, -1.27349891e-03,  1.31000571e-03, -4.73984406e-04,
		 -6.50876951e-04,  1.30517237e-03, -1.07008035e-03,  1.30076692e-04,
		  8.65805682e-04, -1.25288003e-03,  7.93837162e-04,  1.77815757e-04,
		 -9.99002015e-04,  1.12834562e-03, -5.02864577e-04, -4.36200921e-04,
		  1.05405692e-03, -9.49791345e-04,  2.17254290e-04,  6.34681438e-04,
		 -1.03559304e-03,  7.34090187e-04,  4.58420880e-05, -7.68524891e-04,
		  9.54724697e-04, -4.99412243e-04, -2.73085553e-04,  8.31535403e-04,
		 -8.54285829e-04,  4.32097252e-05, -1.11220822e-03, -1.19593878e-02,
		  1.70498064e-03
	};


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

TEST(WaveDSPTest, remezTest)
{
    const tint N = 327;
	double f[] = {0.0, 8000.0 / 44100.0 , 8100.0 / 44100.0, 0.5};
	double a[] = {1.0, 1.0, 0.0, 0.0};
    double w[] = {1.0, 1.0};
	
	double h[N + 1];

    bool res = engine::RemezFIR::designBandPass(N, 2, f, a, w, h);
	ASSERT_TRUE(res);
	
    for(tint i = 0; i < N + 1; i+=4)
	{
        fprintf(stdout, "\t%.12f, %.12f, %.12f, %.12f,\n", h[i], h[i+1], h[i+2], h[i+3]);
	}
}

//-------------------------------------------------------------------------------------------
