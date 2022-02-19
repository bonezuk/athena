//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERCOMMON_PLAYBACKSTATE_H
#define __ORCUS_PLAYERAPP_PLAYERCOMMON_PLAYBACKSTATE_H
//-------------------------------------------------------------------------------------------

#include <QtGlobal>
#include <QObject>
#include <QQmlEngine>

#include "common/inc/TimeStamp.h"
#include "track/db/inc/DBInfo.h"
#include "track/db/inc/TrackDB.h"
#include "playerapp/playercommon/inc/PlayerCommonDLL.h"
#include "playerapp/playercommon/inc/OmegaAudioInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT PlaybackStateController : public QObject
{
	public:
		Q_OBJECT
		Q_PROPERTY(quint32 timeInSeconds READ getTimeInSeconds NOTIFY onTimeChanged)
		Q_PROPERTY(qint32 index READ getIndex NOTIFY onIndexChanged)
		Q_PROPERTY(qint32 state READ getState NOTIFY onStateChanged)

	public:
		enum PlayState
		{
			Pause = 0,
			Play
		};
		Q_ENUM(PlayState)

	public:
		PlaybackStateController(QObject *parent = 0);
		PlaybackStateController(QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent = 0);
		virtual ~PlaybackStateController();
		
		quint32 getTimeInSeconds() const;
		qint32 getIndex() const;
		qint32 getState() const;
		
		void setTime(quint64 tS);
		void setNextItem(qint32 pbIndex, const QPair<track::db::DBInfoSPtr,tint>& pbItem);
		
		void onAudioStart(const QString& fileName);
		void onAudioPlay();
		void onAudioPause();
		void onAudioStop();

		void resumeOrPausePlayback();

	signals:
		void onTimeChanged();
		void onIndexChanged();
		void onStateChanged();
		
	private:
		QSharedPointer<OmegaAudioInterface> m_pAudioInterface;
		common::TimeStamp m_playbackTime;

		qint32 m_pbIndex;
		QPair<track::db::DBInfoSPtr,tint> m_pbItem;
		
		qint32 m_pbNextIndex;
		QPair<track::db::DBInfoSPtr, tint> m_pbNextItem;
		
		enum PlayState m_pbState;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
