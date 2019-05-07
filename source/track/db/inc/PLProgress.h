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

		virtual void setProgress(tfloat32 v);
		virtual tfloat32 getProgress();
		virtual bool isCancelled();

		virtual bool isActive();
		virtual void activate(bool useReference = false);
		virtual void deactivate(bool useReference = false);
		
#if defined(OMEGA_MAC_STORE)
		virtual bool getPermissions(const QString& fileName, QList<QPair<QString,QByteArray> >& fileList);
		virtual QByteArray getSandboxBookmark(const QString& outFilename,const QString& trackFilename);
#endif
};

//-------------------------------------------------------------------------------------------
} // namespace db
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

