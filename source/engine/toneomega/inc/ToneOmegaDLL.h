//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_TONEOMEGA_TONEOMEGADLL_H
#define __ORCUS_ENGINE_TONEOMEGA_TONEOMEGADLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_ENGINE_TONEOMEGA_DLL)
#define TONEOMEGA_EXPORT __declspec(dllexport)
#define TONEOMEGA_TEMPLATE
#else
#define TONEOMEGA_EXPORT __declspec(dllimport)
#define TONEOMEGA_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define TONEOMEGA_EXPORT
#define TONEOMEGA_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace engine
	{
		namespace toneomega
		{
		} // namespace toneomega
	} // namespace engine
}// namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
