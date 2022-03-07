//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERCOMMON_EMBEDDEDENV_H
#define __ORCUS_PLAYERAPP_PLAYERCOMMON_EMBEDDEDENV_H
//-------------------------------------------------------------------------------------------

#include <QString>

#include "playerapp/playercommon/inc/PlayerCommonDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PLAYERCOMMON_EXPORT void setPluginLocation(const char *appPath);
PLAYERCOMMON_EXPORT void setupPlatform();
PLAYERCOMMON_EXPORT void setupSettingsPath();
PLAYERCOMMON_EXPORT void setupEnviroment(const char *appPath);
PLAYERCOMMON_EXPORT QString userApplicationDataDirectory();

PLAYERCOMMON_EXPORT void initCodecs();
PLAYERCOMMON_EXPORT void releaseCodecs();

PLAYERCOMMON_EXPORT QString pathToUNIXSocket(const QString& serviceName);

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
