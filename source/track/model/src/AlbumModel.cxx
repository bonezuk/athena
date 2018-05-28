#include "track/model/inc/AlbumModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace model
{
//-------------------------------------------------------------------------------------------

AlbumModel::AlbumModel() : AbstractTrackModel(),
	m_albums()
{}

//-------------------------------------------------------------------------------------------

AlbumModel::AlbumModel(const AbstractTrackModelSPtr& parentItem,const TrackModelKey& filterKey) : AbstractTrackModel(parentItem,filterKey),
	m_albums()
{}

//-------------------------------------------------------------------------------------------

AlbumModel::~AlbumModel()
{}

//-------------------------------------------------------------------------------------------

TrackModelType AlbumModel::type() const
{
	return e_modelAlbums;
}

//-------------------------------------------------------------------------------------------

QVariant AlbumModel::data(int sectionIndex,int rowIndex,int columnIndex) const
{
	QVariant dataItem;

	if(sectionIndex>=0 && sectionIndex<static_cast<int>(m_index.size()))
	{
		if(rowIndex>=0 && rowIndex<numberRowsInSection(sectionIndex))
		{
			int aIndex = rowIndex + m_index.at(sectionIndex);
			if(columnIndex>=0 && columnIndex<2)
			{
				const QPair<AlbumModelKey,QString>& item = m_albums.at(aIndex);
				if(columnIndex==0)
				{
                    dataItem.setValue(item.first.variant());
				}
				else
				{
					dataItem.setValue(item.second);
				}
			}
		}
	}
	return dataItem;
}

//-------------------------------------------------------------------------------------------

int AlbumModel::size() const
{
	return m_albums.size();
}

//-------------------------------------------------------------------------------------------

int AlbumModel::numberSections() const
{
	return m_index.size();
}

//-------------------------------------------------------------------------------------------

int AlbumModel::numberRowsInSection(int secIdx) const
{
	if(secIdx>=0 && secIdx<static_cast<int>(m_index.size()))
	{
		int sPosition = m_index.at(secIdx);
		int ePosition = ((secIdx + 1) < static_cast<int>(m_index.size())) ? m_index.at(secIdx+1) : m_albums.size();
		if(sPosition < ePosition)
		{
			return ePosition - sPosition;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------------

bool AlbumModel::build()
{
	return populate();
}

//-------------------------------------------------------------------------------------------

bool AlbumModel::onAddToDatabase(int albumID,int trackID)
{
	// TODO
	return false;
}

//-------------------------------------------------------------------------------------------

bool AlbumModel::onRemoveFromDatabase(int albumID,int trackID)
{
	// TODO
	return false;
}

//-------------------------------------------------------------------------------------------

bool AlbumModel::populate()
{
	QString cmdQ;
	QueryResult results;
	AlbumModelKey albumKey;
	bool res = false;
	
	cmdQ = getQuery(albumKey);
	if(runAlbumQuery(cmdQ,results))
	{
		enumerateSections(results);
		res = true;
	}
	return res;
}

//-------------------------------------------------------------------------------------------
// The enumerateSections is used to scan through the album names.
// Based on the lower case first letter of the trimmed album name
// they are placed into sections defining all album names beginning
// with that associated letter.
//-------------------------------------------------------------------------------------------

void AlbumModel::enumerateSections(const QueryResult& results)
{
	QVector<QChar> alphabet;
	QMap<QChar,int> indexMap;
	QMap<QChar,QMap<QString,int> > sectionMap;
	
	alphabet = getIndexAlphabet();
	buildIndexMap(alphabet,indexMap);
	mapResultsToAlphabetIndex(results,indexMap,alphabet,sectionMap);
	buildModelFromSortedIndex(results,alphabet,sectionMap);
}

//-------------------------------------------------------------------------------------------

void AlbumModel::insertIntoAlbum(QVector<QueryRecord>& recordList)
{
	qSort(recordList.begin(),recordList.end(),AlbumModel::compareIdenticalAlbumNameLessThan);
	
	for(QVector<QueryRecord>::const_iterator ppI=recordList.begin();ppI!=recordList.end();ppI++)
	{
		const QueryRecord& record = *ppI;
		m_albums.append(QPair<AlbumModelKey,QString>(keyRecordAlbum(record),nameRecordAlbum(record)));
	}
	recordList.clear();
}

//-------------------------------------------------------------------------------------------

void AlbumModel::addToModelForGivenMap(const QueryResult& results,const QMap<QString,int>& map)
{
	QVector<QueryRecord> recordList;

	for(QMap<QString,int>::const_iterator ppI=map.begin();ppI!=map.end();ppI++)
	{
		const QueryRecord& record = results.at(ppI.value());
		
		if(!recordList.isEmpty())
		{
			QString prevIndexName = nameRecordAlbum(recordList.at(0)).trimmed().toLower();
			if(prevIndexName!=ppI.key())
			{
				insertIntoAlbum(recordList);
			}
		}
		recordList.append(record);
	}
	insertIntoAlbum(recordList);
}

//-------------------------------------------------------------------------------------------

void AlbumModel::buildModelFromSortedIndex(const QueryResult& results,const QVector<QChar>& alphabet,const QMap<QChar,QMap<QString,int> >& sectionMap)
{
	m_albums.clear();
    m_index.clear();
	
	for(int i=0;i<alphabet.size();i++)
	{
        QMap<QChar,QMap<QString,int> >::const_iterator ppI;
		
		m_index.push_back(m_albums.size());
        ppI = sectionMap.find(alphabet.at(i));
        if(ppI!=sectionMap.end())
		{
            addToModelForGivenMap(results,ppI.value());
		}
	}
}

//-------------------------------------------------------------------------------------------
// a < b -> true
// a > b -> false
// album < group
// a.album < b.group -> true
// a.group > b.album -> false
//-------------------------------------------------------------------------------------------

bool AlbumModel::compareIdenticalAlbumNameLessThan(const QueryRecord& a,const QueryRecord& b)
{
	AlbumModelKey keyA = keyRecordAlbumStatic(a);
	AlbumModelKey keyB = keyRecordAlbumStatic(b);
	bool res = false;
	
	if(keyA.isGroup())
	{
		if(keyB.isGroup())
		{
            res = (keyA.id() < keyB.id()) ? true : false;
		}
		else
		{
			res = false;
		}
	}
	else
	{
		if(keyB.isAlbum())
		{
            res = (keyA.id() < keyB.id()) ? true : false;
		}
		else
		{
            res = true;
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void AlbumModel::buildIndexMap(const QVector<QChar>& alphabet,QMap<QChar,int>& indexMap) const
{
	indexMap.clear();
	for(int i=0;i<alphabet.size();i++)
	{
		indexMap.insert(alphabet.at(i),i);
	}
}

//-------------------------------------------------------------------------------------------

QChar AlbumModel::getFirstCharacter(const QString& name,const QMap<QChar,int>& indexMap,const QVector<QChar>& alphabet) const
{
	QChar c;
	QString n = name.trimmed();
	
	if(!n.isEmpty())
	{
		c = n.at(0).toLower();
		if(indexMap.find(c)==indexMap.end())
		{
			c = alphabet.at(alphabet.size() - 1);
		}
	}
	else
	{
		c = alphabet.at(alphabet.size() - 1);
	}
	return c;
}

//-------------------------------------------------------------------------------------------

void AlbumModel::mapResultsToAlphabetIndex(const QueryResult& results,const QMap<QChar,int>& indexMap,const QVector<QChar>& alphabet,QMap<QChar,QMap<QString,int> >& sectionMap) const
{
	int i;

	sectionMap.clear();
	
	for(i=0;i<static_cast<int>(results.size());i++)
	{
		QString name = nameRecordAlbum(results.at(i)).trimmed();
		QChar firstChar = getFirstCharacter(name,indexMap,alphabet);
		QMap<QChar,QMap<QString,int> >::iterator ppJ = sectionMap.find(firstChar);
		if(ppJ==sectionMap.end())
		{
			QMap<QString,int> newMap;
			sectionMap.insert(firstChar,newMap);
			ppJ = sectionMap.find(firstChar);
		}
		QMap<QString,int>& rMap = ppJ.value();
		rMap.insertMulti(name.toLower(),i);
	}
}

//-------------------------------------------------------------------------------------------
// Get the alphabet sequence in unicode format, allows for internationization.
// The default implementation gives the alphabet.
// All alphabet characters are given in lowercase.
// The end character is used to represent all names beginning without a letter in
// the given alphabet.
//-------------------------------------------------------------------------------------------

QVector<QChar> AlbumModel::getIndexAlphabet() const
{
	QVector<QChar> alphabet;
	tchar letter;
	
	letter = 'a';
    while(letter <= 'z')
	{
		alphabet.append(QChar(letter));
		letter++;
	}
	alphabet.append(QChar('#'));
	return alphabet;
}

//-------------------------------------------------------------------------------------------

QString AlbumModel::getQuery(const AlbumModelKey& albumID) const
{
	QString cmdQ;

	cmdQ  = "SELECT CASE WHEN a.groupid>=0 THEN 1 ELSE 0 END AS flag, ";
	cmdQ += "CASE WHEN a.groupid>=0 THEN a.groupid ELSE a.albumid END AS id, ";
	cmdQ += "a.albumname ";
	
	cmdQ += "FROM album AS a ";
	if(m_filterKey.isArtist() || m_filterKey.isGenre())
	{
		cmdQ += "INNER JOIN track AS b ON a.albumID=b.albumID ";
	}
	
    if(!albumID.isAll() || m_filterKey.isArtist() || m_filterKey.isGenre())
	{
		cmdQ += "WHERE ";
	
        if(!albumID.isAll())
		{
			cmdQ += (albumID.isGroup()) ? "a.groupID" : "a.albumID";
			cmdQ += "=" + QString::number(albumID.id()) + " ";
		}
		
		if(m_filterKey.isArtist() || m_filterKey.isGenre())
		{			
			if(m_filterKey.isGenre())
			{
                if(!albumID.isAll())
				{
					cmdQ += "AND ";
				}

				cmdQ += "b.genreID=" + QString::number(m_filterKey.genre()) + " ";
			}
			
			if(m_filterKey.isArtist())
			{
				QString dbArtist = db::TrackDB::dbString(m_filterKey.artist());
				
				if(!albumID.isAll() || m_filterKey.isGenre())
				{
					cmdQ += "AND ";
				}
				
				cmdQ += "(b.artist LIKE \'" + dbArtist + "\' OR ";
				cmdQ += "b.originalArtist LIKE \'" + dbArtist + "\' OR ";
				cmdQ += "b.composer LIKE \'" + dbArtist + "\') ";

			}
		}
	}
	
	cmdQ += "GROUP BY id ";
	cmdQ += "ORDER BY a.albumname";

	return cmdQ;
}

//-------------------------------------------------------------------------------------------

QueryRecord AlbumModel::createRecordAlbum(const AlbumModelKey& key,const QString& name) const
{
	QueryRecord r;
	r.push_back(key.isGroup());
	r.push_back(key.id());
    r.push_back(name);
	return r;
}

//-------------------------------------------------------------------------------------------

AlbumModelKey AlbumModel::keyRecordAlbum(const QueryRecord& record) const
{
	return keyRecordAlbumStatic(record);
}

//-------------------------------------------------------------------------------------------

AlbumModelKey AlbumModel::keyRecordAlbumStatic(const QueryRecord& record)
{
	std::pair<bool,int> key(false,-1);
	if(record.size()>=2)
	{
        key = std::pair<bool,int>(record.at(0).toBool(),record.at(1).toInt());
	}
	AlbumModelKey k(key);
	return k;
}

//-------------------------------------------------------------------------------------------

QString AlbumModel::nameRecordAlbum(const QueryRecord& record) const
{
	return nameRecordAlbumStatic(record);
}

//-------------------------------------------------------------------------------------------

QString AlbumModel::nameRecordAlbumStatic(const QueryRecord& record)
{
	QString name;
	if(record.size()>=3)
	{
		name = record.at(2).toString();
	}
    return name;
}

//-------------------------------------------------------------------------------------------

bool AlbumModel::runAlbumQuery(const QString& cmdQ,QueryResult& results) const
{
	bool res = false;
	
	try
	{
		bool isGroup;
		int albumID;
		QString albumName;
		db::SQLiteQuerySPtr query = getDBQuery();
		
		query->prepare(cmdQ);
		query->bind(isGroup);
		query->bind(albumID);
		query->bind(albumName);
		
		while(query->next())
		{
			AlbumModelKey key(std::pair<bool,int>(isGroup,albumID));
			results.push_back( createRecordAlbum(key,albumName) );
		}
		res = true;
	}
	catch(const db::SQLiteException&)
	{
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------
} // namespace model
} // namepsace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
