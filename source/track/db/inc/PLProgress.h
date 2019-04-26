//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_DB_PLPROGRESS_H
#define __ORCUS_TRACK_DB_PLPROGRESS_H
//-------------------------------------------------------------------------------------------

#include "track/db/inc/TrackDB.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace db
{
//-------------------------------------------------------------------------------------------

class TRACK_DB_EXPORT PLProgress : public QObject
{
	public:
		Q_OBJECT
		
	public:
		PLProgress(QObject *parent = 0);
		virtual ~PLProgress();

		virtual void setProgress(tfloat32 v) = 0;
		virtual tfloat32 getProgress() = 0;
		virtual bool isCancelled() = 0;

		virtual bool isActive() = 0;
		virtual void activate(bool useReference = false) = 0;
		virtual void deactivate(bool useReference = false) = 0;
		
#if defined(OMEGA_MAC_STORE)
		virtual bool getPermissions(const QString& fileName, QList<QPair<QString,QByteArray> >& fileList) = 0;
		virtual QByteArray getSandboxBookmark(const QString& outFilename,const QString& trackFilename) = 0;
#endif
};

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

