#include "gtest/gtest.h"

#include "common/inc/DiskOps.h"
#include "common/inc/BIOBufferedStream.h"
#include "engine/inc/Codec.h"
#include "engine/blueomega/inc/WaveEngine.h"
#include "engine/inc/FormatTypeFromFloat.h"
#include "engine/inc/BiQuadFilter.h"
#include "engine/inc/RemezFIR.h"
#include "engine/inc/FIRFilter.h"

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

TEST(WaveDSPTest, lowPass400HzBiQuad)
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

TEST(WaveDSPTest, lowPass500HzLowPassFIR)
{
	double lpCoef[] = {
		 -0.034704234430132916,  0.005416108747275855,  0.00502339208980658,  0.00469467274935257,  // 4
		 0.004382086568789974,  0.004125520886421798,  0.0038769099742894317,  0.0036774646903519684,  // 8
		 0.0034790095870517665,  0.003324115148507986,  0.0031636379842621826,  0.0030426353116016214,  // 12
		 0.002909783959677989,  0.0028140582926278884,  0.0026994989356275284,  0.0026238265987903325,  // 16
		 0.002518881636289848,  0.0024334364621349663,  0.0024050273055876356,  0.002265074178729058,  // 20
		 0.0021856613666517283,  0.0021302791141972754,  0.0020615600376895776,  0.001984757058537845,  // 24
		 0.0018868486401886,  0.001794971893740914,  0.0016926152837622027,  0.0016032774199878086,  // 28
		 0.001502679829365798,  0.0014108252820053332,  0.0013028308096720148,  0.001198787140484027,  // 32
		 0.0010785844847519613,  0.0009592666284256313,  0.0008281822062675415,  0.0007090661194806211,  // 36
		 0.0005519547265557256,  0.000441300285513771,  0.0002964385792189467,  0.0001648752895691899,  // 40
		 1.5292430639253507e-05,  -0.00010850077802521813,  -0.0002499608541454562,  -0.00037417786539172687,  // 44
		 -0.0005196940760437488,  -0.0006460826947293496,  -0.0007883596685755861,  -0.0009064062503503084,  // 48
		 -0.0010382935548618674,  -0.0011449076355851684,  -0.0012674011605759636,  -0.001361551816343537,  // 52
		 -0.001471258312662392,  -0.0015556958370884846,  -0.0016380138951387624,  -0.0017063539840971934,  // 56
		 -0.0017739652494610236,  -0.001812868030571388,  -0.001855668152879436,  -0.0018768229379973606,  // 60
		 -0.0019016201118161671,  -0.0018993604464166147,  -0.0018981835334436342,  -0.0018713419128069675,  // 64
		 -0.0018473230418781447,  -0.0017982804706993677,  -0.0017499017791685056,  -0.0016755710219641742,  // 68
		 -0.0015996035415666368,  -0.0015013916956943818,  -0.0014026476196213056,  -0.0012796440354980662,  // 72
		 -0.0011660660867114508,  -0.0010220197976624143,  -0.0008847310257786567,  -0.0007250575330149389,  // 76
		 -0.0005722124141797763,  -0.00039476829010925973,  -0.0002261088318035731,  -3.8622873487874354e-05,  // 80
		 0.00013704291651518014,  0.0003312502865225331,  0.000513404482822867,  0.0007119512321676277,  // 84
		 0.0008945136743558693,  0.0010903921534521424,  0.0012663411743047137,  0.0014558760813525723,  // 88
		 0.001623375125693903,  0.001798790697053257,  0.001955391983425342,  0.002113454379683813,  // 92
		 0.002248309212901943,  0.002387017255868748,  0.002501717688814936,  0.0026155859897375525,  // 96
		 0.002703041375465433,  0.0027893090132134826,  0.0028477761628826273,  0.0029013480713598605,  // 100
		 0.0029247899213908496,  0.0029423692580617597,  0.0029306901164163485,  0.0029121960598471053,  // 104
		 0.0028650322694416808,  0.002807575325203669,  0.0027212937066011245,  0.0026272780642113205,  // 108
		 0.002501420490335478,  0.002370075363820792,  0.00220954748666731,  0.0020412601109893874,  // 112
		 0.0018437573739307142,  0.0016416076391586112,  0.0014130807982703705,  0.0011801168146316904,  // 116
		 0.0009221354803893994,  0.0006625363298371351,  0.00038099375219251275,  9.971889924616922e-05,  // 120
		 -0.00020135216494187452,  -0.0004987196957604856,  -0.0008130980667271706,  -0.0011183865093726056,  // 124
		 -0.00143745199757115,  -0.0017469760664800202,  -0.002064347019756871,  -0.0023693613989245813,  // 128
		 -0.002679356165211935,  -0.002971369902242234,  -0.0032627450086330154,  -0.003533836006464043,  // 132
		 -0.0038012884347268933,  -0.004043636640443833,  -0.004277121833313906,  -0.004482484902282521,  // 136
		 -0.004675648891916441,  -0.00483706739868562,  -0.0049821413481264815,  -0.0050929541700135555,  // 140
		 -0.005183098670698036,  -0.005237807120924316,  -0.005268191994193837,  -0.005258411842962628,  // 144
		 -0.00522322899611085,  -0.005146238398730851,  -0.00504061630968141,  -0.004892435441322292,  // 148
		 -0.004714725709186206,  -0.004492269048709689,  -0.0042378431680744275,  -0.003939403306633539,  // 152
		 -0.003609935851294023,  -0.0032365598608346743,  -0.002831579326792316,  -0.0023839474605422337,  // 156
		 -0.0019057669391737558,  -0.0013862522971183344,  -0.0008386495684886558,  -0.000250682066415861,  // 160
		 0.00036298371135294605,  0.0010123170888718669,  0.0016857585534125613,  0.002391970215363942,  // 164
		 0.003117886967680429,  0.0038722567259356425,  0.004643042679553205,  0.0054370211695648485,  // 168
		 0.006242048826759682,  0.007066080848345986,  0.007897792582526576,  0.008742599372709312,  // 172
		 0.009588795113376833,  0.010442521498634366,  0.011292496258554593,  0.012144046612760195,  // 176
		 0.012987405305676748,  0.013825003570122306,  0.014648726324210545,  0.015461816289762769,  // 180
		 0.016254760271892557,  0.017030198842136718,  0.017781066955563887,  0.018509257908841514,  // 184
		 0.019207257527364976,  0.01987712855268857,  0.020513045206505817,  0.02111538552704491,  // 188
		 0.021678366937220903,  0.02220375268352462,  0.02268736217839268,  0.023128790668597358,  // 192
		 0.0235248970931013,  0.023875081253179303,  0.0241765403592697,  0.024430679753779112,  // 196
		 0.024634743344123244,  0.024788434455427173,  0.024891130851759057,  0.024943051616453056,  // 200
		 0.024943051616453056,  0.024891130851759057,  0.024788434455427173,  0.024634743344123244,  // 204
		 0.024430679753779112,  0.0241765403592697,  0.023875081253179303,  0.0235248970931013,  // 208
		 0.023128790668597358,  0.02268736217839268,  0.02220375268352462,  0.021678366937220903,  // 212
		 0.02111538552704491,  0.020513045206505817,  0.01987712855268857,  0.019207257527364976,  // 216
		 0.018509257908841514,  0.017781066955563887,  0.017030198842136718,  0.016254760271892557,  // 220
		 0.015461816289762769,  0.014648726324210545,  0.013825003570122306,  0.012987405305676748,  // 224
		 0.012144046612760195,  0.011292496258554593,  0.010442521498634366,  0.009588795113376833,  // 228
		 0.008742599372709312,  0.007897792582526576,  0.007066080848345986,  0.006242048826759682,  // 232
		 0.0054370211695648485,  0.004643042679553205,  0.0038722567259356425,  0.003117886967680429,  // 236
		 0.002391970215363942,  0.0016857585534125613,  0.0010123170888718669,  0.00036298371135294605,  // 240
		 -0.000250682066415861,  -0.0008386495684886558,  -0.0013862522971183344,  -0.0019057669391737558,  // 244
		 -0.0023839474605422337,  -0.002831579326792316,  -0.0032365598608346743,  -0.003609935851294023,  // 248
		 -0.003939403306633539,  -0.0042378431680744275,  -0.004492269048709689,  -0.004714725709186206,  // 252
		 -0.004892435441322292,  -0.00504061630968141,  -0.005146238398730851,  -0.00522322899611085,  // 256
		 -0.005258411842962628,  -0.005268191994193837,  -0.005237807120924316,  -0.005183098670698036,  // 260
		 -0.0050929541700135555,  -0.0049821413481264815,  -0.00483706739868562,  -0.004675648891916441,  // 264
		 -0.004482484902282521,  -0.004277121833313906,  -0.004043636640443833,  -0.0038012884347268933,  // 268
		 -0.003533836006464043,  -0.0032627450086330154,  -0.002971369902242234,  -0.002679356165211935,  // 272
		 -0.0023693613989245813,  -0.002064347019756871,  -0.0017469760664800202,  -0.00143745199757115,  // 276
		 -0.0011183865093726056,  -0.0008130980667271706,  -0.0004987196957604856,  -0.00020135216494187452,  // 280
		 9.971889924616922e-05,  0.00038099375219251275,  0.0006625363298371351,  0.0009221354803893994,  // 284
		 0.0011801168146316904,  0.0014130807982703705,  0.0016416076391586112,  0.0018437573739307142,  // 288
		 0.0020412601109893874,  0.00220954748666731,  0.002370075363820792,  0.002501420490335478,  // 292
		 0.0026272780642113205,  0.0027212937066011245,  0.002807575325203669,  0.0028650322694416808,  // 296
		 0.0029121960598471053,  0.0029306901164163485,  0.0029423692580617597,  0.0029247899213908496,  // 300
		 0.0029013480713598605,  0.0028477761628826273,  0.0027893090132134826,  0.002703041375465433,  // 304
		 0.0026155859897375525,  0.002501717688814936,  0.002387017255868748,  0.002248309212901943,  // 308
		 0.002113454379683813,  0.001955391983425342,  0.001798790697053257,  0.001623375125693903,  // 312
		 0.0014558760813525723,  0.0012663411743047137,  0.0010903921534521424,  0.0008945136743558693,  // 316
		 0.0007119512321676277,  0.000513404482822867,  0.0003312502865225331,  0.00013704291651518014,  // 320
		 -3.8622873487874354e-05,  -0.0002261088318035731,  -0.00039476829010925973,  -0.0005722124141797763,  // 324
		 -0.0007250575330149389,  -0.0008847310257786567,  -0.0010220197976624143,  -0.0011660660867114508,  // 328
		 -0.0012796440354980662,  -0.0014026476196213056,  -0.0015013916956943818,  -0.0015996035415666368,  // 332
		 -0.0016755710219641742,  -0.0017499017791685056,  -0.0017982804706993677,  -0.0018473230418781447,  // 336
		 -0.0018713419128069675,  -0.0018981835334436342,  -0.0018993604464166147,  -0.0019016201118161671,  // 340
		 -0.0018768229379973606,  -0.001855668152879436,  -0.001812868030571388,  -0.0017739652494610236,  // 344
		 -0.0017063539840971934,  -0.0016380138951387624,  -0.0015556958370884846,  -0.001471258312662392,  // 348
		 -0.001361551816343537,  -0.0012674011605759636,  -0.0011449076355851684,  -0.0010382935548618674,  // 352
		 -0.0009064062503503084,  -0.0007883596685755861,  -0.0006460826947293496,  -0.0005196940760437488,  // 356
		 -0.00037417786539172687,  -0.0002499608541454562,  -0.00010850077802521813,  1.5292430639253507e-05,  // 360
		 0.0001648752895691899,  0.0002964385792189467,  0.000441300285513771,  0.0005519547265557256,  // 364
		 0.0007090661194806211,  0.0008281822062675415,  0.0009592666284256313,  0.0010785844847519613,  // 368
		 0.001198787140484027,  0.0013028308096720148,  0.0014108252820053332,  0.001502679829365798,  // 372
		 0.0016032774199878086,  0.0016926152837622027,  0.001794971893740914,  0.0018868486401886,  // 376
		 0.001984757058537845,  0.0020615600376895776,  0.0021302791141972754,  0.0021856613666517283,  // 380
		 0.002265074178729058,  0.0024050273055876356,  0.0024334364621349663,  0.002518881636289848,  // 384
		 0.0026238265987903325,  0.0026994989356275284,  0.0028140582926278884,  0.002909783959677989,  // 388
		 0.0030426353116016214,  0.0031636379842621826,  0.003324115148507986,  0.0034790095870517665,  // 392
		 0.0036774646903519684,  0.0038769099742894317,  0.004125520886421798,  0.004382086568789974,  // 396
		 0.00469467274935257,  0.00502339208980658,  0.005416108747275855,  -0.034704234430132916,  // 400
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
	
	engine::FIRFilter filterL(lpCoef, 400);
	engine::FIRFilter filterR(lpCoef, 400);

	int totalDataSize = 0;
    
    const int c_dataSize = 2048;
    int dataIdx = 0;
    int sampleCount = 0;
    engine::RData dataA(c_dataSize, codec->noChannels(), codec->noChannels());
    engine::RData dataB(c_dataSize, codec->noChannels(), codec->noChannels());
    engine::RData dataC(c_dataSize, codec->noChannels(), codec->noChannels());
    engine::RData *dataPkts[3] = {&dataA, &dataB, &dataC};
    
    engine::RData *data = dataPkts[dataIdx % 3];
    while(codec->next(*data))
	{
        filterL.process(data, 0, 0, false);
        filterR.process(data, 1, 0, false);

		if(dataIdx > 0)
		{
	        data = dataPkts[(dataIdx - 1) % 3];

			for(int partIdx = 0; partIdx < data->noParts(); partIdx++)
			{
				sample_t *x = data->partFilterData(partIdx, 0);
				
				for(int idx = 0; idx < data->part(partIdx).length(); idx++)
				{
					int chIdx;
					sample_t y[8];
					tubyte yOut[8 * 4];
					
					for(chIdx = 0; chIdx < noChannels; chIdx++)
					{
						y[chIdx] = x[(idx * noChannels) + chIdx];
					}
					
					for(chIdx = 0; chIdx < noChannels; chIdx++)
					{
						engine::write32BitsLittleEndianFromSample(y[chIdx], reinterpret_cast<tchar *>(&yOut[chIdx << 2]));
					}
					ASSERT_EQ(out->write(yOut, noChannels * 4), noChannels * 4);
					totalDataSize += noChannels * 4;
					sampleCount++;
				}
			}
		}
		
		dataIdx++;
		
        data = dataPkts[dataIdx % 3];
        data->reset();
	}
	
	ASSERT_TRUE(saveWaveHeaderSize(codec, totalDataSize, out));

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

TEST(WaveDSPTest, lowPass500HzRemezLowPassFIR)
{
    const tint N = 400;
	double f[] = {0.0, 500.0 / 44100.0 , 550.0 / 44100.0, 0.5};
	double a[] = {1.0, 1.0, 0.0, 0.0};
    double w[] = {1.0, 1.0};
	
	double h[N + 1];

    bool res = engine::RemezFIR::designBandPass(N, 2, f, a, w, h);
	ASSERT_TRUE(res);

    QString inFilename = "/Users/bonez/Development/Temp/kiss_rose_44.1_24bit.wav";
    QString outFilename = "/Users/bonez/Development/Temp/lp_kiss_rose_1_remez.wav";
	
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
	
	engine::FIRFilter filterL(h, N);
	engine::FIRFilter filterR(h, N);

	int totalDataSize = 0;
    
    const int c_dataSize = 2048;
    int dataIdx = 0;
    int sampleCount = 0;
    engine::RData dataA(c_dataSize, codec->noChannels(), codec->noChannels());
    engine::RData dataB(c_dataSize, codec->noChannels(), codec->noChannels());
    engine::RData dataC(c_dataSize, codec->noChannels(), codec->noChannels());
    engine::RData *dataPkts[3] = {&dataA, &dataB, &dataC};
    
    engine::RData *data = dataPkts[dataIdx % 3];
    while(codec->next(*data))
	{
        filterL.process(data, 0, 0, false);
        filterR.process(data, 1, 0, false);

		if(dataIdx > 0)
		{
	        data = dataPkts[(dataIdx - 1) % 3];

			for(int partIdx = 0; partIdx < data->noParts(); partIdx++)
			{
				sample_t *x = data->partFilterData(partIdx, 0);
				
				for(int idx = 0; idx < data->part(partIdx).length(); idx++)
				{
					int chIdx;
					sample_t y[8];
					tubyte yOut[8 * 4];
					
					for(chIdx = 0; chIdx < noChannels; chIdx++)
					{
						y[chIdx] = x[(idx * noChannels) + chIdx];
					}
					
					for(chIdx = 0; chIdx < noChannels; chIdx++)
					{
						engine::write32BitsLittleEndianFromSample(y[chIdx], reinterpret_cast<tchar *>(&yOut[chIdx << 2]));
					}
					ASSERT_EQ(out->write(yOut, noChannels * 4), noChannels * 4);
					totalDataSize += noChannels * 4;
					sampleCount++;
				}
			}
		}
		
		dataIdx++;
		
        data = dataPkts[dataIdx % 3];
        data->reset();
	}
	
	ASSERT_TRUE(saveWaveHeaderSize(codec, totalDataSize, out));

	out->close();
	delete out;

    codec->close();
    delete codec;
}

//-------------------------------------------------------------------------------------------