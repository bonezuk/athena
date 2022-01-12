//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_EMBEDDED_OMEGAPICOMMON_EMBEDDEDENV_H
#define __ORCUS_EMBEDDED_OMEGAPICOMMON_EMBEDDEDENV_H
//-------------------------------------------------------------------------------------------

#include <QString>

#include "embedded/omegapicommon/inc/OmegaPiCommonDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OMEGAPICOMMON_EXPORT void setPluginLocation(const char *appPath);
OMEGAPICOMMON_EXPORT void setupPlatform();
OMEGAPICOMMON_EXPORT void setupSettingsPath();
OMEGAPICOMMON_EXPORT void setupEnviroment(const char *appPath);
OMEGAPICOMMON_EXPORT QString userApplicationDataDirectory();

OMEGAPICOMMON_EXPORT void initCodecs();
OMEGAPICOMMON_EXPORT void releaseCodecs();

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
