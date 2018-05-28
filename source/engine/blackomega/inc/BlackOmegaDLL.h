//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_BLACKOMEGA_BLACKOMEGADLL_H
#define __ORCUS_ENGINE_BLACKOMEGA_BLACKOMEGADLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_ENGINE_BLACKOMEGA_DLL)
#define BLACKOMEGA_EXPORT __declspec(dllexport)
#define BLACKOMEGA_TEMPLATE
#else
#define BLACKOMEGA_EXPORT __declspec(dllimport)
#define BLACKOMEGA_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define BLACKOMEGA_EXPORT
#define BLACKOMEGA_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace engine
	{
		namespace blackomega
		{
		} // namespace blackomega
	} // namespace engine
} // namespace orcus

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
