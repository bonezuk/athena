//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_PLAYERCOMMON_PLAYERCOMMONDLL_H
#define __OMEGA_PLAYERAPP_PLAYERCOMMON_PLAYERCOMMONDLL_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"
#include <QThread>

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
#if defined(OMEGA_PLAYERCOMMON_DLL)
#define PLAYERCOMMON_EXPORT __declspec(dllexport)
#define PLAYERCOMMON_TEMPLATE
#else
#define PLAYERCOMMON_EXPORT __declspec(dllimport)
#define PLAYERCOMMON_TEMPLATE extern
#endif
#elif defined(OMEGA_POSIX)
#define PLAYERCOMMON_EXPORT
#define PLAYERCOMMON_TEMPLATE
#endif

//-------------------------------------------------------------------------------------------
namespace omega
{
}// namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
