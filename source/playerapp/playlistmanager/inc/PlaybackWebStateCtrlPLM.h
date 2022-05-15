//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_PLAYBACKWEBSTATECTRLPLM_H
#define __PLAYERAPP_PLAYLISTMANAGER_PLAYBACKWEBSTATECTRLPLM_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/PlaybackWebStateController.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class PlaybackWebStateCtrlPLM : public PlaybackWebStateController
{
	Q_OBJECT
	public:
		PlaybackWebStateCtrlPLM(QObject *parent = 0);
		PlaybackWebStateCtrlPLM(QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent = 0);
		virtual ~PlaybackWebStateCtrlPLM();

	protected:
	
		virtual void initWebController();
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
