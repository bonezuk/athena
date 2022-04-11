#include "playerapp/playlistmanager/inc/PlayListWebModelPLM.h"
#include "playerapp/playlistmanager/inc/PlaybackWebStateCtrlPLM.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlayListWebModelPLM::PlayListWebModelPLM(QObject *parent) : PlayListWebModel(parent),
	OmegaWebInterface()
{}

//-------------------------------------------------------------------------------------------

PlayListWebModelPLM::PlayListWebModelPLM(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent) : PlayListWebModel(playList, pAudioInterface, parent),
	OmegaWebInterface()
{}

//-------------------------------------------------------------------------------------------

PlayListWebModelPLM::~PlayListWebModelPLM()
{}

//-------------------------------------------------------------------------------------------

void PlayListWebModelPLM::initialise()
{
	QSharedPointer<PlaybackWebStateCtrlPLM> plState(new PlaybackWebStateCtrlPLM(m_pAudioInterface, this));
	m_pPlaybackState = plState.dynamicCast<PlaybackStateController>();
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
