//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_DB_TRACKDB_H
#define __ORCUS_TRACK_DB_TRACKDB_H
//-------------------------------------------------------------------------------------------

#include "track/info/inc/Info.h"
#include "track/db/inc/Schema.h"
#include "track/db/inc/SQLiteInsert.h"
#include "track/db/inc/TrackDBMountPoints.h"

#include <QSharedPointer>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

class DBInfo;

//-------------------------------------------------------------------------------------------

class TRACK_DB_EXPORT TrackDB
{
	public:
		friend class DBInfo;
	
	public:
		~TrackDB();
		
		static TrackDB *instance();
		static TrackDB *instance(const QString& dbName);
		
		static common::TimeStamp getModifiedTime(const QString& fileName);
		static common::TimeStamp getModifiedTime(const QString& fileName,tint& fileSize);
			
		bool add(info::Info *data);
		bool addInfo(info::Info *data);
		
		bool erase(const QString& fileName);
		bool erase(tint albumID,tint trackID);
		
		bool isUpdateRequired(tint dirID,tint fileID);
		bool update(info::Info *data);
		
		static QString dbString(const QString& dStr);
		static QString dbStringInv(const QString& dStr);
		
		SQLiteDatabase *db();
		
		tint groupAlbums(const QVector<tint>& albumIDList);

		bool getTrackKey(const QString& fileName,QPair<tint,tint>& pairID);
		
		int idOfInserted();
		
		int dbVersion();
		
		QSharedPointer<TrackDBMountPoints>& mountPoints();
		
	protected:
		
		static TrackDB *m_instance;
		
		SQLiteDatabase *m_db;
		QSharedPointer<TrackDBMountPoints> m_mountPoints;
		
		TrackDB();
		
		void printError(const tchar *strR,const tchar *strE) const;

		bool open(const QString& dbName);
		void close();
		
		tint addAlbum(info::Info *data,tint& dirID);
		tint addTrack(info::Info *data,tint dirID,tint albumID);
		tint addGenre(info::Info *data);
		tint addDirectory(info::Info *data);
		tint addDirectory(const QString& dName);
		tint addFile(info::Info *data,tint dirID);
		bool addImage(info::Info *data,tint albumID,tint trackID);
		int addImageFile(const QString& fileName);
		bool addSubtrack(info::Info *data,tint albumID,tint trackID);
		QPair<tuint64,tuint64> getSHA1Signature(common::Array<tubyte,tubyte> *pArr);
		
		bool eraseTrack(tint albumID,tint trackID,tint dirID,tint fileID);
		bool getKeysFromFilename(const QString& fullFileName,tint& albumID,tint& trackID);
		bool getKeysFromFilename(const QString& fullFileName,tint& albumID,tint& trackID,tint& dirID,tint& fileID);
		bool getFilenameComponents(const QString& name,QString& dirName,QString& fileName);
		
		bool updateAlbumImage(int albumID);
		tint levenshteinDistance(const QString& a,const QString& b) const;
		QString loadDirectoryImage(const QString& dirName,const QString& albumName) const;
};

//-------------------------------------------------------------------------------------------

inline SQLiteDatabase *TrackDB::db()
{
	return m_db;
}

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

