#include "playerapp/playerapp/inc/PlayListWebModelPLA.h"
#include "playerapp/playerapp/inc/PlaybackWebStateCtrlPLA.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlayListWebModelPLA::PlayListWebModelPLA(QObject *parent) : PlayListWebModel(parent),
	OmegaWebInterface()
{}

//-------------------------------------------------------------------------------------------

PlayListWebModelPLA::PlayListWebModelPLA(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent) : PlayListWebModel(playList, pAudioInterface, parent),
	OmegaWebInterface()
{}

//-------------------------------------------------------------------------------------------

PlayListWebModelPLA::~PlayListWebModelPLA()
{}

//-------------------------------------------------------------------------------------------

void PlayListWebModelPLA::initialise()
{
	QSharedPointer<PlaybackWebStateCtrlPLA> plState(new PlaybackWebStateCtrlPLA(m_pAudioInterface, this));
	m_pPlaybackState = plState.dynamicCast<PlaybackStateController>();
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
