//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_DB_PLAYLISTM3UIO_H
#define __ORCUS_TRACK_DB_PLAYLISTM3UIO_H
//-------------------------------------------------------------------------------------------

#include "track/db/inc/PlaylistAbstractIO.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

class TRACK_DB_EXPORT PlaylistM3UIO : public PlaylistAbstractIO
{
	public:
		PlaylistM3UIO();
		virtual ~PlaylistM3UIO();

		virtual bool load(const QString& fileName,QVector<track::info::InfoSPtr>& pList,PLProgress *progress);
		virtual bool save(const QString& fileName,const QVector<track::info::InfoSPtr>& pList,PLProgress *progress);
	
	protected:
#if defined(OMEGA_MAC_STORE)
		virtual void getPermissionsM3U(PLProgress *progress,const QString& fileName)
#endif
};

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
