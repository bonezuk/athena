//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_REDOMEGA_REDOMEGADLL_H
#define __ORCUS_ENGINE_REDOMEGA_REDOMEGADLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_ENGINE_REDOMEGA_DLL)
#define REDOMEGA_EXPORT __declspec(dllexport)
#define REDOMEGA_TEMPLATE
#else
#define REDOMEGA_EXPORT __declspec(dllimport)
#define REDOMEGA_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define REDOMEGA_EXPORT
#define REDOMEGA_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace engine
	{
		namespace redomega
		{
		} // namespace redomega
	} // namespace engine
}// namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
