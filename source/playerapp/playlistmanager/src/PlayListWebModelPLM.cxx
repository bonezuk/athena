#include "playerapp/playlistmanager/inc/PlayListWebModelPLM.h"
#include "playerapp/playlistmanager/inc/PlaybackWebStateCtrlPLM.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

PlayListWebModelPLM::PlayListWebModelPLM(QObject *parent) : PlayListWebModel(parent)
{}

//-------------------------------------------------------------------------------------------

PlayListWebModelPLM::PlayListWebModelPLM(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent) : PlayListWebModel(playList, pAudioInterface, parent)
{}

//-------------------------------------------------------------------------------------------

PlayListWebModelPLM::~PlayListWebModelPLM()
{}

//-------------------------------------------------------------------------------------------

bool PlayListWebModelPLM::initialise()
{
	QSharedPointer<PlaybackWebStateCtrlPLM> plState(new PlaybackWebStateCtrlPLM(m_pAudioInterface, this));
	m_pPlaybackState = plState.dynamicCast<PlaybackStateController>();
	plState->init();
	return true;
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
