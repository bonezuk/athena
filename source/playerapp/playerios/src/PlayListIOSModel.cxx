#include "playerapp/playerios/inc/PlayListIOSModel.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

PlayListIOSModel::PlayListIOSModel(QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent) : PlayListModel(pAudioInterface, parent),
	m_playlistID(0),
	m_playlistName("Default")
{}

//-------------------------------------------------------------------------------------------

PlayListIOSModel::~PlayListIOSModel()
{}

//-------------------------------------------------------------------------------------------

void PlayListIOSModel::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "PlayListIOSModel::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void PlayListIOSModel::appendPlaylistTuple(const track::db::PlaylistTuple& t)
{
	track::db::TrackDB *pDB = track::db::TrackDB::instance();
	
	if(pDB != 0)
	{
		QSharedPointer<track::info::Info> pInfoItem = track::db::DBInfo::readInfo(t.albumID, t.trackID);
		track::db::DBInfoSPtr pDBItem = pInfoItem.dynamicCast<track::db::DBInfo>();
		if(!pDBItem.isNull())
		{
			m_items.insert(t.itemID, QPair<track::db::DBInfoSPtr,tint>(pDBItem, t.subtrackID));
			m_idToIndexMap.insert(t.itemID, m_playList.size());
			m_playList.append(t.itemID);
		}
		else
		{
			QString err = QString("Failed to load albumID=%1, trackID=%2").arg(t.albumID, t.trackID);
			printError("loadPlaylistFromDB", err.toUtf8().constData());
		}	
	}
	else
	{
		printError("appendPlaylistTuple", "Track database has not been instantiated");
	}
}

//-------------------------------------------------------------------------------------------

bool PlayListIOSModel::loadPlaylistFromDB()
{
	bool res = false;
	track::db::TrackDB *pDB = track::db::TrackDB::instance();
	
	m_items.clear();
	m_playList.clear();
	m_idToIndexMap.clear();
	
	if(pDB != 0)
	{
		QVector<track::db::PlaylistTuple> idPList;
		
		if(pDB->loadPlaylist(m_playlistID, idPList))
		{
			QVector<QPair<track::db::DBInfoSPtr,tint> > pList;
			
			for(QVector<track::db::PlaylistTuple>::iterator ppI = idPList.begin(); ppI != idPList.end(); ppI++)
			{
				track::db::PlaylistTuple t = *ppI;
				appendPlaylistTuple(t);
			}
			res = true;
		}
		else
		{
			printError("loadPlaylistFromDB", "Failed to load playlist from database");
		}
	}
	else
	{
		printError("loadPlaylistFromDB", "Track database has not been instantiated");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void PlayListIOSModel::savePlaylistToDB()
{
	track::db::TrackDB *pDB = track::db::TrackDB::instance();
	
	if(pDB != 0)
	{
		QVector<track::db::PlaylistTuple> list;

		for(QVector<tuint64>::iterator ppI = m_playList.begin(); ppI != m_playList.end(); ppI++)
		{
			QMap<tuint64, QPair<track::db::DBInfoSPtr,tint> >::iterator ppJ;
			track::db::PlaylistTuple t;
		
			t.itemID = *ppI;
			ppJ = m_items.find(t.itemID);
			if(ppJ != m_items.end())
			{
				track::db::DBInfoSPtr pDBInfo = ppJ.value().first;
				if(!pDBInfo.isNull())
				{
					t.albumID = pDBInfo->albumID();
					t.trackID = pDBInfo->trackID();
					t.subtrackID = ppJ.value().second;
					list.append(t);
				}
				else
				{
					printError("savePlaylistToDB", "Expected DB item is null pointer");
				}
			}
			else
			{
				QString err = QString("Expected item ID %1 not found in items map").arg(t.itemID);
				printError("savePlaylistToDB", err.toUtf8().constData());
			}
		}
		if(!pDB->replacePlaylist(m_playlistID, m_playlistName, list))
		{
			printError("savePlaylistToDB", "Error saving playlist to database");
		}
	}
	else
	{
		printError("savePlaylistToDB", "Track database has not been instantiated");
	}
}

//-------------------------------------------------------------------------------------------

bool PlayListIOSModel::initialise()
{	
	bool res = false;
	
	if(PlayListModel::initialise())
	{
		if(loadPlaylistFromDB())
		{
			QSharedPointer<PlaybackStateController> pPlaybackState(new PlaybackStateController(m_pAudioInterface, this));
			m_pPlaybackState = pPlaybackState;
			res = true;
		}
		else
		{
			printError("initialise", "Failed to load playlist from database");
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void PlayListIOSModel::appendTrack(const QString& fileName)
{
	if(common::DiskOps::exist(fileName))
	{
		QSharedPointer<track::db::DBInfo> pInfo = track::db::DBInfo::readInfo(fileName).dynamicCast<track::db::DBInfo>();
		if(!pInfo.isNull())
		{
			track::db::PlaylistTuple t;
			int nextPosition = sizeOfPlaylist();
			int albumID = pInfo->albumID();
			int trackID = pInfo->trackID();
			
			t.albumID = albumID;
			t.trackID = trackID;
			if(!pInfo->noChildren())
			{
				beginInsertRows(QModelIndex(), nextPosition, nextPosition);
				t.subtrackID = -1;
				t.itemID = generateNewId();
				appendPlaylistTuple(t);
			}
			else
			{
				beginInsertRows(QModelIndex(), nextPosition, nextPosition + (pInfo->noChildren() - 1));
				for(int i = 0; i < pInfo->noChildren(); i++)
				{
					t.subtrackID = i;
					t.itemID = generateNewId();
					appendPlaylistTuple(t);
				}
			}
			endInsertRows();
			
			savePlaylistToDB();
		}
	}
}

//-------------------------------------------------------------------------------------------

void PlayListIOSModel::deleteTrack(const QString& fileName)
{
	tint albumID, trackID;
	
	if(track::db::TrackDB::instance()->getKeysFromFilename(fileName, albumID, trackID))
	{
		for(tint i = 0; i < m_playList.size(); i++)
		{
			tuint64 itemID = m_playList.at(i);
			QMap<tuint64, QPair<track::db::DBInfoSPtr,tint> >::iterator ppJ;
			
			ppJ = m_items.find(itemID);
			if(ppJ != m_items.end())
			{
				track::db::DBInfoSPtr pDBItem = ppJ.value().first;
				if(albumID == pDBItem->albumID() && trackID == pDBItem->trackID())
				{
					beginRemoveRows(QModelIndex(), i, i);
					
					QMap<tuint64, tint>::iterator ppK = m_idToIndexMap.find(itemID);
					if(ppK != m_idToIndexMap.end())
					{
						m_idToIndexMap.erase(ppK);
					}
					m_items.erase(ppJ);
					m_playList.removeAt(i);
					
					endRemoveRows();
					i--;					
				}
			}
		}
		savePlaylistToDB();
	}
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
