//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_WHITEOMEGA_WHITEOMEGADLL_H
#define __ORCUS_ENGINE_WHITEOMEGA_WHITEOMEGADLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_ENGINE_WHITEOMEGA_DLL)
#define WHITEOMEGA_EXPORT __declspec(dllexport)
#define WHITEOMEGA_TEMPLATE
#else
#define WHITEOMEGA_EXPORT __declspec(dllimport)
#define WHITEOMEGA_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define WHITEOMEGA_EXPORT
#define WHITEOMEGA_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace engine
	{
		namespace whiteomega
		{
		} // namespace whiteomega
	} // namespace engine
}// namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
