//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_OMEGAPLSERVICE_H
#define __PLAYERAPP_PLAYLISTMANAGER_OMEGAPLSERVICE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCService.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class OmegaPLService : public IPCService
{
	Q_OBJECT
	
	public:
		OmegaPLService(QSharedPointer<OmegaPlaylistInterface>& pPLInterface, QObject *parent = 0);
		virtual ~OmegaPLService();

	private:
		QSharedPointer<OmegaPlaylistInterface> m_pPLInterface;

		virtual void printError(const char *strR, const char *strE) const;
		virtual void handleRPCJson(const QJsonDocument& doc);
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

