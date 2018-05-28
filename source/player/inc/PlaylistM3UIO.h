//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYER_PLAYLISTM3UIO_H
#define __ORCUS_PLAYER_PLAYLISTM3UIO_H
//-------------------------------------------------------------------------------------------

#include "player/inc/PlaylistAbstractIO.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

class PlaylistM3UIO : public PlaylistAbstractIO
{
	public:
		PlaylistM3UIO();
		virtual ~PlaylistM3UIO();

		virtual bool load(const QString& fileName,QVector<track::info::InfoSPtr>& pList,QPLProgress *progress);
		virtual bool save(const QString& fileName,const QVector<track::info::InfoSPtr>& pList,QPLProgress *progress);
};

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

