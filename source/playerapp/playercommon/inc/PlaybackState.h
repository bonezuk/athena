//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERCOMMON_PLAYBACKSTATE_H
#define __ORCUS_PLAYERAPP_PLAYERCOMMON_PLAYBACKSTATE_H
//-------------------------------------------------------------------------------------------

#include <QtGlobal>
#include <QObject>
#include <QQmlEngine>

#include "common/inc/TimeStamp.h"
#include "playerapp/playercommon/inc/PlayerCommonDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT PlaybackState : public QObject
{
	Q_OBJECT
	Q_PROPERTY(quint32 playbackTimeInSeconds READ getPlaybackTimeInSeconds NOTIFY onPlaybackTimeChanged)
	public:
		PlaybackState(QObject *parent = 0);
		virtual ~PlaybackState();
		
		quint32 getPlaybackTimeInSeconds() const;
		
		void setPlaybackTime(quint64 tS);
		
	signals:
		void onPlaybackTimeChanged();
		
	private:
		common::TimeStamp m_playbackTime;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
