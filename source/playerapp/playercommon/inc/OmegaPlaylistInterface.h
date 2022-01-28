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
		virtual void onAudioStart(const QString& name) = 0;
		virtual void onAudioPlay() = 0;
		virtual void onAudioPause() = 0;
		virtual void onAudioStop() = 0;
		virtual void onAudioBuffer(tfloat32 percent) = 0;
		virtual void onAudioReadyForNext() = 0;
		virtual void onAudioNoNext() = 0;
		virtual void onAudioCrossfade() = 0;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
