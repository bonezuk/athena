//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_MODEL_ABSTRACTTRACKITEM_H
#define __ORCUS_TRACK_MODEL_ABSTRACTTRACKITEM_H
//-------------------------------------------------------------------------------------------

#include <QVector>
#include <QVariant>

#include "track/db/inc/TrackDB.h"
#include "track/model/inc/TrackModelKey.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace model
{
//-------------------------------------------------------------------------------------------

class AbstractTrackItem;
typedef QSharedPointer<AbstractTrackItem> AbstractTrackItemSPtr;

class AbstractTrackModel;
typedef QSharedPointer<AbstractTrackModel> AbstractTrackModelSPtr;

//-------------------------------------------------------------------------------------------

typedef QVector<QVariant> QueryRecord;
typedef QVector<QueryRecord> QueryResult;

//-------------------------------------------------------------------------------------------

class TRACK_MODEL_EXPORT AbstractTrackItem
{
	public:
		AbstractTrackItem();
		AbstractTrackItem(const AbstractTrackModelSPtr& parentItem);
		virtual ~AbstractTrackItem();
		
		virtual bool hasParent() const;
		virtual AbstractTrackModelSPtr parent();		
		
	protected:	
	
		AbstractTrackModelSPtr m_parent;
		
		virtual db::SQLiteQuerySPtr getDBQuery() const;
};

//-------------------------------------------------------------------------------------------
} // namespace model
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

