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

class PlayerAppMain : public QGuiApplication
{
	Q_OBJECT
	
	public:
		PlayerAppMain(int& argc, char **argv);
		virtual ~PlayerAppMain();
		
		virtual bool initPlaylistManager(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB);
		virtual QSharedPointer<PlaybackStateController>& getPlaybackState();
		virtual QSharedPointer<PlayListModel>& getPlayListModel();
		
	private:
		QSharedPointer<PlayListModel> m_pModel;
		QSharedPointer<OmegaAudioIOInterface> m_pAudioInterface;
		QSharedPointer<OmegaPlaylistInterface> m_pPLInterface;
		
		virtual void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
