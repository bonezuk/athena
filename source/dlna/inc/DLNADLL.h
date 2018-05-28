//-------------------------------------------------------------------------------------------
#ifndef __DLNADLL_H
#define __DLNADLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_DLNA_DLL)
#define DLNA_EXPORT __declspec(dllexport)
#define DLNA_TEMPLATE
#else
#define DLNA_EXPORT __declspec(dllimport)
#define DLNA_TEMPLATE extern
#endif
#else
#define DLNA_EXPORT
#define DLNA_TEMPLATE extern
#endif

//-------------------------------------------------------------------------------------------

namespace orcus
{
	namespace dlna
	{
	} // namespace dlna
} // namespace orcus

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
