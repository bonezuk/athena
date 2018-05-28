//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_MONKEYOMEGA_MONKEYOMEGADLL_H
#define __ORCUS_ENGINE_MONKEYOMEGA_MONKEYOMEGADLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_ENGINE_MONKEYOMEGA_DLL)
#define MONKEYOMEGA_EXPORT __declspec(dllexport)
#define MONKEYOMEGA_TEMPLATE
#else
#define MONKEYOMEGA_EXPORT __declspec(dllimport)
#define MONKEYOMEGA_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define MONKEYOMEGA_EXPORT
#define MONKEYOMEGA_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace engine
	{
		namespace monkeyomega
		{
		} // namespace monkeyomega
	} // namespace engine
}// namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
