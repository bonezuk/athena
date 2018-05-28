#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "engine/monkeyomega/inc/MonkeyCodec.h"

#include "monkey/All.h"
#include "monkey/MACLib.h"

using namespace APE;

TEST(MonkeyDecode,decode)
{
	int nRetVal = 0;

    QString fileName = QString::fromLatin1("/Users/bonez/VMWare/Musepack/encode/quest.ape");
    wchar_t *fWStr = new wchar_t [fileName.length() + 1];
    fileName.toWCharArray(fWStr);

    IAPEDecompress *pAPEDecompress = CreateIAPEDecompress(fWStr,&nRetVal);
	ASSERT_TRUE(pAPEDecompress!=0);
	
	int nAlign = pAPEDecompress->GetInfo(APE_INFO_BLOCK_ALIGN,0,0);
	ASSERT_TRUE(nAlign > 0);
	char *pRawData = new char [1024 * nAlign];
	ASSERT_TRUE(pRawData!=0);
	
	int nBlocksRetrieved = 1;
	int nTotalBlocksRetrieved = 0;
	int nTotalBlocks = pAPEDecompress->GetInfo(APE_DECOMPRESS_TOTAL_BLOCKS,0,0);
	
	while(nBlocksRetrieved > 0)
	{
		nRetVal = pAPEDecompress->GetData(pRawData,1024,&nBlocksRetrieved);
        EXPECT_TRUE(nRetVal==ERROR_SUCCESS);
		
		nTotalBlocksRetrieved += nBlocksRetrieved;
		
		printf("Progress: %.1f%%          \r",(static_cast<float>(nTotalBlocksRetrieved) * 100.0f) / static_cast<float>(nTotalBlocks));
	}
	
	delete [] pRawData;
	delete pAPEDecompress;
}
