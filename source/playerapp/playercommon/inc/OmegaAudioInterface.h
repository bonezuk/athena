//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERCOMMON_OMEGAAUDIOINTERFACE_H
#define __ORCUS_PLAYERAPP_PLAYERCOMMON_OMEGAAUDIOINTERFACE_H
//-------------------------------------------------------------------------------------------

#include <QString>

#include "common/inc/CommonTypes.h"
#include "playerapp/playercommon/inc/PlayerCommonDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT OmegaAudioInterface
{
	public:
		OmegaAudioInterface();
		
		virtual void playFile(const QString& fileName) = 0;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
