#include "engine/monkeyomega/inc/MonkeyIF.h"

#include "monkey/All.h"
#if defined(ORCUS_WIN32)
#include "monkey/MACDll.h"
#else
#include "monkey/MACLib.h"
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace monkeyomega
{
//-------------------------------------------------------------------------------------------
	
MonkeyIF::MonkeyIF()
{
#if defined(ORCUS_WIN32)
	m_DLL = 0;
	m_pInterface = 0;
	m_pDecoder = 0;
#elif defined(OMEGA_MACOSX)
	m_pDecoder = 0;
#endif	
}

//-------------------------------------------------------------------------------------------

MonkeyIF::~MonkeyIF()
{
	MonkeyIF::destroy();
#if defined(ORCUS_WIN32)
	freeMonkeyLibrary();
#elif defined(OMEGA_MACOSX)
#endif
}

//-------------------------------------------------------------------------------------------
#if defined(ORCUS_WIN32)
//-------------------------------------------------------------------------------------------

typedef struct 
{
	proc_APEDecompress_Create  Create;
	proc_APEDecompress_Destroy Destroy;
	proc_APEDecompress_GetData GetData;
	proc_APEDecompress_Seek    Seek;
	proc_APEDecompress_GetInfo GetInfo;
} MAC_DLL;

//-------------------------------------------------------------------------------------------

bool MonkeyIF::getFunctions(HMODULE hMACDll,void *pVoidMACDll)
{
	MAC_DLL *pMACDll = reinterpret_cast<MAC_DLL *>(pVoidMACDll);
	bool res;
	
	::memset(pMACDll,0,sizeof(MAC_DLL));

	if(hMACDll!=0)
	{	
		pMACDll->Create = (proc_APEDecompress_Create)(::GetProcAddress(hMACDll, "_c_APEDecompress_Create@8"));
		pMACDll->Destroy = (proc_APEDecompress_Destroy)(::GetProcAddress(hMACDll, "_c_APEDecompress_Destroy@4"));
		pMACDll->GetData = (proc_APEDecompress_GetData)(::GetProcAddress(hMACDll, "_c_APEDecompress_GetData@16"));
		pMACDll->Seek = (proc_APEDecompress_Seek)(::GetProcAddress(hMACDll, "_c_APEDecompress_Seek@8"));
		pMACDll->GetInfo = (proc_APEDecompress_GetInfo)(::GetProcAddress(hMACDll, "_c_APEDecompress_GetInfo@16"));
	}
	
	if(pMACDll->Create==0 || pMACDll->Destroy==0 || pMACDll->GetData==0 || pMACDll->Seek==0 || pMACDll->GetInfo==0)
	{
		res = false;
	}
	else
	{
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool MonkeyIF::versionCheckInterface(HMODULE hMACDll)
{
	int nRetVal = -1;
	proc_GetInterfaceCompatibility GetInterfaceCompatibility = (proc_GetInterfaceCompatibility)(::GetProcAddress(hMACDll,"_GetInterfaceCompatibility@12"));
	if(GetInterfaceCompatibility!=0)
	{
		nRetVal = GetInterfaceCompatibility(MAC_FILE_VERSION_NUMBER,TRUE,NULL);
	}
	return (nRetVal==0) ? true : false;
}

//-------------------------------------------------------------------------------------------

bool MonkeyIF::loadMonkeyLibrary()
{
	bool res = false;
	
	if(m_DLL==0 || m_pInterface==0)
	{
		m_DLL = ::LoadLibraryA("monkey.dll");
		if(m_DLL!=0)
		{
			if(versionCheckInterface(m_DLL))
			{
				MAC_DLL *pInterface = new MAC_DLL;
				
				m_pInterface = reinterpret_cast<void *>(pInterface);
				if(getFunctions(m_DLL,m_pInterface))
				{
					res = true;
				}
			}
		}
	}
	else
	{
		res = true;
	}

	if(!res)
	{
		freeMonkeyLibrary();
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void MonkeyIF::freeMonkeyLibrary()
{
	if(m_pInterface!=0)
	{
		MAC_DLL *pInterface = reinterpret_cast<MAC_DLL *>(m_pInterface);
		delete pInterface;
		m_pInterface = 0;		
	}
	if(m_DLL!=0)
	{
		::FreeLibrary(m_DLL);
		m_DLL = 0;
	}
}

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

bool MonkeyIF::create(const QString& fileName)
{
	int nRetVal = 0;
	bool res = false;
	
	destroy();
	
#if defined(ORCUS_WIN32)
	if(loadMonkeyLibrary())
	{
		MAC_DLL *pInterface = reinterpret_cast<MAC_DLL *>(m_pInterface);
				
		m_pDecoder = pInterface->Create(fileName.toLatin1().constData(),&nRetVal);
		if(m_pDecoder!=0)
		{
			res = true;
		}
	}
#elif defined(OMEGA_MACOSX)
    wchar_t *fWStr = new wchar_t [fileName.length() + 1];

    ::memset(fWStr,0,(fileName.length() + 1) * sizeof(wchar_t));
	fileName.toWCharArray(fWStr);
	
    APE::IAPEDecompress *pAPEDecoder = CreateIAPEDecompress(fWStr,&nRetVal);
	m_pDecoder = reinterpret_cast<void *>(pAPEDecoder);
	if(pAPEDecoder!=0)
	{
		res = true;
	}
	
	delete [] fWStr;
#endif

	return res;
}

//-------------------------------------------------------------------------------------------

void MonkeyIF::destroy()
{
	if(m_pDecoder!=0)
	{
#if defined(ORCUS_WIN32)
		if(loadMonkeyLibrary())
		{
			MAC_DLL *pInterface = reinterpret_cast<MAC_DLL *>(m_pInterface);
			pInterface->Destroy(m_pDecoder);
		}
#elif defined(OMEGA_MACOSX)
		APE::IAPEDecompress *pFreeDecoder = reinterpret_cast<APE::IAPEDecompress *>(m_pDecoder);
		delete pFreeDecoder;
#endif
		m_pDecoder = 0;
	}
}

//-------------------------------------------------------------------------------------------

int MonkeyIF::getData(char *pBuffer,int nBlocks,int *pBlocksRetrieved)
{
	int nResult = ERROR_UNDEFINED;
	
	if(m_pDecoder!=0)
	{
#if defined(ORCUS_WIN32)
		if(loadMonkeyLibrary())
		{
			MAC_DLL *pInterface = reinterpret_cast<MAC_DLL *>(m_pInterface);
			nResult = pInterface->GetData(m_pDecoder,pBuffer,nBlocks,pBlocksRetrieved);
		}
#elif defined(OMEGA_MACOSX)
		APE::IAPEDecompress *pDecoder = reinterpret_cast<APE::IAPEDecompress *>(m_pDecoder);
		nResult = pDecoder->GetData(pBuffer,nBlocks,pBlocksRetrieved);
#endif
	}
	return nResult;
}

//-------------------------------------------------------------------------------------------

int MonkeyIF::getInfo(int field,int nParam1,int nParam2)
{
	APE::APE_DECOMPRESS_FIELDS eField = static_cast<APE::APE_DECOMPRESS_FIELDS>(field);
	int nResult = ERROR_UNDEFINED;
	
	if(m_pDecoder!=0)
	{
#if defined(ORCUS_WIN32)
		if(loadMonkeyLibrary())
		{
			MAC_DLL *pInterface = reinterpret_cast<MAC_DLL *>(m_pInterface);
			nResult = pInterface->GetInfo(m_pDecoder,eField,nParam1,nParam2);
		}
#elif defined(OMEGA_MACOSX)
		APE::IAPEDecompress *pDecoder = reinterpret_cast<APE::IAPEDecompress *>(m_pDecoder);
        nResult = pDecoder->GetInfo(eField,nParam1,nParam2);
#endif
	}
	return nResult;	
}

//-------------------------------------------------------------------------------------------

bool MonkeyIF::seek(int nBlockOffset)
{
	int nResult = ERROR_UNDEFINED;
	
	if(m_pDecoder!=0)
	{
#if defined(ORCUS_WIN32)
		if(loadMonkeyLibrary())
		{
			MAC_DLL *pInterface = reinterpret_cast<MAC_DLL *>(m_pInterface);
			nResult = pInterface->Seek(m_pDecoder,nBlockOffset);
		}
#elif defined(OMEGA_MACOSX)
		APE::IAPEDecompress *pDecoder = reinterpret_cast<APE::IAPEDecompress *>(m_pDecoder);
		nResult = pDecoder->Seek(nBlockOffset);
#endif
	}
	return (nResult==ERROR_SUCCESS) ? true : false;
}

//-------------------------------------------------------------------------------------------
} // namespace monkeyomega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
