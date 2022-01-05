//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_DB_TRACKDBMOUNTPOINTS_H
#define __ORCUS_TRACK_DB_TRACKDBMOUNTPOINTS_H
//-------------------------------------------------------------------------------------------

#include "track/db/inc/SQLiteDatabase.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

class TRACK_DB_EXPORT TrackDBMountPoints
{
	public:
		TrackDBMountPoints(SQLiteDatabase *db);
		virtual ~TrackDBMountPoints();
		
		virtual bool addMountPoint(const QString& mountPointDir);
		virtual QStringList mountPoints();
		
	private:
		SQLiteDatabase *m_db;
		
		virtual void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
