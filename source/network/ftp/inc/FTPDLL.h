//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_FTP_FTPDLL_H
#define __ORCUS_NETWORK_FTP_FTPDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_FTP_DLL)
#define FTP_EXPORT __declspec(dllexport)
#define FTP_TEMPLATE
#else
#define FTP_EXPORT __declspec(dllimport)
#define FTP_TEMPLATE
#endif
#elif defined(OMEGA_POSIX)
#define FTP_EXPORT
#define FTP_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace ftp
{
//-------------------------------------------------------------------------------------------
} // namespace ftp
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
