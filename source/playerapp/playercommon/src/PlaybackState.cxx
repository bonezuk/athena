#include "playerapp/playercommon/inc/PlaybackState.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlaybackState::PlaybackState(QObject *parent) : QObject(parent),
	m_playbackTime(),
	m_pbIndex(-1),
	m_pbItem(),
	m_pbState(Pause)
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

void PlaybackState::setPlaybackItem(qint32 pbIndex, const QPair<track::db::DBInfoSPtr,tint>& pbItem)
{
	m_pbIndex = pbIndex;
	m_pbItem = pbItem;
}

//-------------------------------------------------------------------------------------------

qint32 PlaybackState::getPlaybackIndex() const
{
	return m_pbIndex;
}

//-------------------------------------------------------------------------------------------

qint32 PlaybackState::getPlaybackState() const
{
	return static_cast<qint32>(m_pbState);
}

//-------------------------------------------------------------------------------------------

void PlaybackState::onAudioStart(const QString& fileName)
{
	if(m_pbIndex >= 0 && !m_pbItem.first.isNull())
	{
		if(fileName == m_pbItem.first->getFilename())
		{
			if(m_pbState != Play)
			{
				m_pbState = Pause;
				emit onPlaybackStateChanged();
			}
			emit onPlaybackIndexChanged();
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
