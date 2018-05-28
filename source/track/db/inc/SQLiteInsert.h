//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_DB_SQLITEINSERT_H
#define __ORCUS_TRACK_DB_SQLITEINSERT_H
//-------------------------------------------------------------------------------------------

#include "track/db/inc/SQLiteDatabase.h"
#include "track/db/inc/SQLiteOperation.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

class TRACK_DB_EXPORT SQLiteInsert : public SQLiteOperation
{
	public:
		SQLiteInsert(SQLiteDatabase *db);
		virtual ~SQLiteInsert();
		
		virtual bool next();
};

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

