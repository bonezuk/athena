//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_TRACK_MODEL_ABSTRACTTRACKMODEL_H
#define __OMEGA_TRACK_MODEL_ABSTRACTTRACKMODEL_H
//-------------------------------------------------------------------------------------------

#include "track/model/inc/AbstractTrackItem.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace track
{
namespace model
{
//-------------------------------------------------------------------------------------------

typedef enum
{
	e_modelAlbums = 1,
	e_modelAlbumTracks,
	e_modelSongs,
	e_modelArtist,
	e_modelGenre,
	e_modelArtistAlbums,
	e_modelPlaylist,
	e_modelUnknown = 0
} TrackModelType;

//-------------------------------------------------------------------------------------------
// Model follows a parent / child relationship. e.g.
//   Album List Model -> Album Tracks -> Track Item
//   Songs -> Track Item
// For this reason the model inherits AbstractTrackItem
//-------------------------------------------------------------------------------------------

class TRACK_MODEL_EXPORT AbstractTrackModel : public AbstractTrackItem
{
	public:
		AbstractTrackModel();
		AbstractTrackModel(const AbstractTrackModelSPtr& parentItem,const TrackModelKey& filterKey);
		virtual ~AbstractTrackModel();
		
		const TrackModelKey& filterKey() const;
		
		virtual TrackModelType type() const = 0;
		
        virtual QVariant data(int sectionIndex,int rowIndex,int columnIndex) const = 0;
		
		virtual int size() const = 0;
		virtual int numberSections() const = 0;
		virtual int numberRowsInSection(int secIdx) const = 0;

		virtual bool onAddToDatabase(int albumID,int trackID) = 0;
		virtual bool onRemoveFromDatabase(int albumID,int trackID) = 0;
		
	protected:
		
		TrackModelKey m_filterKey;
		
		virtual bool populate() = 0;
};

//-------------------------------------------------------------------------------------------
} // namespace model
} // namespace track
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
