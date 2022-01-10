#include "embedded/playlistmanager/inc/PlayListModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlayListModel::PlayListModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QObject *parent) : QAbstractListModel(parent),
	m_playList(playList)
{}

//-------------------------------------------------------------------------------------------

PlayListModel::~PlayListModel()
{}

//-------------------------------------------------------------------------------------------

QVariant PlayListModel::data(const QModelIndex& index, int role) const
{
	if(index.isValid() && index.row() >= 0 && index.row() < m_playList.size())
	{
		track::db::DBInfoSPtr pInfo = m_playList.at(index.row()).first;
		
		if(role == Qt::DisplayRole)
		{
			QString s = pInfo->title() + " (" + pInfo->artist() + ")";
			return QVariant(s);
		}
		else if(role == ArtistRole)
		{
			return QVariant(pInfo->artist());
		}
		else if(role == TitleRole)
		{
			return QVariant(pInfo->title());
		}
		else if(role == AlbumRole)
		{
			return QVariant(pInfo->album());
		}
		else if(role == YearRole)
		{
			return QVariant(pInfo->year());
		}
		else if(role == CommentRole)
		{
			return QVariant(pInfo->comment());
		}
		else if(role == GenreRole)
		{
			return QVariant(pInfo->originalArtist());
		}
		else if(role == TrackRole)
		{
			return QVariant(pInfo->track());
		}
		else if(role == DiscRole)
		{
			return QVariant(pInfo->disc());
		}
		else if(role == ComposerRole)
		{
			return QVariant(pInfo->composer());
		}
		else if(role == OriginalArtistRole)
		{
			return QVariant(pInfo->originalArtist());
		}
		else if(role == CopyrightRole)
		{
			return QVariant(pInfo->copyright());
		}
		else if(role == EncoderRole)
		{
			return QVariant(pInfo->encoder());
		}
	}
	return QVariant();
}

//-------------------------------------------------------------------------------------------

int PlayListModel::rowCount(const QModelIndex& parent) const
{
	if(parent.isValid())
	{
		return 0;
	}
	return m_playList.size();
}

//-------------------------------------------------------------------------------------------

QHash<int,QByteArray> PlayListModel::roleNames() const
{
	QHash<int,QByteArray> h;
	h[ArtistRole] = "artist";
	h[TitleRole] = "title";
	h[AlbumRole] = "album";
	h[YearRole] = "year";
	h[CommentRole] = "comment";
	h[GenreRole] = "genre";
	h[TrackRole] = "track";
	h[DiscRole] = "disc";
	h[ComposerRole] = "composer";
	h[OriginalArtistRole] = "originalArtist";
	h[CopyrightRole] = "copyright";
	h[EncoderRole] = "encoder";
	return h;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
