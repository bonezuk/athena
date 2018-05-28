//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_HTTP_HTTPDLL_H
#define __ORCUS_NETWORK_HTTP_HTTPDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_HTTP_DLL)
#define HTTP_EXPORT __declspec(dllexport)
#define HTTP_TEMPLATE
#else
#define HTTP_EXPORT __declspec(dllimport)
#define HTTP_TEMPLATE
#endif
#elif defined(OMEGA_POSIX)
#define HTTP_EXPORT
#define HTTP_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace http
{
//-------------------------------------------------------------------------------------------
} // namespace http
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
