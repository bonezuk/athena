#include "playerapp/playerios/inc/PlayerIOSBaseModel.h"
#include "common/inc/DiskOps.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

PlayerIOSBaseModel::PlayerIOSBaseModel(QObject *parent) : QAbstractListModel(parent),
	m_playlistID(0),
	m_currentInfo()
{}

//-------------------------------------------------------------------------------------------

PlayerIOSBaseModel::~PlayerIOSBaseModel()
{}

//-------------------------------------------------------------------------------------------

void PlayerIOSBaseModel::printError(const tchar *strR, const tchar *strE) const
{
	common::Log::g_Log << "PlayerIOSBaseModel::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

int PlayerIOSBaseModel::sizeOfPlaylist() const
{
	int rCount;
	QString cmd;
	track::db::SQLiteDatabase *db = track::db::TrackDB::instance()->db();
	track::db::SQLiteQuery countQ(db);
		
	cmd = QString("SELECT COUNT(position) FROM playlist WHERE playListID=%1").arg(m_playlistID);
	countQ.prepare(cmd);
	countQ.bind(rCount);
	if(!countQ.next())
	{
		rCount = 0;
	}
	return rCount;
}

//-------------------------------------------------------------------------------------------

int PlayerIOSBaseModel::rowCount(const QModelIndex& parent) const
{
	int rCount;
	
	if(!parent.isValid())
	{
		rCount = sizeOfPlaylist();
	}
	else
	{
		rCount = 0;
	}
	return rCount;
}

//-------------------------------------------------------------------------------------------

bool PlayerIOSBaseModel::getID(const QModelIndex& index, int& albumID, int& trackID, int& subtrackID) const
{
	bool res = false;
	
	if(index.isValid())
	{
		QString cmd;
		track::db::SQLiteDatabase *db = track::db::TrackDB::instance()->db();
		track::db::SQLiteQuery trackQ(db);
		
		cmd = QString("SELECT albumID, trackID, subtrackID FROM playlist WHERE playListID=%1 AND position=%2")
			.arg(m_playlistID).arg(index.row());
		trackQ.prepare(cmd);
		trackQ.bind(albumID);
		trackQ.bind(trackID);
		trackQ.bind(subtrackID);
		if(trackQ.next())
		{
			res = true;
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<track::db::DBInfo> PlayerIOSBaseModel::getTrack(const QModelIndex& index, int& subtrackID) const
{
	QSharedPointer<track::db::DBInfo> pInfo;
	
	if(index.isValid())
	{
		int albumID, trackID;
		
		if(getID(index, albumID, trackID, subtrackID))
		{
			if(!m_currentInfo.isNull() && m_currentInfo->albumID() == albumID || m_currentInfo->trackID() == trackID)
			{
				pInfo = m_currentInfo;
			}
			else
			{
				QSharedPointer<track::info::Info> info = track::db::DBInfo::readInfo(trackID, albumID);
				m_currentInfo = info.dynamicCast<track::db::DBInfo>();
				pInfo = m_currentInfo;
			}
		}
	}
	return pInfo;
}

//-------------------------------------------------------------------------------------------

QVariant PlayerIOSBaseModel::data(const QModelIndex& index, int role) const
{
	int subtrackID;
	
	if(index.isValid())
	{
		QSharedPointer<track::db::DBInfo> pInfo = getTrack(index, subtrackID);
		if(!pInfo.isNull())
		{
			QString trackName;
			
			if(subtrackID >= 0 && subtrackID < pInfo->noChildren())
			{
				trackName = pInfo->child(subtrackID).name();
			}
			else
			{
				trackName = pInfo->title();
			}
			if(role == Qt::DisplayRole)
			{
				QString artist = pInfo->artist();
				QString s = trackName + " (" + artist + ")";
				return QVariant(s);
			}
			else if(role == ArtistRole)
			{
				QString s = pInfo->artist();
				return QVariant(s);
			}
			else if(role == TitleRole) 
			{
				return QVariant(trackName);
			}
			else if(role == AlbumRole)
			{
				QString s = pInfo->album();
				return QVariant(s);
			}		
		}
	}
	return QVariant();
}

//-------------------------------------------------------------------------------------------

QHash<int,QByteArray> PlayerIOSBaseModel::roleNames() const
{
	QHash<int,QByteArray> h;
	h[ArtistRole] = "artist";
	h[TitleRole] = "title";
	h[AlbumRole] = "album";
	return h;
}

//-------------------------------------------------------------------------------------------

void PlayerIOSBaseModel::appendTrack(const QString& fileName)
{
	if(common::DiskOps::exist(fileName))
	{
		QSharedPointer<track::db::DBInfo> pInfo = track::db::DBInfo::readInfo(fileName).dynamicCast<track::db::DBInfo>();
		if(!pInfo.isNull())
		{
			track::db::SQLiteDatabase *db = track::db::TrackDB::instance()->db();
			int nextPosition = sizeOfPlaylist();

			beginInsertRows(QModelIndex(), nextPosition, nextPosition + pInfo->noChildren());
			try
			{
				QString cmdI;
				track::db::SQLiteInsert playI(db);
				int albumID = pInfo->albumID();
				int trackID = pInfo->trackID();
				int subtrackID;
				bool res = true;
				
				db->exec("SAVEPOINT savePlaylistPLM");
				
				cmdI = "INSERT INTO playlist VALUES (?,?,?,?,?)";
				playI.prepare(cmdI);
				if(pInfo->noChildren() > 0)
				{
					for(int i = 0; i < pInfo->noChildren() && res; i++)
					{
						playI.bind(m_playlistID);
						playI.bind(nextPosition);
						playI.bind(albumID);
						playI.bind(trackID);
						playI.bind(i);
						if(!playI.next())
						{
							printError("appendTrack", "Error insert into playlist table");
							res = false;
						}
						nextPosition++;
					}
				}
				else
				{
					subtrackID = -1;
					playI.bind(m_playlistID);
					playI.bind(nextPosition);
					playI.bind(albumID);
					playI.bind(trackID);
					playI.bind(subtrackID);
					if(!playI.next())
					{
						printError("appendTrack", "Error insert into playlist table");
						res = false;
					}				
				}
				
				if(res)
				{
					db->exec("RELEASE savePlaylistPLM");
				}
				else
				{
					db->exec("ROLLBACK TO SAVEPOINT savePlaylistPLM");
				}
			}
			catch(const track::db::SQLiteException& e)
			{
				printError("appendTrack", e.error().toUtf8().constData());
				db->exec("ROLLBACK TO SAVEPOINT savePlaylistPLM");
			}
			
			endInsertRows();
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
