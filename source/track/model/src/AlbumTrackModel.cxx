#include "track/model/inc/AlbumTrackModel.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace track
{
namespace model
{
//-------------------------------------------------------------------------------------------

AlbumTrackModel::AlbumTrackModel() : AbstractTrackModel()
{}

//-------------------------------------------------------------------------------------------

AlbumTrackModel::AlbumTrackModel(const AbstractTrackModelSPtr& parentItem,const TrackModelKey& filterKey) : AbstractTrackModel(parentItem,filterKey)
{}

//-------------------------------------------------------------------------------------------

AlbumTrackModel::~AlbumTrackModel()
{}

//-------------------------------------------------------------------------------------------

TrackModelType AlbumTrackModel::type() const
{
	return e_modelAlbumTracks;
}

//-------------------------------------------------------------------------------------------

QVariant AlbumTrackModel::data(int sectionIndex,int rowIndex,int columnIndex) const
{
	// TODO
	return QVariant();
}

//-------------------------------------------------------------------------------------------

int AlbumTrackModel::size() const
{
	//TODO
	return 0;
}

//-------------------------------------------------------------------------------------------

int AlbumTrackModel::numberSections() const
{
	return 1;
}

//-------------------------------------------------------------------------------------------

int AlbumTrackModel::numberRowsInSection(int secIdx) const
{
	//TODO
	return 0;
}

//-------------------------------------------------------------------------------------------

bool AlbumTrackModel::onAddToDatabase(int albumID,int trackID)
{
	//TODO
	return false;
}

//-------------------------------------------------------------------------------------------

bool AlbumTrackModel::onRemoveFromDatabase(int albumID,int trackID)
{
	//TODO
	return false;
}

//-------------------------------------------------------------------------------------------

bool AlbumTrackModel::populate()
{
	return false;
}

	// SELECT CASE WHEN a.groupid>=0 THEN 1 ELSE 0 END AS flag,
	//        CASE WHEN a.groupid>=0 THEN a.groupid ELSE a.albumid END AS id,
	//        a.albumname
	//   FROM album AS a INNER JOIN track AS b ON a.albumID=b.albumID
	//   WHERE a.albumID=2 AND
	//         b.genreID=3 AND
	//         (b.artist LIKE 'Steve Jablonsky' OR
	//          b.originalArtist LIKE 'Steve Jablonsky' OR
	//          b.composer LIKE 'Steve Jablonsky')
	//   GROUP BY id
	//   ORDER BY a.albumname

	// SELECT CASE WHEN a.groupid>=0 THEN 1 ELSE 0 END AS flag,
	//        CASE WHEN a.groupid>=0 THEN a.groupid ELSE a.albumid END AS id,
	//        a.albumname
	//   FROM album AS a INNER JOIN track AS b ON a.albumID=b.albumID
	//   WHERE a.groupID=20 AND
	//         b.genreID=1 AND
	//         (b.artist LIKE 'John Williams' OR
	//          b.originalArtist LIKE 'John Williams' OR
	//          b.composer LIKE 'John Williams')
	//   GROUP BY id
	//   ORDER BY a.albumname

// Handling of files containing same song data.
// An album is identified by its key.
// name = item->album() + ":" + item->year() + ":" + item->directoryGroup() + ":" + item->getName("%Z%N%T",contentFlag,true);
// Two files are said to have an equivalent track IF
// 1. They both have the same album key (as defined above).
// 2. The track names are reasonably identical.
// 3. They are both numbered with the same track position.
//
// Suggested implementation.
// Add a new extension to the file table that contains a link to another directory/file table
// entry of the duplicate file.
// The model will support this new extension suh that it first points to the first file.
// The file can then be queried to give additional files of the same track information.

// Should the track model use the existing DBItem instance?

/*
SELECT CASE WHEN a.groupid>=0 THEN 1 ELSE 0 END AS flag
       CASE WHEN a.groupid>=0 THEN a.groupid ELSE a.albumid END AS albumid,
       b.trackid,
       a.directoryid,
       b.fileid,
*/     

//-------------------------------------------------------------------------------------------

const AlbumModelKey& AlbumTrackModel::getAlbumID() const
{
	return m_filterKey.album();
}

//-------------------------------------------------------------------------------------------
} // namespace model
} // namespace track
} // namespace omega
//-------------------------------------------------------------------------------------------

