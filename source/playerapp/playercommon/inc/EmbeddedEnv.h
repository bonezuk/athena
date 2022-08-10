//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_PLAYERCOMMON_EMBEDDEDENV_H
#define __OMEGA_PLAYERAPP_PLAYERCOMMON_EMBEDDEDENV_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/PlayerCommonDLL.h"

#include <QString>

//-------------------------------------------------------------------------------------------
namespace omega
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
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
