//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_PLAYERCOMMON_PLAYLISTMODEL_H
#define __OMEGA_PLAYERAPP_PLAYERCOMMON_PLAYLISTMODEL_H
//-------------------------------------------------------------------------------------------

#include <QAbstractListModel>
#include <QModelIndex>
#include <QQmlEngine>
#include <QQmlContext>

#include "common/inc/Random.h"
#include "playerapp/playercommon/inc/PlaybackStateController.h"
#include "playerapp/playercommon/inc/OmegaAudioInterface.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT PlayListModel : public QAbstractListModel
{
	Q_OBJECT

	public:
		enum TrackRoles
		{
			ArtistRole = Qt::UserRole + 1,
			IdRole,
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
		
		Q_PROPERTY(qint32 sizeOfModel READ getSizeOfModel NOTIFY onSizeOfModel)

	signals:
		void onSizeOfModel();
		
	public:
		PlayListModel(QObject *parent = 0);
		PlayListModel(QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent = 0);
		PlayListModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent = 0);
		virtual ~PlayListModel();
		
		virtual bool initialise();
		
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		virtual QHash<int,QByteArray> roleNames() const;
		
		Q_INVOKABLE void onPlayPausePressed();
		Q_INVOKABLE void playItemAtIndex(int index);
		Q_INVOKABLE QVariant dataAtIndex(int row, const QString& roleName);
		
		virtual QSharedPointer<PlaybackStateController>& playbackState();
		virtual void playNextItem(bool isNext);
		
		virtual qint32 indexFromId(tuint64 id) const;
		virtual track::db::DBInfoSPtr itemFromId(tuint64 id) const;
		virtual tint childIndexFromId(tuint64 id) const;
		
		virtual qint32 getSizeOfModel() const;

	protected:
		QMap<tuint64, QPair<track::db::DBInfoSPtr,tint> > m_items;
		QVector<tuint64> m_playList;
		QMap<tuint64, tint> m_idToIndexMap;
		
		QSharedPointer<OmegaAudioInterface> m_pAudioInterface;
		QSharedPointer<PlaybackStateController> m_pPlaybackState;
		
		virtual void printError(const char *strR, const char *strE) const;

		virtual tuint64 generateNewId() const;
		virtual void appendToPlaylist(const QVector<QPair<track::db::DBInfoSPtr,tint> >& list);
		virtual QString titleOfItem(const QPair<track::db::DBInfoSPtr,tint>& item) const;
		virtual void playItemAtIndexWithNext(int index, bool isNext);
		virtual int sizeOfPlaylist() const;
		
		virtual void endInsertRows();
		virtual void endRemoveRows();
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
