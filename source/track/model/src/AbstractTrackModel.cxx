#include "track/model/inc/AbstractTrackModel.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace track
{
namespace model
{
//-------------------------------------------------------------------------------------------

AbstractTrackModel::AbstractTrackModel() : AbstractTrackItem()
{}

//-------------------------------------------------------------------------------------------

AbstractTrackModel::AbstractTrackModel(const TrackModelKey& filterKey) : AbstractTrackItem(parentItem),
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

db::SQLiteQuerySPtr AbstractTrackModel::getDBQuery() const
{
    db::SQLiteQuerySPtr pDB(new db::SQLiteQuery(db::TrackDB::instance()->db()));
	return pDB;
}

//-------------------------------------------------------------------------------------------
} // namespace model
} // namespace track
} // namespace omega
//-------------------------------------------------------------------------------------------

