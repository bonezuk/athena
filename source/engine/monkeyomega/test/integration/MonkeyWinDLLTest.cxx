#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "engine/monkeyomega/inc/MonkeyCodec.h"

#include "monkey/All.h"
#include "monkey/MACDll.h"

#if defined(ORCUS_WIN32)

struct MAC_DLL
{
	// APEDecompress functions
	proc_APEDecompress_Create  Create;
	proc_APEDecompress_Destroy Destroy;
	proc_APEDecompress_GetData GetData;
	proc_APEDecompress_Seek    Seek;
	proc_APEDecompress_GetInfo GetInfo;
};

int GetFunctions(HMODULE hMACDll,MAC_DLL *pMACDll)
{
	// clear
	memset(pMACDll, 0, sizeof(MAC_DLL));

	// load the functions
	if (hMACDll != NULL)
	{	
		pMACDll->Create = (proc_APEDecompress_Create) GetProcAddress(hMACDll, "_c_APEDecompress_Create@8");
		pMACDll->Destroy = (proc_APEDecompress_Destroy) GetProcAddress(hMACDll, "_c_APEDecompress_Destroy@4");
		pMACDll->GetData = (proc_APEDecompress_GetData) GetProcAddress(hMACDll, "_c_APEDecompress_GetData@16");
		pMACDll->Seek = (proc_APEDecompress_Seek) GetProcAddress(hMACDll, "_c_APEDecompress_Seek@8");
		pMACDll->GetInfo = (proc_APEDecompress_GetInfo) GetProcAddress(hMACDll, "_c_APEDecompress_GetInfo@16");
	}

	// error check
	if ((pMACDll->Create == NULL) ||
		(pMACDll->Destroy == NULL) ||
		(pMACDll->GetData == NULL) ||
		(pMACDll->Seek == NULL) ||
		(pMACDll->GetInfo == NULL))
	{
		return -1;
	}

	return 0;
}

int VersionCheckInterface(HMODULE hMACDll)
{
	int nRetVal = -1;
	proc_GetInterfaceCompatibility GetInterfaceCompatibility = (proc_GetInterfaceCompatibility) GetProcAddress(hMACDll, "_GetInterfaceCompatibility@12");
	if (GetInterfaceCompatibility)
	{
		nRetVal = GetInterfaceCompatibility(MAC_FILE_VERSION_NUMBER, TRUE, NULL);
	}
	return nRetVal;
}

TEST(MonkeyWinDLLTest,loadDLL)
{
	int nRetVal = 0;

	QString fileName = QString::fromLatin1("C:\\Encode\\Rip\\hobbit\\quest.ape");
	wchar_t *fWStr = new wchar_t [fileName.length() + 1];
	fileName.toWCharArray(fWStr);
	
	HMODULE hMACDll = LoadLibraryA("monkey.dll");
	ASSERT_TRUE(hMACDll!=0);
	
	if(VersionCheckInterface(hMACDll)!=0)
	{
		FreeLibrary(hMACDll);
		ASSERT_TRUE(false);
	}

	MAC_DLL MACDll; 
	if(GetFunctions(hMACDll,&MACDll)!=0)
	{
		FreeLibrary(hMACDll);
		ASSERT_TRUE(false);
	}
	
	APE_DECOMPRESS_HANDLE hAPEDecompress = MACDll.Create(fileName.toUtf8().constData(), &nRetVal);
	if(hAPEDecompress==0)
	{
		FreeLibrary(hMACDll);
		ASSERT_TRUE(false);
	}
	
	int nBlockAlign = MACDll.GetInfo(hAPEDecompress,APE::APE_INFO_BLOCK_ALIGN,0,0);
	unsigned char *pBuffer = new unsigned char [1024 * nBlockAlign];
	
	// loop through the whole file
	int nTotalBlocks = MACDll.GetInfo(hAPEDecompress,APE::APE_DECOMPRESS_TOTAL_BLOCKS,0,0);
	int nBlocksRetrieved = 1;
	int nTotalBlocksRetrieved = 0;
	unsigned int nChecksum = 0;
	while (nBlocksRetrieved > 0)
	{
		// try to decompress 1024 blocks
		nRetVal = MACDll.GetData(hAPEDecompress, (char *) pBuffer, 1024, &nBlocksRetrieved);
		EXPECT_TRUE(nRetVal==0);

		nTotalBlocksRetrieved += nBlocksRetrieved;
		// output the progress
		printf("Progress: %.1f%%          \r", ((float)(nTotalBlocksRetrieved) * 100.0f) / (float)(nTotalBlocks));
	}
	delete [] pBuffer;

	MACDll.Destroy(hAPEDecompress);
	FreeLibrary(hMACDll);
}

#endif