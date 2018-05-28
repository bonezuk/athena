//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_INFO_INFODLL_H
#define __ORCUS_TRACK_INFO_INFODLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_TRACK_INFO_DLL)
#define TRACK_INFO_EXPORT __declspec(dllexport)
#define TRACK_INFO_TEMPLATE
#else
#define TRACK_INFO_EXPORT __declspec(dllimport)
#define TRACK_INFO_TEMPLATE extern
#endif
#else
#define TRACK_INFO_EXPORT
#define TRACK_INFO_TEMPLATE extern
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace track
	{
		namespace info
		{} // namespace info
	} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

