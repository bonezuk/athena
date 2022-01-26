//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERCOMMON_PLAYLISTLOADFUNCTIONS_H
#define __ORCUS_PLAYERAPP_PLAYERCOMMON_PLAYLISTLOADFUNCTIONS_H
//-------------------------------------------------------------------------------------------

#include "track/db/inc/TrackDB.h"
#include "track/db/inc/DBInfo.h"
#include "track/db/inc/PlaylistAbstractIO.h"

#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PLAYERCOMMON_EXPORT QString mountPointFromArguments();
PLAYERCOMMON_EXPORT QString playlistFromArguments();
PLAYERCOMMON_EXPORT void playlistToDBList(QVector<track::info::InfoSPtr>& playList, QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB);
PLAYERCOMMON_EXPORT void playlistSubtrackToDBList(QVector<QPair<track::info::InfoSPtr, tint> >& playList, QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB);
PLAYERCOMMON_EXPORT bool loadPlaylistFromDBOrArguments(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB);

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
