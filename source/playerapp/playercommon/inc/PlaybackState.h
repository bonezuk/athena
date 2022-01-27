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

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT PlaybackState : public QObject
{
	public:
		Q_OBJECT
		Q_PROPERTY(quint32 playbackTimeInSeconds READ getPlaybackTimeInSeconds NOTIFY onPlaybackTimeChanged)
		Q_PROPERTY(qint32 playbackIndex READ getPlaybackIndex NOTIFY onPlaybackIndexChanged)
		Q_PROPERTY(qint32 playbackState READ getPlaybackState NOTIFY onPlaybackStateChanged)

	public:
		enum PlayState
		{
			Pause = 0,
			Play
		};
		Q_ENUM(PlayState)

	public:
		PlaybackState(QObject *parent = 0);
		virtual ~PlaybackState();
		
		quint32 getPlaybackTimeInSeconds() const;
		qint32 getPlaybackIndex() const;
		qint32 getPlaybackState() const;
		
		void setPlaybackTime(quint64 tS);
		void setPlaybackItem(qint32 pbIndex, const QPair<track::db::DBInfoSPtr,tint>& pbItem);
		
		void onAudioStart(const QString& fileName);
		
	signals:
		void onPlaybackTimeChanged();
		void onPlaybackIndexChanged();
		void onPlaybackStateChanged();
		
	private:
		common::TimeStamp m_playbackTime;
		qint32 m_pbIndex;
		QPair<track::db::DBInfoSPtr,tint> m_pbItem;
		enum PlayState m_pbState;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
