#include "playerapp/playlistmanager/inc/PlaybackWebStateCtrlPLM.h"
#include "playerapp/playlistmanager/inc/WebEventBusInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlaybackWebStateCtrlPLM::PlaybackWebStateCtrlPLM(QObject *parent) : PlaybackWebStateController(parent)
{}

//-------------------------------------------------------------------------------------------

PlaybackWebStateCtrlPLM::PlaybackWebStateCtrlPLM(QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent) : PlaybackWebStateController(pAudioInterface, parent)
{}

//-------------------------------------------------------------------------------------------

PlaybackWebStateCtrlPLM::~PlaybackWebStateCtrlPLM()
{}

//-------------------------------------------------------------------------------------------

void PlaybackWebStateCtrlPLM::initWebController()
{
	m_pEventInterface = new WebEventBusInterface();
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
