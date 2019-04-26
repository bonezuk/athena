//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_DAEMON_DAEMONDLL_H
#define __ORCUS_DAEMON_DAEMONDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_DAEMON_DLL)
#define DAEMON_EXPORT __declspec(dllexport)
#define DAEMON_TEMPLATE
#else
#define DAEMON_EXPORT __declspec(dllimport)
#define DAEMON_TEMPLATE
#endif
#elif defined(OMEGA_POSIX)
#define DAEMON_EXPORT
#define DAEMON_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace daemon
{
//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
