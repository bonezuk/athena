#include "playerapp/playercommon/inc/PlayListModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlayListModel::PlayListModel(QObject *parent) : QAbstractListModel(parent),
	m_items(),
	m_playList(),
	m_idToIndexMap(),
	m_pAudioInterface(),
	m_pPlaybackState(0)
{}

//-------------------------------------------------------------------------------------------

PlayListModel::PlayListModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent) : QAbstractListModel(parent),
	m_items(),
	m_playList(),
	m_idToIndexMap(),
	m_pAudioInterface(pAudioInterface),
	m_pPlaybackState(0)
{
	PlayListModel::appendToPlaylist(playList);
}

//-------------------------------------------------------------------------------------------

PlayListModel::~PlayListModel()
{}

//-------------------------------------------------------------------------------------------

void PlayListModel::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "PlayListModel::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void PlayListModel::initialise()
{
	m_pPlaybackState = QSharedPointer<PlaybackStateController>(new PlaybackStateController(m_pAudioInterface, this));
}

//-------------------------------------------------------------------------------------------

QSharedPointer<PlaybackStateController>& PlayListModel::playbackState()
{
	return m_pPlaybackState;
}

//-------------------------------------------------------------------------------------------

tuint64 PlayListModel::generateNewId() const
{
	tuint64 r;
	QMap<tuint64, tint>::const_iterator ppI;
	
	do
	{
		r = common::Random::instance()->randomUInt64();
		ppI = m_idToIndexMap.find(r);
		if(ppI != m_idToIndexMap.constEnd())
		{
			r = 0;
		}
	} while(!r);
	return r;
}

//-------------------------------------------------------------------------------------------

void PlayListModel::appendToPlaylist(const QVector<QPair<track::db::DBInfoSPtr,tint> >& list)
{
	QVector<QPair<track::db::DBInfoSPtr,tint> >::const_iterator ppI;
	common::Random *rand = common::Random::instance();
	
	for(ppI = list.constBegin(); ppI != list.constEnd(); ppI++)
	{
		const QPair<track::db::DBInfoSPtr,tint>& item = *ppI;
		tuint64 id = generateNewId();
		m_items.insert(id, *ppI);
		m_idToIndexMap.insert(id, m_playList.size());
		m_playList.append(id);
	}
}

//-------------------------------------------------------------------------------------------

QString PlayListModel::titleOfItem(const QPair<track::db::DBInfoSPtr,tint>& item) const
{
	QString title;

	if(item.second >= 0 && item.second < item.first->noChildren())
	{
		title = item.first->child(item.second).name();
	}
	else
	{
		title = item.first->title();
	}
	return title;
}

//-------------------------------------------------------------------------------------------

QVariant PlayListModel::data(const QModelIndex& index, int role) const
{
	if(index.isValid() && index.row() >= 0 && index.row() < m_playList.size())
	{
		tuint64 id = m_playList.at(index.row());
		
		if(role == IdRole)
		{
			return QVariant(id);
		}
		else
		{
			QMap<tuint64, QPair<track::db::DBInfoSPtr,tint> >::const_iterator ppI;
			
			ppI = m_items.find(id);
			if(ppI != m_items.constEnd())
			{
				track::db::DBInfoSPtr pInfo = ppI.value().first;
				
				if(role == Qt::DisplayRole)
				{
					QString s = titleOfItem(ppI.value()) + " (" + pInfo->artist() + ")";
					return QVariant(s);
				}
				else if(role == ArtistRole)
				{
					return QVariant(pInfo->artist());
				}
				else if(role == TitleRole)
				{
					QString s = titleOfItem(ppI.value());
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
			else
			{
				printError("data", "Index item not found in playlist map");
			}
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
	h[IdRole] = "id";
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

qint32 PlayListModel::indexFromId(tuint64 id) const
{
	qint32 idx = -1;
	QMap<tuint64, tint>::const_iterator ppI;
	
	ppI = m_idToIndexMap.find(id);
	if(ppI != m_idToIndexMap.constEnd())
	{
		idx = ppI.value();
	}
	return idx;
}

//-------------------------------------------------------------------------------------------

track::db::DBInfoSPtr PlayListModel::itemFromId(tuint64 id) const
{
	track::db::DBInfoSPtr pItem;
	QMap<tuint64, QPair<track::db::DBInfoSPtr,tint> >::const_iterator ppI;
	
	ppI = m_items.find(id);
	if(ppI != m_items.constEnd())
	{
		pItem = ppI.value().first;
	}
	return pItem;
}

//-------------------------------------------------------------------------------------------

tint PlayListModel::childIndexFromId(tuint64 id) const
{
	tint childIndex = -1;
	QMap<tuint64, QPair<track::db::DBInfoSPtr,tint> >::const_iterator ppI;
	
	ppI = m_items.find(id);
	if(ppI != m_items.constEnd())
	{
		childIndex = ppI.value().second;
	}
	return childIndex;
}

//-------------------------------------------------------------------------------------------

void PlayListModel::playItemAtIndexWithNext(int index, bool isNext)
{
	if(index >= 0 && index < m_playList.size())
	{
		tuint64 id = m_playList.at(index);
		track::db::DBInfoSPtr pItem = itemFromId(id);
		if(!pItem.isNull())
		{
			QString fileName = pItem->getFilename();
			tint childIdx = childIndexFromId(id);
			
			common::Log::g_Log.print("PlayListModel::playItemAtIndex - %d '%s'\n", index, fileName.toUtf8().constData());
			if(childIdx >= 0 && childIdx < pItem->noChildren())
			{
				m_pAudioInterface->playFileWithTime(fileName, pItem->child(childIdx).startTime(), pItem->child(childIdx).length(), isNext);
			}
			else
			{
				m_pAudioInterface->playFile(fileName, isNext);
			}
			m_pPlaybackState->setNextItem(id);
		}
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
		if(!isNext)
		{
			m_pPlaybackState->onAudioStop();
		}
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
