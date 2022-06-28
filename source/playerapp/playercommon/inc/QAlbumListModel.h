//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_PLAYERCOMMON_QALBUMLISTMODEL_H
#define __OMEGA_PLAYERAPP_PLAYERCOMMON_QALBUMLISTMODEL_H
//-------------------------------------------------------------------------------------------

#include <QAbstractListModel>
#include <QModelIndex>
#include <QQmlEngine>
#include <QQmlContext>

#include "track/model/inc/AlbumModel.h"
#include "playerapp/playercommon/inc/QAlbumTrackListModel.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT QAlbumListModel : public QAbstractListModel
{
	Q_OBJECT
	public:
		enum AlbumListRoles
		{
			ArtistRole = Qt::UserRole + 1,
			AlbumRole,
			YearRole,
		};
		Q_ENUM(AlbumListRoles)
		
	public:
		QAlbumListModel(QObject *parent = 0);
		virtual ~QAlbumListModel();
		
		virtual bool initialise();
		virtual QAlbumTrackListModel *trackModel();
		
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		virtual QHash<int,QByteArray> roleNames() const;
		
		Q_INVOKABLE void showAlbumTracks(int index);
		
	public slots:
		virtual void appendTrack(const QString& fileName);
		virtual void deleteTrack(const QString& fileName);

	private:
		QSharedPointer<track::model::AlbumModel> m_pAlbums;
		QAlbumTrackListModel *m_pTrackModel;
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

