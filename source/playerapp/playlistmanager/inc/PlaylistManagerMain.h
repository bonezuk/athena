//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYLISTMANAGER_PLAYLISTMANAGERMAIN_H
#define __ORCUS_PLAYERAPP_PLAYLISTMANAGER_PLAYLISTMANAGERMAIN_H
//-------------------------------------------------------------------------------------------

#include <QGuiApplication>

#include "track/db/inc/TrackDB.h"
#include "track/db/inc/DBInfo.h"

#include "playerapp/playercommon/inc/PlaybackStateController.h"
#include "playerapp/playercommon/inc/PlaylistLoadFunctions.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"

#include "playerapp/playlistmanager/inc/OmegaPLService.h"
#include "playerapp/playlistmanager/inc/OmegaAudioBusInterface.h"
#include "playerapp/playlistmanager/inc/OmegaPLWebService.h"
#include "playerapp/playlistmanager/inc/PlayListWebModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PlaylistManagerApp : public QGuiApplication
{
	Q_OBJECT
	
	public:
		PlaylistManagerApp(int& argc, char **argv);
		virtual ~PlaylistManagerApp();
		
		virtual void initPlaylistManager(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB);
		virtual QSharedPointer<PlaybackStateController>& getPlaybackState();
		virtual QSharedPointer<PlayListWebModel>& getPlayListModel();
		virtual QSharedPointer<OmegaPlaylistInterface>& getPlayListInterface();
		virtual QSharedPointer<OmegaWebInterface> getWebInterface();

	private:
		QSharedPointer<PlayListWebModel> m_pModel;
		QSharedPointer<OmegaAudioBusInterface> m_pAudioInterface;
		QSharedPointer<OmegaPlaylistInterface> m_pPLInterface;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
