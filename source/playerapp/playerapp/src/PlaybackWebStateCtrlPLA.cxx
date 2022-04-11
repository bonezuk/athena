#include "playerapp/playerapp/inc/PlaybackWebStateCtrlPLA.h"
#include "playerapp/playerapp/inc/OmegaWebEventHandler.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlaybackWebStateCtrlPLA::PlaybackWebStateCtrlPLA(QObject *parent) : PlaybackWebStateController(parent)
{}

//-------------------------------------------------------------------------------------------

PlaybackWebStateCtrlPLA::PlaybackWebStateCtrlPLA(QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent) : PlaybackWebStateController(pAudioInterface, parent)
{}

//-------------------------------------------------------------------------------------------

PlaybackWebStateCtrlPLA::~PlaybackWebStateCtrlPLA()
{}

//-------------------------------------------------------------------------------------------

void PlaybackWebStateCtrlPLA::initWebController()
{
	m_pEventInterface = new OmegaWebEventHandler();
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
