//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_OMEGAPLWEBSERVICE_H
#define __PLAYERAPP_PLAYLISTMANAGER_OMEGAPLWEBSERVICE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCService.h"
#include "playerapp/playercommon/inc/OmegaWebInterface.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class OmegaPLWebService : public IPCService
{
	Q_OBJECT
	
	public:
		OmegaPLWebService(QSharedPointer<OmegaWebInterface>& pPLWebInterface, QObject *parent = 0);
		virtual ~OmegaPLWebService();
		
	protected:
		QSharedPointer<OmegaWebInterface> m_pPLWebInterface;
		
		virtual void printError(const char *strR, const char *strE) const;
		
		virtual void handleRPCJson(const QJsonDocument& doc);
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

