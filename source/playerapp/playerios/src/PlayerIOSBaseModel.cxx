#include "playerapp/playerios/inc/PlayerIOSBaseModel.h"
#include "common/inc/DiskOps.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

PlayerIOSBaseModel::PlayerIOSBaseModel(QObject *parent) : QAbstractListModel(parent),
	m_playlistID(0),
	m_playlistName("Default"),
	m_playlist(),
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

bool PlayerIOSBaseModel::loadPlaylist()
{
	m_playlist.clear();
	return track::db::TrackDB::instance()->loadPlaylist(m_playlistID, m_playlist);
}

//-------------------------------------------------------------------------------------------

int PlayerIOSBaseModel::sizeOfPlaylist() const
{
	return m_playlist.size();
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
	
	if(index.isValid() && index.row() >= 0 && index.row() < sizeOfPlaylist())
	{
		track::db::PlaylistTuple item = m_playlist.at(index.row());
		albumID = item.albumID;
		trackID = item.trackID;
		subtrackID = item.subtrackID;
		res = true;
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
			if(!m_currentInfo.isNull() && m_currentInfo->albumID() == albumID && m_currentInfo->trackID() == trackID)
			{
				pInfo = m_currentInfo;
			}
			else
			{
				QSharedPointer<track::info::Info> info = track::db::DBInfo::readInfo(albumID, trackID);
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
				m_playlist.append(t);
			}
			else
			{
				beginInsertRows(QModelIndex(), nextPosition, nextPosition + (pInfo->noChildren() - 1));
				for(int i = 0; i < pInfo->noChildren(); i++)
				{
					t.subtrackID = i;
					m_playlist.append(t);
				}
				
			}
			endInsertRows();

			track::db::TrackDB::instance()->replacePlaylist(m_playlistID, m_playlistName, m_playlist);
		}
	}
}

//-------------------------------------------------------------------------------------------

void PlayerIOSBaseModel::deleteTrack(const QString& fileName)
{
	tint i, albumID, trackID;
	
	if(track::db::TrackDB::instance()->getKeysFromFilename(fileName, albumID, trackID))
	{
		for(i = 0; i < m_playlist.size(); i++)
		{
			track::db::PlaylistTuple t = m_playlist.at(i);
			if(t.albumID == albumID && t.trackID == trackID)
			{
				beginRemoveRows(QModelIndex(), i, i);
				m_playlist.removeAt(i);
				endRemoveRows();
				i--;
			}
		}
		track::db::TrackDB::instance()->replacePlaylist(m_playlistID, m_playlistName, m_playlist);
	}
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
