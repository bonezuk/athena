#include "track/db/inc/SBBookmarkTrackDB.h"
#include "common/inc/TimeStamp.h"
#include "track/db/inc/SandboxBookmarkService.h"
#include "track/info/inc/SBBookmarkService.h"

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include <sys/types.h>
#include <pwd.h>

//-------------------------------------------------------------------------------------------
namespace omega
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
    return track::info::SBBookmarkService::instance()->has(fileName,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::has(const QString& docFileName,const QString& refFileName,bool readOnlyFlag)
{
    return track::info::SBBookmarkService::instance()->has(docFileName,refFileName,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::add(const QString& fileName,bool readOnlyFlag)
{
    return track::info::SBBookmarkService::instance()->add(fileName,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::add(const QString& docFileName,const QStringList& refFileNames,bool readOnlyFlag)
{
    return track::info::SBBookmarkService::instance()->add(docFileName,refFileNames,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::checkOut(const QString& fileName,bool readOnlyFlag)
{
    return track::info::SBBookmarkService::instance()->checkOut(fileName,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------

bool SBBookmarkTrackDB::checkIn(const QString& fileName,bool readOnlyFlag)
{
    return track::info::SBBookmarkService::instance()->checkIn(fileName,readOnlyFlag);
}

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace omega
//-------------------------------------------------------------------------------------------
