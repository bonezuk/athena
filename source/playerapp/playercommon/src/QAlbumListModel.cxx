#include "playerapp/playercommon/inc/QAlbumListModel.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

QAlbumListModel::QAlbumListModel(QObject *parent) : QAbstractListModel(parent),
	m_pAlbums()
{
	m_pTrackModel = new QAlbumTrackListModel(this);
}

//-------------------------------------------------------------------------------------------

QAlbumListModel::~QAlbumListModel()
{
	delete m_pTrackModel;
	m_pTrackModel = 0;
}

//-------------------------------------------------------------------------------------------

int QAlbumListModel::rowCount(const QModelIndex& parent) const
{
	if(parent.isValid() || m_pAlbums.isNull())
	{
		return 0;
	}
	return m_pAlbums->size();
}

//-------------------------------------------------------------------------------------------

QVariant QAlbumListModel::data(const QModelIndex& index, int role) const
{
	QVariant v;
	
	if(!m_pAlbums.isNull())
	{
		if(index.isValid() && index.row() >= 0 && index.row() < m_pAlbums->size())
		{
			if(role == Qt::DisplayRole || role == AlbumRole)
			{
				v = m_pAlbums->data(index.row(), 1);
			}
			else if(role == ArtistRole)
			{
				v = m_pAlbums->data(index.row(), 2);
			}
			else if(role == YearRole)
			{
				v = m_pAlbums->data(index.row(), 3);
			}
		}
	}
	return v;
}

//-------------------------------------------------------------------------------------------

QHash<int,QByteArray> QAlbumListModel::roleNames() const
{
	QHash<int,QByteArray> h;
	h[ArtistRole] = "artist";
	h[AlbumRole] = "album";
	h[YearRole] = "year";
	return h;
}

//-------------------------------------------------------------------------------------------

bool QAlbumListModel::initialise()
{
	QSharedPointer<track::model::AlbumModel> pAlbums(new track::model::AlbumModel());
	bool res = false;
	
	if(pAlbums->build())
	{
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

QAlbumTrackListModel *QAlbumListModel::trackModel()
{
	return m_pTrackModel;
}

//-------------------------------------------------------------------------------------------

void QAlbumListModel::showAlbumTracks(int index)
{
	if(!m_pAlbums.isNull() && index >= 0 && index < m_pAlbums->size())
	{
		track::model::TrackModelKey key;
		QVariant aKey = m_pAlbums->data(index, 0);
		track::model::AlbumModelKey albumKey(aKey);
		key.album() = albumKey;
		m_pTrackModel->load(key);
	}
}

//-------------------------------------------------------------------------------------------

void QAlbumListModel::appendTrack(const QString& fileName)
{
	if(!m_pAlbums.isNull() && common::DiskOps::exist(fileName))
	{
		QSharedPointer<track::db::DBInfo> pInfo = track::db::DBInfo::readInfo(fileName).dynamicCast<track::db::DBInfo>();
		if(!pInfo.isNull())
		{
			QVector<tint> indexes = m_pAlbums->indexForDBInfo(pInfo, true);
			for(int i = indexes.size()-1; i >= 0; i--)
			{
				int idx = indexes.at(i);
				beginInsertRows(QModelIndex(), idx, idx);
				m_pAlbums->addDBInfo(indexes.at(i), pInfo);
				endInsertRows();
			}
		}
	}
}

//-------------------------------------------------------------------------------------------

void QAlbumListModel::deleteTrack(const QString& fileName)
{
	if(!m_pAlbums.isNull() && common::DiskOps::exist(fileName))
	{
		QSharedPointer<track::db::DBInfo> pInfo = track::db::DBInfo::readInfo(fileName).dynamicCast<track::db::DBInfo>();
		if(!pInfo.isNull())
		{
			QVector<tint> indexes = m_pAlbums->indexForDBInfo(pInfo, false);
			for(int i = indexes.size()-1; i >= 0; i--)
			{
				int idx = indexes.at(i);
				beginRemoveRows(QModelIndex(), idx, idx);
				m_pAlbums->removeRow(idx);
				endRemoveRows();
			}
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
