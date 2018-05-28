//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_VIOLETOMEGA_VIOLETOMEGADLL_H
#define __ORCUS_ENGINE_VIOLETOMEGA_VIOLETOMEGADLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_ENGINE_VIOLETOMEGA_DLL)
#define VIOLETOMEGA_EXPORT __declspec(dllexport)
#define VIOLETOMEGA_TEMPLATE
#else
#define VIOLETOMEGA_EXPORT __declspec(dllimport)
#define VIOLETOMEGA_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define VIOLETOMEGA_EXPORT
#define VIOLETOMEGA_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace engine
	{
		namespace violetomega
		{
		} // namespace violetomega
	} // namespace engine
}// namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
