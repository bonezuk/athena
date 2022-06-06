//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_PLAYERIOS_PLAYERIOSBASEMODEL_H
#define __OMEGA_PLAYERAPP_PLAYERIOS_PLAYERIOSBASEMODEL_H
//-------------------------------------------------------------------------------------------

#include <QAbstractListModel>
#include <QModelIndex>
#include <QQmlEngine>
#include <QQmlContext>

#include "track/db/inc/DBInfo.h"
#include "track/db/inc/TrackDB.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class PlayerIOSBaseModel : public QAbstractListModel
{
	Q_OBJECT
	
	public:
		enum TrackRoles
		{
			ArtistRole = Qt::UserRole + 1,
			TitleRole,
			AlbumRole
		};
		Q_ENUM(TrackRoles)
		
	public:
		PlayerIOSBaseModel(QObject *parent = 0);
		virtual ~PlayerIOSBaseModel();
		
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		virtual QHash<int,QByteArray> roleNames() const;
	
	public slots:
		virtual void appendTrack(const QString& fileName);
		
	private:
		int m_playlistID;
		mutable QSharedPointer<track::db::DBInfo> m_currentInfo;
		
		virtual void printError(const tchar *strR, const tchar *strE) const;
		virtual int sizeOfPlaylist() const;
		virtual bool getID(const QModelIndex& index, int& albumID, int& trackID, int& subtrackID) const;
		virtual QSharedPointer<track::db::DBInfo> getTrack(const QModelIndex& index, int& subtrackID) const;
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
