//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYLISTMANAGER_PLAYLISTMANAGERMAIN_H
#define __ORCUS_PLAYERAPP_PLAYLISTMANAGER_PLAYLISTMANAGERMAIN_H
//-------------------------------------------------------------------------------------------

#include <QGuiApplication>

#include "track/db/inc/TrackDB.h"
#include "track/db/inc/DBInfo.h"

#include "playerapp/playercommon/inc/PlaybackStateController.h"
#include "playerapp/playercommon/inc/PlayListModel.h"
#include "playerapp/playercommon/inc/PlaylistLoadFunctions.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"

#include "playerapp/playlistmanager/inc/OmegaPLDBusAdaptor.h"
#include "playerapp/playlistmanager/inc/OmegaAudioBusInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PlaylistManagerApp : public QGuiApplication, public OmegaPlaylistInterface
{
	Q_OBJECT
	
	public:
		PlaylistManagerApp(int argc, char **argv);
		virtual ~PlaylistManagerApp();
		
		virtual void initPlaylistManager(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB);
		virtual PlaybackStateController *getPlaybackState();
		virtual PlayListModel *getPlayListModel();
		
		virtual void playbackTime(quint64 tS);
		virtual void onAudioStart(const QString& name);
		virtual void onAudioPlay();
		virtual void onAudioPause();
		virtual void onAudioStop();
		virtual void onAudioBuffer(tfloat32 percent);
		virtual void onAudioReadyForNext();
		virtual void onAudioNoNext();
		virtual void onAudioCrossfade();

	private:
		PlaybackStateController *m_pState;
		PlayListModel *m_pModel;
		OmegaAudioBusInterface *m_pAudioInterface;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
