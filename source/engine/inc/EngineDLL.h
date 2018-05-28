//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_ENGINEDLL_H
#define __ORCUS_ENGINE_ENGINEDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_ENGINE_DLL)
#define ENGINE_EXPORT __declspec(dllexport)
#define ENGINE_TEMPLATE
#else
#define ENGINE_EXPORT __declspec(dllimport)
#define ENGINE_TEMPLATE extern
#endif
#else
#define ENGINE_EXPORT
#define ENGINE_TEMPLATE extern
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace engine
	{
	} // namespace engine
} // namespace orcus

//-------------------------------------------------------------------------------------------

#endif
//-------------------------------------------------------------------------------------------
