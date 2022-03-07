//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_OMEGAPLSERVICE_H
#define __PLAYERAPP_PLAYLISTMANAGER_OMEGAPLSERVICE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCServiceThread.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaPLService : public QObject
{
	Q_OBJECT
	
	public:
		OmegaPLService(QSharedPointer<OmegaPlaylistInterface>& pPLInterface, QObject *parent = 0);
		virtual ~OmegaPLService();
		
		virtual bool start();
		virtual void stop();
		
	private:
		QSharedPointer<OmegaPlaylistInterface> m_pPLInterface;
		IPCServiceThread *m_pServiceThread;
		
		virtual void printError(const char *strR, const char *strE) const;
		
	public slots:
		void processRPC(QByteArray rpcArray);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

