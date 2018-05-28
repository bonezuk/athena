#include "track/db/test/SQLiteMock.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

SQLiteDatabaseMock::SQLiteDatabaseMock() : SQLiteDatabase()
{}

//-------------------------------------------------------------------------------------------

SQLiteQueryMock::SQLiteQueryMock(SQLiteDatabase *db) : SQLiteQuery(db)
{}

//-------------------------------------------------------------------------------------------

SQLiteInsertMock::SQLiteInsertMock(SQLiteDatabase *db) : SQLiteInsert(db)
{}

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
