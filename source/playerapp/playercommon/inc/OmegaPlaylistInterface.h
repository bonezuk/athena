//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERCOMMON_OMEGAPLAYLISTINTERFACE_H
#define __ORCUS_PLAYERAPP_PLAYERCOMMON_OMEGAPLAYLISTINTERFACE_H
//-------------------------------------------------------------------------------------------

#include <QtGlobal>

#include "common/inc/CommonTypes.h"
#include "playerapp/playercommon/inc/PlayerCommonDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT OmegaPlaylistInterface
{
	public:
		OmegaPlaylistInterface();
		
		virtual void playbackTime(quint64 tS) = 0;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
