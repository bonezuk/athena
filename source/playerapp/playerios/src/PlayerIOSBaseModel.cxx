#include "playerapp/playerios/inc/PlayerIOSBaseModel.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

PlayerIOSBaseModel::SoundFileItem PlayerIOSBaseModel::m_soundFileStaticData[NO_SOUNDFILES_IN_MODEL] = {
	{	// 0
		"Danger Zone",
		"Kenny Loggins",
		"Top Gun"
	},
	{	// 1
		"Mighty Wings",
		"Cheap Trick",
		"Top Gun"
	},
	{	// 2
		"Playing with the Boys",
		"Kenny Loggins",
		"Top Gun"
	},
	{	// 3
		"Lead Me On",
		"Teena Marie",
		"Top Gun"
	},
	{	// 4
		"Take My Breath Away",
		"Berlin",
		"Top Gun"
	},
	{	// 5
		"Hot Summer Nights",
		"Miami Sound Machine",
		"Top Gun"
	},
	{	// 6
		"Heaven in Your Eyes",
		"Loverboy",
		"Top Gun"
	},
	{	// 7
		"Through the Fire",
		"Larry Greene",
		"Top Gun"
	},
	{	// 8
		"Destination Unknown",
		"Marietta",
		"Top Gun"
	},
	{	// 9
		"Top Gun Anthem",
		"Harold Faltermeyer, Steve Stevens",
		"Top Gun"
	},
};

//-------------------------------------------------------------------------------------------

PlayerIOSBaseModel::PlayerIOSBaseModel(QObject *parent) : QAbstractListModel(parent)
{}

//-------------------------------------------------------------------------------------------

PlayerIOSBaseModel::~PlayerIOSBaseModel()
{}

//-------------------------------------------------------------------------------------------

int PlayerIOSBaseModel::rowCount(const QModelIndex& parent) const
{
	if(parent.isValid())
	{
		return 0;
	}
	return NO_SOUNDFILES_IN_MODEL;
}

//-------------------------------------------------------------------------------------------

QVariant PlayerIOSBaseModel::data(const QModelIndex& index, int role) const
{
	if(index.isValid() && index.row() >= 0 && index.row() < NO_SOUNDFILES_IN_MODEL)
	{
		if(role == Qt::DisplayRole)
		{
			QString track = QString::fromUtf8(m_soundFileStaticData[index.row()].track);
			QString artist = QString::fromUtf8(m_soundFileStaticData[index.row()].artist);
			QString s = track + " (" + artist + ")";
			return QVariant(s);
		}
		else if(role == ArtistRole)
		{
			QString s = QString::fromUtf8(m_soundFileStaticData[index.row()].artist);
			return QVariant(s);
		}
		else if(role == TitleRole) 
		{
			QString s = QString::fromUtf8(m_soundFileStaticData[index.row()].track);
			return QVariant(s);
		}
		else if(role == AlbumRole)
		{
			QString s = QString::fromUtf8(m_soundFileStaticData[index.row()].album);
			return QVariant(s);
		}
	}
	return QVariant();
}

//-------------------------------------------------------------------------------------------

QHash<int,QByteArray> PlayerIOSBaseModel::roleNames() const
{
	QHash<int,QByteArray> h;
	h[ArtistRole] = "artist";
	h[TitleRole] = "title";
	h[AlbumRole] = "album";
	return h;
}

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
