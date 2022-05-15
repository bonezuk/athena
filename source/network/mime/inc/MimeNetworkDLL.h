//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_NETWORK_MIME_MIMEDLL_H
#define __OMEGA_NETWORK_MIME_MIMEDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(OMEGA_MIME_DLL)
#define MIME_EXPORT __declspec(dllexport)
#define MIME_TEMPLATE
#else
#define MIME_EXPORT __declspec(dllimport)
#define MIME_TEMPLATE
#endif
#elif defined(OMEGA_POSIX)
#define MIME_EXPORT
#define MIME_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace network
{
namespace mime
{
//-------------------------------------------------------------------------------------------
} // namespace mime
} // namespace network
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
