//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_DB_SCHEMA_H
#define __ORCUS_TRACK_DB_SCHEMA_H
//-------------------------------------------------------------------------------------------

#include "track/db/inc/SQLiteDatabase.h"
#include "track/db/inc/SQLiteQuery.h"
#include "common/inc/Log.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

class TRACK_DB_EXPORT Schema
{
	public:
		Schema();
		virtual ~Schema();
		
		virtual bool createDB(SQLiteDatabase *db);
		
		static bool upgradeVersion4To5(const QString& orgTrackDBFileName);
		
	protected:
	
		SQLiteDatabase *m_db;
		
		virtual void printError(const tchar *strR,const tchar *strE) const;
		
		virtual bool isTableDefined(const QString& tableName);
		
		virtual void createAlbum();
		virtual void createTrack();
		virtual void createSubtrack();
		virtual void createGenre();
		virtual void createImage();
		virtual void createImageMap();
		virtual void createImageAlbumMap();
		virtual void createDirectory();
		virtual void createFile();
		virtual void createPlayList();
		virtual void createPlayListInfo();
		virtual void createDatabaseInfo();
		virtual void createSandBoxURL();
		virtual bool writeDatabaseVersion(int versionNo);
		
		virtual bool upgradeVersion1To2();
		
		static QString tempUpgradeDBFileName(const QString& orgTrackDBFileName);
		static bool upgradeVersion4To5CopySandbox(SQLiteDatabase *orgDB,SQLiteDatabase *newDB);
};

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

