//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_NETWORK_NETWORKDLL_H
#define __OMEGA_NETWORK_NETWORKDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(OMEGA_NETWORK_DLL)
#define NETWORK_EXPORT __declspec(dllexport)
#define NETWORK_TEMPLATE
#else
#define NETWORK_EXPORT __declspec(dllimport)
#define NETWORK_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define NETWORK_EXPORT
#define NETWORK_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace network
{
//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
