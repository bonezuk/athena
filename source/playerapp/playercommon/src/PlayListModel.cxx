#include "playerapp/playercommon/inc/PlayListModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlayListModel::PlayListModel(QObject *parent) : QAbstractListModel(parent),
	m_playList(),
	m_pAudioInterface(),
	m_pPlaybackState(0)
{}

//-------------------------------------------------------------------------------------------

PlayListModel::PlayListModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent) : QAbstractListModel(parent),
	m_playList(playList),
	m_pAudioInterface(pAudioInterface),
	m_pPlaybackState(0)
{
	m_pPlaybackState = QSharedPointer<PlaybackStateController>(new PlaybackStateController(pAudioInterface, this));
}

//-------------------------------------------------------------------------------------------

PlayListModel::~PlayListModel()
{
	m_pAudioInterface.clear();
	m_pPlaybackState.clear();
}

//-------------------------------------------------------------------------------------------

void PlayListModel::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "PlayListModel::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<PlaybackStateController>& PlayListModel::playbackState()
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

void PlayListModel::playItemAtIndexWithNext(int index, bool isNext)
{
	if(index >= 0 && index < m_playList.size())
	{
		QString fileName = m_playList.at(index).first->getFilename();
		common::Log::g_Log.print("PlayListModel::playItemAtIndex - %d '%s'\n", index, fileName.toUtf8().constData());
		m_pAudioInterface->playFile(fileName, isNext);
		m_pPlaybackState->setNextItem(index, m_playList.at(index));
	}
	else
	{
		QString err = QString("Given index, %1, out of range").arg(index);
		printError("playItemAtIndex", err.toUtf8().constData());
	}
}

//-------------------------------------------------------------------------------------------

void PlayListModel::playItemAtIndex(int index)
{
	playItemAtIndexWithNext(index, false);
}

//-------------------------------------------------------------------------------------------

void PlayListModel::onPlayPausePressed()
{
	int playIndex;
	
	playIndex = m_pPlaybackState->getIndex();
	if (playIndex >= 0 && playIndex < m_playList.size())
	{
		m_pPlaybackState->resumeOrPausePlayback();
	}
	else
	{
		playItemAtIndex(0);
	}
}

//-------------------------------------------------------------------------------------------

void PlayListModel::playNextItem(bool isNext)
{
	int nextIndex;

	nextIndex = m_pPlaybackState->getIndex();
	if(nextIndex < 0)
	{
		nextIndex = 0;
	}
	else
	{
		nextIndex++;
	}
	if(nextIndex >= 0 && nextIndex < m_playList.size())
	{
		playItemAtIndexWithNext(nextIndex, isNext);
	}
	else
	{
		if(isNext)
		{
			m_pPlaybackState->setNextItem(-1, QPair<track::db::DBInfoSPtr, tint>());
		}
		else
		{
			m_pPlaybackState->onAudioStop();
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
