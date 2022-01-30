//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERAPP_PLAYERAPPMAIN_H
#define __ORCUS_PLAYERAPP_PLAYERAPP_PLAYERAPPMAIN_H
//-------------------------------------------------------------------------------------------

#include <QGuiApplication>

#include "track/db/inc/TrackDB.h"
#include "track/db/inc/DBInfo.h"

#include "playerapp/playercommon/inc/PlaybackStateController.h"
#include "playerapp/playercommon/inc/PlayListModel.h"
#include "playerapp/playercommon/inc/PlaylistLoadFunctions.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"

#include "playerapp/playerapp/inc/OmegaAudioIOInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PlayerAppMain : public QGuiApplication, public OmegaPlaylistInterface
{
	Q_OBJECT
	
	public:
		PlayerAppMain(int& argc, char **argv);
		virtual ~PlayerAppMain();
		
		virtual bool initPlaylistManager(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB);
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
		OmegaAudioIOInterface *m_pAudioInterface;
		
		virtual void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
