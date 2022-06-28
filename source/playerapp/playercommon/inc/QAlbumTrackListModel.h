//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_PLAYERCOMMON_QALBUMTRACKLISTMODEL_H
#define __OMEGA_PLAYERAPP_PLAYERCOMMON_QALBUMTRACKLISTMODEL_H
//-------------------------------------------------------------------------------------------

#include <QAbstractListModel>
#include <QModelIndex>
#include <QQmlEngine>
#include <QQmlContext>

#include "common/inc/DiskOps.h"
#include "track/model/inc/AlbumTrackModel.h"
#include "playerapp/playercommon/inc/PlayerCommonDLL.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT QAlbumTrackListModel : public QAbstractListModel
{
	Q_OBJECT
	public:
		enum AlbumTrackRoles
		{
			TrackRole = Qt::UserRole + 1,
			AlbumRole,
			FileRole,
			LengthRole,
			ArtistRole,
			OriginalArtistRole,
			ComposerRole
		};
		Q_ENUM(AlbumTrackRoles)
		
	public:
		QAlbumTrackListModel(QObject *parent = 0);
		virtual ~QAlbumTrackListModel();

		virtual void load(const track::model::TrackModelKey& key);
		
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		virtual QHash<int,QByteArray> roleNames() const;

	public slots:
		virtual void appendTrack(const QString& fileName);
		virtual void deleteTrack(const QString& fileName);
		
	private:
		QSharedPointer<track::model::AlbumTrackModel> m_pTracks;
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
