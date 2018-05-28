//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_MONKEYOMEGA_MONKEYIF_H
#define __ORCUS_ENGINE_MONKEYOMEGA_MONKEYIF_H
//-------------------------------------------------------------------------------------------

#include "engine/monkeyomega/inc/MonkeyOmegaDLL.h"

#include <QString>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace monkeyomega
{
//-------------------------------------------------------------------------------------------

class MONKEYOMEGA_EXPORT MonkeyIF
{
	public:
		MonkeyIF();
		virtual ~MonkeyIF();
		
		bool create(const QString& fileName);
		void destroy();
		
		int getData(char *pBuffer,int nBlocks,int *pBlocksRetrieved);		
		int getInfo(int field,int nParam1 = 0,int nParam2 = 0);
		bool seek(int nBlockOffset);
		
	protected:

#if defined(ORCUS_WIN32)
		HMODULE m_DLL;
		void *m_pInterface;
		void *m_pDecoder;
#elif defined(OMEGA_MACOSX)
		void *m_pDecoder;
#endif

#if defined(ORCUS_WIN32)
		virtual bool getFunctions(HMODULE hMACDll,void *pVoidMACDll);
		virtual bool versionCheckInterface(HMODULE hMACDll);
		virtual bool loadMonkeyLibrary();
		virtual void freeMonkeyLibrary();
#endif

};

//-------------------------------------------------------------------------------------------
} // namespace monkeyomega
} // namespace engine
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
