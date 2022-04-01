#include "track/db/inc/Schema.h"
#include "track/db/inc/SQLiteInsert.h"
#include "track/db/inc/TrackDB.h"
#include "common/inc/DiskOps.h"

#include <QFile>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

Schema::Schema() : m_db(0)
{}

//-------------------------------------------------------------------------------------------

Schema::~Schema()
{}

//-------------------------------------------------------------------------------------------

void Schema::printError(const tchar *strR,const tchar *strE) const
{
	common::Log::g_Log << "Schema::" << strR << " - " << strE << "." << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool Schema::isTableDefinedOps(SQLiteDatabase *db, const QString& tableName)
{
	QString cmd;
	SQLiteQuery qTable(db);
	
	cmd = "SELECT name FROM sqlite_master WHERE type=\"table\" AND name=\"" + tableName + "\"";
	qTable.prepare(cmd);
	return (qTable.next()) ? true : false;
}

//-------------------------------------------------------------------------------------------

bool Schema::isTableDefined(const QString& tableName)
{
	return isTableDefinedOps(m_db, tableName);
}

//-------------------------------------------------------------------------------------------

bool Schema::createDB(SQLiteDatabase *db)
{
	bool res = true;
	
	if(db!=0)
	{
		try
		{
			m_db = db;
			createDatabaseInfo();
			if(writeDatabaseVersion(TRACKDB_VERSION))
			{
				createAlbum();
				createTrack();
				createSubtrack();
				createGenre();
				createImage();
				createImageMap();
				createImageAlbumMap();
				createDirectory();
				createFile();
				createPlayList();
				createPlayListInfo();
				createSandBoxURL();
				createMountPoints();
				createFileHash();
			}
		}
		catch(const SQLiteException& e)
		{
			printError("Schema",e.error().toLatin1().constData());
			res = false;
		}
	}
	else
	{
		printError("Schema","No database given to define");
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void Schema::createMountPointsOps(SQLiteDatabase *db)
{
	if(!isTableDefinedOps(db, "mountpoints"))
	{
		QString cmd;

		cmd  = "CREATE TABLE mountpoints (";
		cmd += "  mountID INTEGER PRIMARY KEY AUTOINCREMENT,";
		cmd += "  mountName TEXT NOT NULL";
		cmd += ");";
		db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createMountPoints()
{
	createMountPointsOps(m_db);
}

//-------------------------------------------------------------------------------------------

void Schema::createAlbum()
{
	if(!isTableDefined("album"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE album (";
		cmd += "  albumID INTEGER PRIMARY KEY AUTOINCREMENT,";
		cmd += "  albumName TEXT NOT NULL COLLATE NOCASE,";
		cmd += "  year INTEGER NOT NULL,";
		cmd += "  directoryID INTEGER NOT NULL,";
		cmd += "  groupID INTEGER NOT NULL";
		cmd += ");";
		m_db->exec(cmd);
		cmd = "CREATE INDEX albumNameIndex ON album (albumName COLLATE NOCASE);";
		m_db->exec(cmd);
		cmd = "CREATE INDEX albumLookupIndex ON album (albumName COLLATE NOCASE, year, directoryID);";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createTrack()
{
	if(!isTableDefined("track"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE track (";
		cmd += "  albumID INTEGER NOT NULL,";
		cmd += "  trackID INTEGER NOT NULL,";
		cmd += "  fileID INTEGER NOT NULL,";
		cmd += "  trackName TEXT NOT NULL COLLATE NOCASE,";
		cmd += "  artist TEXT NOT NULL COLLATE NOCASE,";
		cmd += "  discNo INTEGER NOT NULL,";
		cmd += "  trackNo INTEGER NOT NULL,";
		cmd += "  originalArtist TEXT NOT NULL COLLATE NOCASE,";
		cmd += "  composer TEXT NOT NULL COLLATE NOCASE,";
		cmd += "  genreID INTEGER NOT NULL,";
		cmd += "  comment TEXT NOT NULL,";
		cmd += "  copyright TEXT NOT NULL,";
		cmd += "  encoder TEXT NOT NULL,";
		cmd += "  timeLength INTEGER NOT NULL,";
		cmd += "  PRIMARY KEY(albumID,trackID)";
		cmd += ");";
		m_db->exec(cmd);
		cmd = "CREATE INDEX trackNameIndex ON track (trackName COLLATE NOCASE);";
		m_db->exec(cmd);
		cmd = "CREATE INDEX artistNameIndex ON track (artist COLLATE NOCASE);";
		m_db->exec(cmd);
		cmd = "CREATE INDEX originalArtistNameIndex ON track (originalArtist COLLATE NOCASE);";
		m_db->exec(cmd);
		cmd = "CREATE INDEX composerNameIndex ON track (composer COLLATE NOCASE);";
		m_db->exec(cmd);
		cmd = "CREATE INDEX fileTrackIndex ON track (albumID,fileID);";
		m_db->exec(cmd);
		cmd = "CREATE INDEX genreTrackIndex ON track (genreID);";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createSubtrack()
{
	if(!isTableDefined("subtrack"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE subtrack (";
		cmd += "  albumID INTEGER NOT NULL,";
		cmd += "  trackID INTEGER NOT NULL,";
		cmd += "  subtrackID INTEGER NOT NULL,";
		cmd += "  subtrackName TEXT NOT NULL COLLATE NOCASE,";
		cmd += "  timeStart INTEGER NOT NULL,";
		cmd += "  timeLength INTEGER NOT NULL,";
		cmd += "  PRIMARY KEY(albumID,trackID,subtrackID)";
		cmd += ");";
		m_db->exec(cmd);
		cmd = "CREATE INDEX subtrackNameIndex ON subtrack (subtrackName COLLATE NOCASE);";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createGenre()
{
	if(!isTableDefined("genre"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE genre (";
		cmd += "  genreID INTEGER NOT NULL,";
		cmd += "  genreName TEXT NOT NULL COLLATE NOCASE,";
		cmd += "  PRIMARY KEY(genreID)";
		cmd += ");";
		m_db->exec(cmd);
		cmd = "CREATE INDEX genreNameIndex ON genre (genreName COLLATE NOCASE);";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createImage()
{
	if(!isTableDefined("image"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE image (";
		cmd += "  imageID INTEGER PRIMARY KEY AUTOINCREMENT,";
		cmd += "  type INTEGER NOT NULL,";
		cmd += "  format INTEGER NOT NULL,";
		cmd += "  sha1SignatureH INTEGER NOT NULL,";
		cmd += "  sha1SignatureL INTEGER NOT NULL,";
		cmd += "  data BLOB";
		cmd += ");";
		m_db->exec(cmd);
		cmd = "CREATE INDEX imageSignatureIndex ON image (sha1SignatureH,sha1SignatureL)";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createImageAlbumMap()
{
	if(!isTableDefined("imagealbummap"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE imagealbummap (";
		cmd += "  albumID INTEGER NOT NULL,";
		cmd += "  imageID INTEGER NOT NULL,";
		cmd += "  fileName TEXT NOT NULL,";
		cmd += "  dirModifiedTime INTEGER NOT NULL,";
		cmd += "  fileModifiedTime INTEGER NOT NULL,";
		cmd += "  PRIMARY KEY(albumID)";
		cmd += ");";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createImageMap()
{
	if(!isTableDefined("imagemap"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE imagemap (";
		cmd += "  albumID INTEGER NOT NULL,";
		cmd += "  trackID INTEGER NOT NULL,";
		cmd += "  imageID INTEGER NOT NULL,";
		cmd += "  PRIMARY KEY(albumID,trackID,imageID)";
		cmd += ");";
		m_db->exec(cmd);
		cmd = "CREATE INDEX imageIDIndex ON imagemap (imageID)";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createDirectory()
{
	if(!isTableDefined("directory"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE directory (";
		cmd += "  directoryID INTEGER PRIMARY KEY AUTOINCREMENT,";
		cmd += "  directoryName TEXT NOT NULL UNIQUE";
		cmd += ");";
		m_db->exec(cmd);
		cmd = "CREATE INDEX directoryNameIndex ON directory (directoryName)";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createFile()
{
	if(!isTableDefined("file"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE file (";
		cmd += "  directoryID INTEGER NOT NULL,";
		cmd += "  fileID INTEGER NOT NULL,";
		cmd += "  codecType INTEGER NOT NULL,";
		cmd += "  infoType INTEGER NOT NULL,";
		cmd += "  fileName TEXT NOT NULL,";
		cmd += "  updateTime INTEGER NOT NULL,";
		cmd += "  fileSize INTEGER NOT NULL,";
		cmd += "  PRIMARY KEY(directoryID,fileID)";
		cmd += ");";
		m_db->exec(cmd);
		cmd = "CREATE INDEX fileNameIndex ON file (fileName)";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createPlayList()
{
	if(!isTableDefined("playlist"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE playlist (";
		cmd += "  playListID INTEGER NOT NULL,";
		cmd += "  position INTEGER NOT NULL,";
		cmd += "  albumID INTEGER NOT NULL,";
		cmd += "  trackID INTEGER NOT NULL,";
		cmd += "  subtrackID INTEGER NOT NULL,";
		cmd += "  PRIMARY KEY(playListID,position)";
		cmd += ");";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createPlayListInfo()
{
	if(!isTableDefined("playlistInfo"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE playlistInfo (";
		cmd += "  playListID INTEGER NOT NULL,";
		cmd += "  name TEXT NOT NULL,";
		cmd += "  PRIMARY KEY(playListID)";
		cmd += ");";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createDatabaseInfo()
{
	if(!isTableDefined("databaseInfo"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE databaseInfo (";
		cmd += "  dbVersion INTEGER NOT NULL";
		cmd += ");";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createSandBoxURL()
{
	if(!isTableDefined("sandBoxURL"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE sandBoxURL (";
		cmd += "  url TEXT NOT NULL,";
		cmd += "  docUrl TEXT NOT NULL,";
		cmd += "  access INTEGER NOT NULL,";
		cmd += "  accessTime INTEGER NOT NULL,";
		cmd += "  bookmark BLOB,";
		cmd += "  PRIMARY KEY(url,docUrl)";
		cmd += ");";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

void Schema::createFileHash()
{
	if(!isTableDefined("fileHash"))
	{
		QString cmd;
		
		cmd  = "CREATE TABLE fileHash (";
		cmd += "	directoryID INTEGER NOT NULL,";
		cmd += "	fileID INTEGER NOT NULL,";
		cmd += "	hashID INTEGER NOT NULL,";
		cmd += "	PRIMARY KEY(directoryID, hashID)";
		cmd += ");";
		m_db->exec(cmd);
		cmd = "CREATE INDEX fileHashIDIndex ON fileHash (hashID);";
		m_db->exec(cmd);
	}
}

//-------------------------------------------------------------------------------------------

bool Schema::writeDatabaseVersion(int versionNo)
{
	QString cmd;
	bool res = false;
	
	try
	{
		int cVersionNo;
		SQLiteInsert projectI(m_db);
		SQLiteQuery projectQ(m_db);
		
		m_db->exec("SAVEPOINT writeDatabaseVersion");
		
		cmd = "SELECT dbVersion FROM databaseInfo";
		projectQ.prepare(cmd);
		projectQ.bind(cVersionNo);
		if(projectQ.next())
		{
			if(cVersionNo==versionNo)
			{
				res = true;
			}
			else
			{
				if(cVersionNo==1 && versionNo>=2)
				{
					res = upgradeVersion1To2();
					cVersionNo = 2;
				}
				if(cVersionNo>=2 && versionNo>=2)
				{
					res = true;
				}
				else
				{
					QString errMsg;
					errMsg = "Current project database is version " + QString::number(cVersionNo) + " while version " + QString::number(versionNo) + " was expected.";
					printError("writeProjectVersion",errMsg.toLatin1().constData());
				}
			}
		}
		else
		{
			cmd = "INSERT INTO databaseInfo VALUES (?)";
			projectI.prepare(cmd);
			projectI.bind(versionNo);
			projectI.next();
			res = true;
		}
		
		if(res)
		{
			m_db->exec("RELEASE writeDatabaseVersion");
		}
		else
		{
			m_db->exec("ROLLBACK TO SAVEPOINT writeDatabaseVersion");	
		}
	}
	catch(const SQLiteException& e)
	{
		m_db->exec("ROLLBACK TO SAVEPOINT writeDatabaseVersion");
		printError("writeDatabaseVersion",e.error().toLatin1().constData());
		res = false;		
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool Schema::upgradeVersion1To2()
{
	bool res = false;
	
	try
	{
		tint albumID;
		QString cmd,cmdQ,albumName;
		QMap<QString,QVector<tint> > albumNameMap;
		QMap<QString,QVector<tint> >::iterator ppI;
		SQLiteQuery albumQ(m_db);
		
		m_db->exec("SAVEPOINT upgradeVersion1To2");
		
		cmd = "ALTER TABLE album ADD COLUMN groupID INTEGER NOT NULL DEFAULT -1";
		m_db->exec(cmd);
		cmd = "UPDATE databaseInfo SET dbVersion=2";
		m_db->exec(cmd);
		
		cmdQ = "SELECT albumID, albumName FROM album";
		albumQ.prepare(cmdQ);
		albumQ.bind(albumID);
		albumQ.bind(albumName);
		while(albumQ.next())
		{
			albumName = albumName.trimmed();
			ppI = albumNameMap.find(albumName);
			if(ppI!=albumNameMap.end())
			{
				QVector<tint>& cSet = ppI.value();
				cSet.append(albumID);
			}
			else
			{
				QVector<tint> nSet;
				nSet.append(albumID);
				albumNameMap.insert(albumName,nSet);
			}
		}
		
		for(ppI=albumNameMap.begin();ppI!=albumNameMap.end();ppI++)
		{
			if(ppI.value().size()>=2)
			{
				TrackDB::instance()->groupAlbums(ppI.value());
			}
		}
		
		res = true;
		
		if(res)
		{
			m_db->exec("RELEASE upgradeVersion1To2");
		}
		else
		{
			m_db->exec("ROLLBACK TO SAVEPOINT upgradeVersion1To2");			
		}
	}
	catch(const SQLiteException& e)
	{
		m_db->exec("ROLLBACK TO SAVEPOINT upgradeVersion1To2");
		printError("upgradeVersion1To2",e.error().toLatin1().constData());
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

QString Schema::tempUpgradeDBFileName(const QString& orgTrackDBFileName)
{
	int i;
	QString name;
	
	for(i=orgTrackDBFileName.length()-1;i>=0 && name.isEmpty();i--)
	{
		if(orgTrackDBFileName.at(i)==QChar('.'))
		{
			name = orgTrackDBFileName.mid(0,i) + "_upgrade.db";
		}
	}
	return name;
}

//-------------------------------------------------------------------------------------------

bool Schema::upgradeVersion4To5CopySandbox(SQLiteDatabase *orgDB,SQLiteDatabase *newDB)
{
	bool res = true;

	try
	{
		QString cmdQ;
		SQLiteQuery sandboxQ(orgDB);
		
		newDB->exec("SAVEPOINT upgradeVersion4To5CopySandbox");

		QString url,docUrl;
		tint access, accessTime;
		QByteArray bookmark;

		cmdQ = "SELECT url, docUrl, access, accessTime, bookmark FROM sandBoxURL";
		sandboxQ.prepare(cmdQ);
		sandboxQ.bind(url);
		sandboxQ.bind(docUrl);
		sandboxQ.bind(access);
		sandboxQ.bind(accessTime);
		sandboxQ.bind(bookmark);
		
		while(sandboxQ.next() && res)
		{
			QString cmdI;
			SQLiteInsert sandboxI(newDB);
			
			cmdI = "INSERT INTO sandBoxURL (url, docUrl, access, accessTime, bookmark) VALUES (?,?,?,?,?)";
			sandboxI.prepare(cmdI);
			sandboxI.bind(url);
			sandboxI.bind(docUrl);
			sandboxI.bind(access);
			sandboxI.bind(accessTime);
			sandboxI.bind(bookmark);
			if(!sandboxI.next())
			{
				res = false;
			}
		}
		
		if(res)
		{
			newDB->exec("RELEASE upgradeVersion4To5CopySandbox");
		}
		else
		{
			newDB->exec("ROLLBACK TO SAVEPOINT upgradeVersion4To5CopySandbox");
		}
	}
	catch(const SQLiteException&)
	{
		newDB->exec("ROLLBACK TO SAVEPOINT upgradeVersion4To5CopySandbox");
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool Schema::upgradeVersion4To5(const QString& orgTrackDBFileName)
{
	bool res = false;

	QString newTrackDBFileName = tempUpgradeDBFileName(orgTrackDBFileName);
	if(!newTrackDBFileName.isEmpty())
	{
		SQLiteDatabase orgTrackDB;
		
		if(orgTrackDB.open(orgTrackDBFileName))
		{
			SQLiteDatabase newTrackDB;
		
			if(newTrackDB.open(newTrackDBFileName))
			{
				Schema dbSchema;
				
				if(dbSchema.createDB(&newTrackDB))
				{
					res = upgradeVersion4To5CopySandbox(&orgTrackDB,&newTrackDB);
				}
				newTrackDB.close();
			}
			orgTrackDB.close();
		}
		
		if(res)
		{
			common::DiskOps::remove(orgTrackDBFileName);
			QFile nDBFile(newTrackDBFileName);
			if(!nDBFile.rename(orgTrackDBFileName))
			{
				res = false;
			}
		}
		else
		{
			common::DiskOps::remove(newTrackDBFileName);
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool Schema::upgradeVerion5To6Operation(SQLiteDatabase *orgDB,SQLiteDatabase *newDB)
{
	bool res = true;

	try
	{
		newDB->exec("SAVEPOINT upgradeVersion5To6");
		createMountPointsOps(newDB);
		newDB->exec("RELEASE upgradeVersion5To6");
	}
	catch(const SQLiteException&)
	{
		newDB->exec("ROLLBACK TO SAVEPOINT upgradeVersion5To6");
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool Schema::upgradeVersion5To6(const QString& orgTrackDBFileName)
{
	bool res = false;

	QString newTrackDBFileName = tempUpgradeDBFileName(orgTrackDBFileName);
	if(!newTrackDBFileName.isEmpty())
	{
		SQLiteDatabase orgTrackDB;
		
		if(orgTrackDB.open(orgTrackDBFileName))
		{
			SQLiteDatabase newTrackDB;
		
			if(newTrackDB.open(newTrackDBFileName))
			{
				Schema dbSchema;
				
				if(dbSchema.createDB(&newTrackDB))
				{
					res = upgradeVersion4To5CopySandbox(&orgTrackDB,&newTrackDB);
				}
				newTrackDB.close();
			}
			orgTrackDB.close();
		}
		
		if(res)
		{
			common::DiskOps::remove(orgTrackDBFileName);
			QFile nDBFile(newTrackDBFileName);
			if(!nDBFile.rename(orgTrackDBFileName))
			{
				res = false;
			}
		}
		else
		{
			common::DiskOps::remove(newTrackDBFileName);
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
