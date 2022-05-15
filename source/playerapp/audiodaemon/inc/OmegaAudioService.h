//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_AUDIODAEMON_OMEGAAUDIOSERVICE_H
#define __PLAYERAPP_AUDIODAEMON_OMEGAAUDIOSERVICE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCService.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"
#include "playerapp/audiodaemon/inc/OmegaAudioDaemon.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class OmegaAudioService : public IPCService
{
	Q_OBJECT
	public:
		OmegaAudioService(OmegaAudioDaemon *pDaemon, QObject *parent = 0);
		virtual ~OmegaAudioService();
		
	private:
		OmegaAudioDaemon *m_pDaemon;
		
		virtual void printError(const char *strR, const char *strE) const;
		
		virtual void handleRPCJson(const QJsonDocument& doc);
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
