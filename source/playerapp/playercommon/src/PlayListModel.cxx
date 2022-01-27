#include "playerapp/playercommon/inc/PlayListModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlayListModel::PlayListModel() : QAbstractListModel(0),
	m_playList(),
	m_pAudioInterface(),
	m_pPlaybackState(0)
{}

//-------------------------------------------------------------------------------------------

PlayListModel::PlayListModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, OmegaAudioInterface *pAudioInterface, QObject *parent) : QAbstractListModel(parent),
	m_playList(playList),
	m_pAudioInterface(pAudioInterface),
	m_pPlaybackState(0)
{
	m_pPlaybackState = new PlaybackState(this);
}

//-------------------------------------------------------------------------------------------

PlayListModel::~PlayListModel()
{
	if(m_pPlaybackState != 0)
	{
		delete m_pPlaybackState;
		m_pPlaybackState = 0;
	}
}

//-------------------------------------------------------------------------------------------

void PlayListModel::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "PlayListModel::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

PlaybackState *PlayListModel::playbackState()
{
	return m_pPlaybackState;
}

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
			QString s = pInfo->title();
			return QVariant(s);
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

void PlayListModel::playItemAtIndex(int index)
{
	if(index >= 0 && index < m_playList.size())
	{
		QString fileName = m_playList.at(index).first->getFilename();
		common::Log::g_Log.print("PlayListModel::playItemAtIndex - %d '%s'\n", index, fileName.toUtf8().constData());
		m_pAudioInterface->playFile(fileName);
		m_pPlaybackState->setPlaybackItem(index, m_playList.at(index));
	}
	else
	{
		QString err = QString("Given index, %1, out of range").arg(index);
		printError("playItemAtIndex", err.toUtf8().constData());
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
