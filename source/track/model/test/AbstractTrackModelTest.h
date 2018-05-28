//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_MODEL_TEST_ABSTRACTTRACKMODELTEST_H
#define __ORCUS_TRACK_MODEL_TEST_ABSTRACTTRACKMODELTEST_H
//-------------------------------------------------------------------------------------------

#include "track/model/inc/AbstractTrackModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace model
{
//-------------------------------------------------------------------------------------------

class AbstractTrackModelTest : public AbstractTrackModel
{
	public:
		AbstractTrackModelTest();
		AbstractTrackModelTest(const AbstractTrackModelSPtr& parentItem,const TrackModelKey& filterKey);
		
		virtual TrackModelType type() const;
        virtual QVariant data(int sectionIndex,int rowIndex,int columnIndex) const;
		virtual int size() const;
		virtual int numberSections() const;
		virtual int numberRowsInSection(int secIdx) const;
		virtual bool onAddToDatabase(int albumID,int trackID);
		virtual bool onRemoveFromDatabase(int albumID,int trackID);
	protected:
	
		virtual bool populate();
};

//-------------------------------------------------------------------------------------------
} // namespace model
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
