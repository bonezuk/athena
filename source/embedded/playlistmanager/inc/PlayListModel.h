//-------------------------------------------------------------------------------------------
#ifndef __EMBEDDED_PLAYLISTMANAGER_PLAYLISTMODEL_H
#define __EMBEDDED_PLAYLISTMANAGER_PLAYLISTMODEL_H
//-------------------------------------------------------------------------------------------

#include <QAbstractListModel>
#include <QModelIndex>

#include "track/db/inc/DBInfo.h"
#include "track/db/inc/TrackDB.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PlayListModel : public QAbstractListModel
{
	Q_OBJECT
	Q_ENUMS(TrackRoles)
	
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
		
	public:
		PlayListModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QObject *parent = 0);
		virtual ~PlayListModel();
		
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		virtual QHash<int,QByteArray> roleNames() const;
		
	private:
	
		QVector<QPair<track::db::DBInfoSPtr,tint> > m_playList;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

