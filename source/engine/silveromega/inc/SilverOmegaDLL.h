//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_SILVEROMEGA_SILVEROMEGADLL_H
#define __ORCUS_ENGINE_SILVEROMEGA_SILVEROMEGADLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_ENGINE_SILVEROMEGA_DLL)
#define SILVEROMEGA_EXPORT __declspec(dllexport)
#define SILVEROMEGA_TEMPLATE
#else
#define SILVEROMEGA_EXPORT __declspec(dllimport)
#define SILVEROMEGA_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define SILVEROMEGA_EXPORT
#define SILVEROMEGA_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace engine
	{
		namespace silveromega
		{
		} // namespace silveromega
	} // namespace engine
}// namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
