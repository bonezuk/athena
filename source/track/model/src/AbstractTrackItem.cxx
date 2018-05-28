#include "track/model/inc/AbstractTrackItem.h"
#include "track/model/inc/AbstractTrackModel.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace model
{
//-------------------------------------------------------------------------------------------

AbstractTrackItem::AbstractTrackItem() : m_parent()
{}

//-------------------------------------------------------------------------------------------

AbstractTrackItem::AbstractTrackItem(const AbstractTrackModelSPtr& parentItem) : m_parent(parentItem)
{}

//-------------------------------------------------------------------------------------------

AbstractTrackItem::~AbstractTrackItem()
{}

//-------------------------------------------------------------------------------------------

bool AbstractTrackItem::hasParent() const
{
    return (m_parent.data()!=0) ? true : false;
}

//-------------------------------------------------------------------------------------------

AbstractTrackModelSPtr AbstractTrackItem::parent()
{
	return m_parent;
}

//-------------------------------------------------------------------------------------------

db::SQLiteQuerySPtr AbstractTrackItem::getDBQuery() const
{
    db::SQLiteQuerySPtr pDB(new db::SQLiteQuery(db::TrackDB::instance()->db()));
	return pDB;
}

//-------------------------------------------------------------------------------------------
} // namespace model
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
