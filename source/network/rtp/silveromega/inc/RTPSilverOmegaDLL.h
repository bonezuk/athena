//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_RTP_SILVEROMEGA_RTPSILVEROMEGADLL_H
#define __ORCUS_NETWORK_RTP_SILVEROMEGA_RTPSILVEROMEGADLL_H
//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(ORCUS_NETWORK_RTPSILVEROMEGA_DLL)
#define RTPSILVEROMEGA_EXPORT __declspec(dllexport)
#define RTPSILVEROMEGA_TEMPLATE
#else
#define RTPSILVEROMEGA_EXPORT __declspec(dllimport)
#define RTPSILVEROMEGA_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define RTPSILVEROMEGA_EXPORT
#define RTPSILVEROMEGA_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace network
	{
		namespace rtp
		{
			namespace silveromega
			{
			} // namespace silveromega
		} // namespace rtp
	} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
