#include "embedded/playlistmanager/inc/PlayListModel.h"
#include "embedded/omegapicommon/inc/OmegaPiDBusServiceNames.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlayListModel::PlayListModel() : QAbstractListModel(0),
	m_playList(),
	m_pAudioInterface(),
	m_pQmlEngine()
{}

//-------------------------------------------------------------------------------------------

PlayListModel::PlayListModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QDBusInterface *pAudioInterface, QQmlEngine *pQmlEngine, QObject *parent) : QAbstractListModel(parent),
	m_playList(playList),
	m_pAudioInterface(pAudioInterface),
	m_pQmlEngine(pQmlEngine)
{}

//-------------------------------------------------------------------------------------------

PlayListModel::~PlayListModel()
{}

//-------------------------------------------------------------------------------------------

QQmlEngine *PlayListModel::qmlEngine()
{
	return m_pQmlEngine;
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
			fprintf(stdout, "%s\n", s.toUtf8().constData());
			return QVariant(s);
		}
		else if(role == ArtistRole)
		{
			return QVariant(pInfo->artist());
		}
		else if(role == TitleRole)
		{
			QString s = pInfo->title();
			fprintf(stdout, "title - %s\n", s.toUtf8().constData());
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
#if defined(OMEGA_LINUX)
		QDBusConnection bus = QDBusConnection::systemBus();
#else
		QDBusConnection bus = QDBusConnection::sessionBus();
#endif
		if(bus.isConnected())
		{
			QDBusInterface audioDaemonIFace(OMEGAAUDIODAEMON_SERVICE_NAME, "/", OMEGAAUDIODAEMON_DBUS_IFACE_NAME, bus);
			if(audioDaemonIFace.isValid())
			{
				QDBusMessage reply;
				reply = m_pAudioInterface->call(QLatin1String("playFile"), fileName);
				fprintf(stderr, "reply - %s, %s\n", reply.errorName().toUtf8().constData(), reply.errorMessage().toUtf8().constData());
				fprintf(stderr, "sysbus - %s\n",m_pAudioInterface->connection().lastError().message().toUtf8().constData());
			}
			else
			{
				common::Log::g_Log << "Failed to connect to Omega Audio Daemon." << common::c_endl;
				common::Log::g_Log << qPrintable(bus.lastError().message()) << common::c_endl;
			}
		}
		else
		{
			common::Log::g_Log << "Failed to connect to session D-Bus" << common::c_endl;
		}
	}
	else
	{
		common::Log::g_Log.print("PlayListModel::playItemAtIndex - Given index, %d, out of range\n", index);
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
