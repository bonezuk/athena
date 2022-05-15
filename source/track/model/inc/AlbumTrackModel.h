//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_TRACK_MODEL_ALBUMTRACKMODEL_H
#define __OMEGA_TRACK_MODEL_ALBUMTRACKMODEL_H
//-------------------------------------------------------------------------------------------

#include "track/model/inc/AbstractTrackModel.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace track
{
namespace model
{
//-------------------------------------------------------------------------------------------

class TRACK_MODEL_EXPORT AlbumTrackModel : public AbstractTrackModel
{
	public:
		AlbumTrackModel();
		AlbumTrackModel(const AbstractTrackModelSPtr& parentItem,const TrackModelKey& filterKey);
		virtual ~AlbumTrackModel();
		
		virtual TrackModelType type() const;
		
		virtual QVariant data(int sectionIndex,int rowIndex,int columnIndex) const;
		
		virtual int size() const;
		virtual int numberSections() const;
		virtual int numberRowsInSection(int secIdx) const;

		virtual bool onAddToDatabase(int albumID,int trackID);
		virtual bool onRemoveFromDatabase(int albumID,int trackID);
		
	protected:
	
		QList<TrackModelKey> m_tracks;
	
		virtual bool populate();
		virtual const AlbumModelKey& getAlbumID() const;
};

//-------------------------------------------------------------------------------------------
} // namespace model
} // namespace track
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

