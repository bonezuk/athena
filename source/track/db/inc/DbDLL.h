//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_DB_DBDLL_H
#define __ORCUS_TRACK_DB_DBDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_TRACK_DB_DLL)
#define TRACK_DB_EXPORT __declspec(dllexport)
#define TRACK_DB_TEMPLATE
#else
#define TRACK_DB_EXPORT __declspec(dllimport)
#define TRACK_DB_TEMPLATE extern
#endif
#else
#define TRACK_DB_EXPORT
#define TRACK_DB_TEMPLATE extern
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace track
	{
		namespace db
		{} // namespace db
	} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
