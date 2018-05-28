#include "track/model/inc/AbstractTrackModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace model
{
//-------------------------------------------------------------------------------------------

AbstractTrackModel::AbstractTrackModel() : AbstractTrackItem()
{}

//-------------------------------------------------------------------------------------------

AbstractTrackModel::AbstractTrackModel(const AbstractTrackModelSPtr& parentItem,const TrackModelKey& filterKey) : AbstractTrackItem(parentItem),
	m_filterKey(filterKey)
{}

//-------------------------------------------------------------------------------------------

AbstractTrackModel::~AbstractTrackModel()
{}

//-------------------------------------------------------------------------------------------

const TrackModelKey& AbstractTrackModel::filterKey() const
{
	return m_filterKey;
}

//-------------------------------------------------------------------------------------------
} // namespace model
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------

