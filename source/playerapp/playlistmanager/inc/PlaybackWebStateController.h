//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_PLAYBACKWEBSTATECONTROLLER_H
#define __PLAYERAPP_PLAYLISTMANAGER_PLAYBACKWEBSTATECONTROLLER_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/PlaybackStateController.h"
#include "playerapp/playlistmanager/inc/WebEventBusInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PlaybackWebStateController : public PlaybackStateController
{
	Q_OBJECT
	public:
		PlaybackWebStateController(QObject *parent = 0);
		PlaybackWebStateController(QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent = 0);
		virtual ~PlaybackWebStateController();
		
		virtual void setTime(quint64 tS);
		
	protected:
	
		WebEventBusInterface *m_pEventInterface;
		
		virtual void initWebController();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
