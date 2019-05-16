//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_DAEMON_CONSOLEPLPROGRESS_H
#define __ORCUS_DAEMON_CONSOLEPLPROGRESS_H
//-------------------------------------------------------------------------------------------

#include "track/db/inc/PlaylistAbstractIO.h"
#include "daemon/inc/DaemonDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace daemon
{
//-------------------------------------------------------------------------------------------

class DAEMON_EXPORT ConsolePLProgress : public track::db::PLProgress
{
	public:
		ConsolePLProgress(const QString& plFileName, QObject *parent = 0);
		virtual ~ConsolePLProgress();
		
		virtual void setProgress(tfloat32 v);
		virtual void done();
		
	private:
		QString m_plFileName;
};

//-------------------------------------------------------------------------------------------
} // namespace daemon
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
