#include "playerapp/playercommon/inc/PlaybackStateController.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlaybackStateController::PlaybackStateController(QObject *parent) : QObject(parent),
	m_pAudioInterface(0),
	m_playbackTime(),
	m_pbIndex(-1),
	m_pbItem(),
	m_pbState(Pause)
{}

//-------------------------------------------------------------------------------------------

PlaybackStateController::PlaybackStateController(OmegaAudioInterface *pAudioInterface, QObject *parent) : QObject(parent),
	m_pAudioInterface(pAudioInterface),
	m_playbackTime(),
	m_pbIndex(-1),
	m_pbItem(),
	m_pbState(Pause)
{}

//-------------------------------------------------------------------------------------------

PlaybackStateController::~PlaybackStateController()
{}

//-------------------------------------------------------------------------------------------

quint32 PlaybackStateController::getTimeInSeconds() const
{
	return m_playbackTime.secondsTotal();
}

//-------------------------------------------------------------------------------------------

void PlaybackStateController::setTime(quint64 tS)
{
	tuint32 lS;
	
	lS = m_playbackTime.secondsTotal();
	m_playbackTime = tS;
	if(lS != m_playbackTime.secondsTotal())
	{
		emit onTimeChanged();
	}
}

//-------------------------------------------------------------------------------------------

void PlaybackStateController::setItem(qint32 pbIndex, const QPair<track::db::DBInfoSPtr,tint>& pbItem)
{
	m_pbIndex = pbIndex;
	m_pbItem = pbItem;
}

//-------------------------------------------------------------------------------------------

qint32 PlaybackStateController::getIndex() const
{
	return m_pbIndex;
}

//-------------------------------------------------------------------------------------------

qint32 PlaybackStateController::getState() const
{
	return static_cast<qint32>(m_pbState);
}

//-------------------------------------------------------------------------------------------

void PlaybackStateController::onAudioStart(const QString& fileName)
{
	if(m_pbIndex >= 0 && !m_pbItem.first.isNull())
	{
		if(fileName == m_pbItem.first->getFilename())
		{
			if(m_pbState == Pause)
			{
				m_pbState = Play;
				emit onStateChanged();
			}
			emit onIndexChanged();
		}
	}
}

//-------------------------------------------------------------------------------------------

void PlaybackStateController::onPlayPausePressed()
{
	if(m_pAudioInterface != 0)
	{
		if(m_pbState == Play)
		{
			m_pAudioInterface->pause();
		}
		else
		{
			m_pAudioInterface->play();
		}
	}
}

//-------------------------------------------------------------------------------------------

void PlaybackStateController::onAudioPlay()
{
	if(m_pbState == Pause)
	{
		m_pbState = Play;
		emit onStateChanged();
	}
}

//-------------------------------------------------------------------------------------------

void PlaybackStateController::onAudioPause()
{
	if(m_pbState == Play)
	{
		m_pbState = Pause;
		emit onStateChanged();
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
