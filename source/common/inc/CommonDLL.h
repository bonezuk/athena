#ifndef __ORCUS_COMMON_COMMONDLL_H
#define __ORCUS_COMMON_COMMONDLL_H

#if defined(OMEGA_WIN32)

#define OMEGA_EXPORT_TEMPLATE extern
#define OMEGA_TEMPLATE_MEMBER

#if defined(ORCUS_COMMON_DLL)
#define COMMON_EXPORT __declspec(dllexport)
#define COMMON_TEMPLATE
#else
#define COMMON_EXPORT __declspec(dllimport)
#define COMMON_TEMPLATE extern
#endif

#elif defined(OMEGA_POSIX)

#define COMMON_EXPORT
#define COMMON_TEMPLATE
#define OMEGA_EXPORT_TEMPLATE extern
#define OMEGA_TEMPLATE_MEMBER template<>

#endif

//-------------------------------------------------------------------------------------------
namespace orcus
{
	namespace common
	{
	} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------

#endif
