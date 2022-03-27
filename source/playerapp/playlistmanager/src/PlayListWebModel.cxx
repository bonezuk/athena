#include "playerapp/playlistmanager/inc/PlayListWebModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlayListWebModel::PlayListWebModel(QObject *parent) : PlayListModel(parent),
	OmegaWebInterface()
{}

//-------------------------------------------------------------------------------------------

PlayListWebModel::PlayListWebModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playList, QSharedPointer<OmegaAudioInterface>& pAudioInterface, QObject *parent) : PlayListModel(playList, pAudioInterface, parent),
	OmegaWebInterface()
{}

//-------------------------------------------------------------------------------------------

PlayListWebModel::~PlayListWebModel()
{}

//-------------------------------------------------------------------------------------------

void PlayListWebModel::initialise()
{
	QSharedPointer<PlaybackWebStateController> plState(new PlaybackWebStateController(m_pAudioInterface, this));
	m_pPlaybackState = plState.dynamicCast<PlaybackStateController>();
}

//-------------------------------------------------------------------------------------------

int PlayListWebModel::playlistSize()
{
	return m_playList.size();
}

//-------------------------------------------------------------------------------------------

QJsonDocument PlayListWebModel::playlistAsJson(int fromIndex, int toIndex)
{
	int idx;
	QJsonDocument doc;
	QJsonArray array;

	if(fromIndex > toIndex)
	{
		common::xorSwap(fromIndex, toIndex);
	}
	if(toIndex < 0 || fromIndex >= m_playList.size())
	{
		return doc;
	}
	
	if(fromIndex < 0)
	{
		fromIndex = 0;
	}
	if(toIndex > m_playList.size())
	{
		toIndex = m_playList.size();
	}
	for(idx = fromIndex; idx < toIndex; idx++)
	{
		QJsonValue v = playlistItemToJson(idx);
		if(!v.isNull())
		{
			array.append(v);
		}
	}
	doc.setArray(array);
	
	return doc;
}

//-------------------------------------------------------------------------------------------
/*
{
	"id": 312681263,
	"title": "Shelter in the Storm",
	"artist": "Dare",
	"album": "Arc of the Dawn",
	"year": "2009",
	"comment": "random track",
	"genre": "Rock",
	"disc": "",
	"track": "2",
	"composer": "Dare",
	"original_artist": "Dare",
	"copyright": "Copyright title",
	"encoder": "m4a encoder",
	"length": 4.35,
	"bitrate": 160000,
	"no_channels": 2,
	"frequency": 44100
}
*/
//-------------------------------------------------------------------------------------------

QJsonValue PlayListWebModel::playlistItemToJson(int idx)
{
	tuint64 id;
	QJsonValue jValue;

	if(idx >= 0 && idx < playlistSize())
	{
		QMap<tuint64, QPair<track::db::DBInfoSPtr,tint> >::const_iterator ppI;
		
		id = m_playList.at(idx);
		ppI = m_items.find(id);
		if(ppI != m_items.constEnd())
		{
			QVariantMap tMap;
			const QPair<track::db::DBInfoSPtr,tint>& item = ppI.value();
			
			tMap.insert("id", QVariant(id));
			tMap.insert("title", QVariant(titleOfItem(item)));
			tMap.insert("artist", QVariant(item.first->artist()));
			tMap.insert("album", QVariant(item.first->album()));
			tMap.insert("year", QVariant(item.first->year()));
			tMap.insert("comment", QVariant(item.first->comment()));
			tMap.insert("genre", QVariant(item.first->genre()));
			tMap.insert("disc", QVariant(item.first->disc()));
			tMap.insert("track", QVariant(item.first->track()));
			tMap.insert("composer", QVariant(item.first->composer()));
			tMap.insert("original_artist", QVariant(item.first->originalArtist()));
			tMap.insert("copyright", QVariant(item.first->copyright()));
			tMap.insert("encoder", QVariant(item.first->encoder()));
			tMap.insert("length", QVariant(static_cast<tfloat64>(item.first->length())));
			tMap.insert("bitrate", QVariant(item.first->bitrate()));
			tMap.insert("no_channels", QVariant(item.first->noChannels()));
			tMap.insert("frequency", QVariant(item.first->frequency()));
		
			QJsonValue v(QJsonObject::fromVariantMap(tMap));
			jValue = v;
		}
	}
	return jValue;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
