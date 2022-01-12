//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_EMBEDDED_OMEGAPICOMMON_OMEGAPICOMMONDLL_H
#define __ORCUS_EMBEDDED_OMEGAPICOMMON_OMEGAPICOMMONDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_OMEGAPICOMMON_DLL)
#define OMEGAPICOMMON_EXPORT __declspec(dllexport)
#define OMEGAPICOMMON_TEMPLATE
#else
#define OMEGAPICOMMON_EXPORT __declspec(dllimport)
#define OMEGAPICOMMON_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define OMEGAPICOMMON_EXPORT
#define OMEGAPICOMMON_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
}// namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
