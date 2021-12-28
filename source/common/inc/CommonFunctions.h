//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_COMMON_COMMONFUNCTIONS_H
#define __ORCUS_COMMON_COMMONFUNCTIONS_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"
#include "common/inc/BString.h"

#include <QString>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace common
{
//-------------------------------------------------------------------------------------------

COMMON_EXPORT QString getOSErrorString(tint errCode);

COMMON_EXPORT void msleepThread(tint msecs);
COMMON_EXPORT void usleepThread(tint usecs);

COMMON_EXPORT void loadSharedLibrary(const char *libName);

//-------------------------------------------------------------------------------------------
} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
