#include "playerapp/playercommon/inc/PlaybackState.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlaybackState::PlaybackState(QObject *parent) : QObject(parent),
	m_playbackTime()
{}

//-------------------------------------------------------------------------------------------

PlaybackState::~PlaybackState()
{}

//-------------------------------------------------------------------------------------------

quint32 PlaybackState::getPlaybackTimeInSeconds() const
{
	return m_playbackTime.secondsTotal();
}

//-------------------------------------------------------------------------------------------

void PlaybackState::setPlaybackTime(quint64 tS)
{
	tuint32 lS;
	
	lS = m_playbackTime.secondsTotal();
	m_playbackTime = tS;
	if(lS != m_playbackTime.secondsTotal())
	{
		emit onPlaybackTimeChanged();
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
