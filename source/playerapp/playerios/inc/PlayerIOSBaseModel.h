//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERIOS_PLAYERIOSBASEMODEL_H
#define __ORCUS_PLAYERAPP_PLAYERIOS_PLAYERIOSBASEMODEL_H
//-------------------------------------------------------------------------------------------

#include <QAbstractListModel>
#include <QModelIndex>
#include <QQmlEngine>
#include <QQmlContext>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

#define NO_SOUNDFILES_IN_MODEL 10

class PlayerIOSBaseModel : public QAbstractListModel
{
	Q_OBJECT
	
	public:
		typedef struct s_SoundFileItem
		{
			const char *track;
			const char *artist;
			const char *album;
		} SoundFileItem;

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
		
	protected:
		static SoundFileItem m_soundFileStaticData[NO_SOUNDFILES_IN_MODEL];
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
