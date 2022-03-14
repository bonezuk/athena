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
		
		virtual quint32 getTimeInSeconds() const;
		virtual qint32 getIndex() const;
		virtual qint32 getState() const;
		
		virtual void setTime(quint64 tS);
		virtual void setNextItem(qint32 pbIndex, const QPair<track::db::DBInfoSPtr,tint>& pbItem);
		
		virtual void onAudioStart(const QString& fileName);
		virtual void onAudioPlay();
		virtual void onAudioPause();
		virtual void onAudioStop();

		virtual void resumeOrPausePlayback();

	signals:
		void onTimeChanged();
		void onIndexChanged();
		void onStateChanged();
		
	protected:
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
