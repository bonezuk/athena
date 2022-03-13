//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERCOMMON_PLAYLISTMODEL_H
#define __ORCUS_PLAYERAPP_PLAYERCOMMON_PLAYLISTMODEL_H
//-------------------------------------------------------------------------------------------

#include <QAbstractListModel>
#include <QModelIndex>
#include <QQmlEngine>
#include <QQmlContext>

#include "playerapp/playercommon/inc/PlaybackStateController.h"
#include "playerapp/playercommon/inc/OmegaAudioInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT PlayListModel : public QAbstractListModel
{
	Q_OBJECT

	public:
		enum TrackRoles
		{
			ArtistRole = Qt::UserRole + 1,
			TitleRole,
			AlbumRole,
			YearRole,
			CommentRole,
			GenreRole,
			TrackRole,
			DiscRole,
			ComposerRole,
			OriginalArtistRole,
			CopyrightRole,
			EncoderRole
		};
		Q_ENUM(TrackRoles)
		
	public:
		PlayListModel(QObject *parent = 0);
		PlayListModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent = 0);
		virtual ~PlayListModel();
		
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		virtual QHash<int,QByteArray> roleNames() const;
		
		Q_INVOKABLE void onPlayPausePressed();
		Q_INVOKABLE void playItemAtIndex(int index);
		
		virtual QSharedPointer<PlaybackStateController>& playbackState();
		virtual void playNextItem(bool isNext);

	protected:
		QVector<QPair<track::db::DBInfoSPtr,tint> > m_playList;
		QSharedPointer<OmegaAudioInterface> m_pAudioInterface;
		QSharedPointer<PlaybackStateController> m_pPlaybackState;
		
		virtual void printError(const char *strR, const char *strE) const;

		virtual void playItemAtIndexWithNext(int index, bool isNext);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
