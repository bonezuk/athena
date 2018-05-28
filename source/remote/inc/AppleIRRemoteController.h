//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_REMOTE_APPLEIRREMOTECONTROLLER_H
#define __ORCUS_REMOTE_APPLEIRREMOTECONTROLLER_H
//-------------------------------------------------------------------------------------------
#if defined(OMEGA_MACOSX)
//-------------------------------------------------------------------------------------------

#include "remote/inc/RemoteIF.h"
#include "remote/inc/AppleRemoteService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace remote
{
//-------------------------------------------------------------------------------------------

class AppleIRRemoteController : public RemoteIF
{
	public:
		AppleIRRemoteController(QObject *parent = 0);
		virtual ~AppleIRRemoteController();
		
		virtual bool start();
		virtual void stop();

        virtual void remoteEvent(RemoteEvent *e);

		static void onTimer(RemoteIF *pRemote);

	protected:
		
		AppleRemoteService::Button m_currentButton;
		bool m_timerEnableFlag;
		
		virtual void onTimerImplementation();

		virtual bool setupTimer();
		virtual void freeTimer();
		virtual void startTimer();
		virtual void stopTimer();
};

//-------------------------------------------------------------------------------------------
} // namespace remote
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
