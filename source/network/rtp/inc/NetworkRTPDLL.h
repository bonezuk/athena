//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_RTP_NETWORKRTPDLL_H
#define __ORCUS_NETWORK_RTP_NETWORKRTPDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_NETWORKRTP_DLL)
#define NETWORKRTP_EXPORT __declspec(dllexport)
#define NETWORKRTP_TEMPLATE
#else
#define NETWORKRTP_EXPORT __declspec(dllimport)
#define NETWORKRTP_TEMPLATE extern
#endif
#else
#define NETWORKRTP_EXPORT
#define NETWORKRTP_TEMPLATE extern
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace network
	{
		namespace rtp
		{
		} // namespace rtp
	} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
