#include "track/db/inc/SBBookmarkTrackDB.h"
#include "common/inc/TimeStamp.h"
#include "track/db/inc/SandboxBookmarkService.h"

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include <sys/types.h>
#include <pwd.h>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------
// SBBookmarkTrackDB
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(common::SBBookmarkFactory,common::SBBookmark, \
                            SBBookmarkTrackDBFactory,SBBookmarkTrackDB, \
                            "trackdb",false)

//-------------------------------------------------------------------------------------------

SBBookmarkTrackDB::SBBookmarkTrackDB()
{}

//-------------------------------------------------------------------------------------------

SBBookmarkTrackDB::~SBBookmarkTrackDB()
{}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::has(const QString& fileName,bool readOnlyFlag)
{
    return SBBookmarkService::instance()->has(fileName,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::has(const QString& docFileName,const QString& refFileName,bool readOnlyFlag)
{
    return SBBookmarkService::instance()->has(docFileName,refFileName,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::add(const QString& fileName,bool readOnlyFlag)
{
    return SBBookmarkService::instance()->add(fileName,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::add(const QString& docFileName,const QStringList& refFileNames,bool readOnlyFlag)
{
    return SBBookmarkService::instance()->add(docFileName,refFileNames,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::checkOut(const QString& fileName,bool readOnlyFlag)
{
    return SBBookmarkService::instance()->checkOut(fileName,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::checkIn(const QString& fileName,bool readOnlyFlag)
{
    return SBBookmarkService::instance()->checkIn(fileName,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------
// SBBookmarkService
//-------------------------------------------------------------------------------------------

QSharedPointer<SBBookmarkService> SBBookmarkService::m_instance;

//-------------------------------------------------------------------------------------------

SBBookmarkService::SBBookmarkService()
{}

//-------------------------------------------------------------------------------------------

SBBookmarkService::~SBBookmarkService()
{}

//-------------------------------------------------------------------------------------------

QSharedPointer<SBBookmarkService> SBBookmarkService::instance()
{
	if(m_instance.data()==0)
	{
		QSharedPointer<SBBookmarkService> pInstance(reinterpret_cast<SBBookmarkService *>(new SandboxBookmarkService()));
		m_instance = pInstance;
	}
	return m_instance;
}

//-------------------------------------------------------------------------------------------

void SBBookmarkService::release()
{
	QSharedPointer<SBBookmarkService> pEmptyInstance;
	m_instance = pEmptyInstance;
}

//-------------------------------------------------------------------------------------------

QString SBBookmarkService::getHomeDirectory()
{
	QString hDir;
	NSString *homeDir;
	const struct passwd *passwd = getpwnam([NSUserName() UTF8String]);

	if(passwd!=0)
	{
		const char *homeDirC = getpwnam([NSUserName() UTF8String])->pw_dir;
        homeDir = [[NSFileManager defaultManager] stringWithFileSystemRepresentation:homeDirC length:strlen(homeDirC)];
        if(homeDir!=nil)
        {
        	hDir = QString::fromUtf8([homeDir UTF8String]);
        }
	}
	return hDir;
}

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
