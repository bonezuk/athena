#include "track/db/inc/TrackDBMountPoints.h"
#include "track/db/inc/TrackDB.h"

#include <QDir>

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

TrackDBMountPoints::TrackDBMountPoints(SQLiteDatabase *db) : m_db(db)
{}

//-------------------------------------------------------------------------------------------

TrackDBMountPoints::~TrackDBMountPoints()
{}

//-------------------------------------------------------------------------------------------

void TrackDBMountPoints::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "TrackDBMountPoints::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool TrackDBMountPoints::addMountPoint(const QString& mountPointDir)
{
	bool res = false;
	QDir mountDir(mountPointDir);
	
	if(mountDir.exists())
	{
		QString mountName = mountDir.absolutePath();
		QStringList existMounts = mountPoints();
		
		if(!existMounts.contains(mountName))
		{
			try
			{
				m_db->exec("SAVEPOINT addmountpoint");
				
				{
					QString cmd;
					SQLiteInsert mountI(m_db);
					cmd = "INSERT INTO mountpoints (mountName) VALUES (?)";
					mountI.prepare(cmd);
					mountI.bind(mountName);
					if(mountI.next())
					{
						res = true;
					}
					else
					{
						printError("addMountPoint", "Failed to insert mount point into database");
					}
				}
			
				if(res)
				{
					m_db->exec("RELEASE addmountpoint");
				}
				else
				{
					m_db->exec("ROLLBACK TO SAVEPOINT addmountpoint");
				}
			}
			catch(const SQLiteException& e)
			{
				QString errStr;
				m_db->exec("ROLLBACK TO SAVEPOINT addmountpoint");
				errStr = "Database exception thrown. " + e.error();
				printError("addMountPoint",errStr.toUtf8().constData());
				res = false;
			}		
		}
		else
		{
			res = true;
		}
	}
	else
	{
		common::Log::g_Log << "Cannot create mount point '" << mountPointDir << "' as directory does not exist." << common::c_endl;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

QStringList TrackDBMountPoints::mountPoints()
{
	QStringList points;
	QString cmdQ, mountPointName;
	SQLiteQuery mountQ(m_db);
	
	cmdQ = "SELECT m.mountName FROM mountpoints AS m";
	mountQ.prepare(cmdQ);
	mountQ.bind(mountPointName);
	while(mountQ.next())
	{
		points.append(mountPointName);
	}
	return points;
}

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace omega
//-------------------------------------------------------------------------------------------
