//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_LICENSE_LICENSEDB_H
#define __ORCUS_LICENSE_LICENSEDB_H
//-------------------------------------------------------------------------------------------

#include <QDir>
#include <QCoreApplication>

#include "common/inc/DiskOps.h"
#include "track/db/inc/SQLiteDatabase.h"
#include "track/db/inc/SQLiteInsert.h"
#include "track/db/inc/SQLiteQuery.h"
#include "track/db/inc/TrackDB.h"
#include "licenser/licenselib/inc/LicenseLibDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace license
{
//-------------------------------------------------------------------------------------------

class LICENSER_EXPORT LicenseDB
{
	public:
		virtual ~LicenseDB();
		
		static LicenseDB *instance();
		static LicenseDB *instance(const QString& dbFilename);
		
		orcus::track::db::SQLiteDatabase *db();
		
		int idOfInserted();

	protected:
		
		static LicenseDB *m_instance;
		
		orcus::track::db::SQLiteDatabase *m_db;
		
		LicenseDB();
		
		bool init(const QString& dbFileName);
		void shutdown();

		bool isTableDefined(const QString& tableName);
		
		bool createSchema();
		void createLicenseTable();
		void createProgramTable();
		void createHardwareTable();
};

//-------------------------------------------------------------------------------------------

inline orcus::track::db::SQLiteDatabase *LicenseDB::db()
{
	return m_db;
}

//-------------------------------------------------------------------------------------------
} // namespace license
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
