//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_AUDIODAEMON_OMEGAAUDIOSERVICE_H
#define __PLAYERAPP_AUDIODAEMON_OMEGAAUDIOSERVICE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCServiceThread.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"
#include "playerapp/audiodaemon/inc/OmegaAudioDaemon.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaAudioService : public QObject
{
	Q_OBJECT
	public:
		OmegaAudioService(OmegaAudioDaemon *pDaemon, QObject *parent = 0);
		virtual ~OmegaAudioService();
		
		virtual bool start();
		virtual void stop();
		
	private:
		OmegaAudioDaemon *m_pDaemon;
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
