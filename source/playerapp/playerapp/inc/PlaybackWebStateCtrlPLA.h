//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERAPP_PLAYBACKWEBSTATECTRLPLA_H
#define __PLAYERAPP_PLAYERAPP_PLAYBACKWEBSTATECTRLPLA_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/PlaybackWebStateController.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PlaybackWebStateCtrlPLA : public PlaybackWebStateController
{
	Q_OBJECT
	public:
		PlaybackWebStateCtrlPLA(QObject *parent = 0);
		PlaybackWebStateCtrlPLA(QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent = 0);
		virtual ~PlaybackWebStateCtrlPLA();

	protected:
	
		virtual void initWebController();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
