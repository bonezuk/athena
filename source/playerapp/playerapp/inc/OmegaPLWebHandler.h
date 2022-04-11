//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERAPP_OMEGAPLWEBHANDLER_H
#define __PLAYERAPP_PLAYERAPP_OMEGAPLWEBHANDLER_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/OmegaPLWebInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaPLWebHandler : public OmegaPLWebInterface
{
	public:
		OmegaPLWebInterface(QSharedPointer<OmegaWebInterface>& pPLWebInterface);
		virtual ~OmegaPLWebInterface();
		
		virtual QJsonDocument getFullPlaylist();
		
		virtual QJsonDocument getPlaybackState();
		
		virtual void onPressPlay();
		
		virtual void onStartPlaying(tuint64 id);
		
	protected:
		QSharedPointer<OmegaWebInterface> m_pPLWebInterface;
	
		virtual void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

