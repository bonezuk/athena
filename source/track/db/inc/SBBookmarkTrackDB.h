//-------------------------------------------------------------------------------------------
#ifndef __TRACK_DB_SBBOOKMARKTRACKDB_H
#define __TRACK_DB_SBBOOKMARKTRACKDB_H
//-------------------------------------------------------------------------------------------

#include "common/inc/SBService.h"
#include "track/db/inc/TrackDB.h"

#include <QMutex>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

typedef enum
{
	e_cannotLoadPlaylist = 0,
} UserSandboxErrorMessage;

//-------------------------------------------------------------------------------------------

class TRACK_DB_EXPORT SBBookmarkTrackDB : public common::SBBookmark
{
	public:
		SBBookmarkTrackDB();
		virtual ~SBBookmarkTrackDB();

		virtual bool has(const QString& fileName,bool readOnlyFlag);
		virtual bool has(const QString& docFileName,const QString& refFileName,bool readOnlyFlag);
		
		virtual bool add(const QString& fileName,bool readOnlyFlag);
		virtual bool add(const QString& docFileName,const QStringList& refFileNames,bool readOnlyFlag);

		virtual bool checkOut(const QString& fileName,bool readOnlyFlag);
		virtual bool checkIn(const QString& fileName,bool readOnlyFlag);
};

//-------------------------------------------------------------------------------------------

class TRACK_DB_EXPORT SBBookmarkService
{
	public:
		SBBookmarkService();
		virtual ~SBBookmarkService();

		static QString getHomeDirectory();

		static QSharedPointer<SBBookmarkService> instance();
		static void release();

		virtual bool isMusicDirectory(const QString& fileName) = 0;
		virtual bool isPlaylist(const QString& fileName) = 0;

		virtual bool has(const QString& fileName) = 0;
		virtual bool has(const QString& fileName,bool readOnlyFlag) = 0;
		virtual bool has(const QString& docFileName,const QString& refFileName) = 0;
		virtual bool has(const QString& docFileName,const QString& refFileName,bool readOnlyFlag) = 0;
		
		virtual bool add(const QString& fileName,bool readOnlyFlag) = 0;
		virtual bool add(const QString& docFileName,const QStringList& refFileNames,bool readOnlyFlag) = 0;
		virtual bool add(const QString& docFileName,const QString& refFileName,bool readOnlyFlag,const QByteArray& bkArray) = 0;
		
		virtual bool checkOut(const QString& fileName,bool readOnlyFlag) = 0;
		
		virtual bool checkIn(const QString& fileName,bool readOnlyFlag) = 0;
		
		virtual QByteArray getBookmarkArray(const QString& docFileName,const QString& refFileName) = 0;
		
        virtual QString userErrorMessage(UserSandboxErrorMessage err) = 0;
        
    protected:
    
    	static QSharedPointer<SBBookmarkService> m_instance;
};

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
