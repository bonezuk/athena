//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_REMOTE_REMOTEDLL_H
#define __ORCUS_REMOTE_REMOTEDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_REMOTE_DLL)
#define REMOTE_EXPORT __declspec(dllexport)
#define REMOTE_TEMPLATE
#else
#define REMOTE_EXPORT __declspec(dllimport)
#define REMOTE_TEMPLATE extern
#endif
#else
#define REMOTE_EXPORT
#define REMOTE_TEMPLATE extern
#endif

//-------------------------------------------------------------------------------------------

namespace orcus
{
	namespace remote
	{
	} // namespace remote
} // namespace orcus

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
