//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_AUDIODAEMON_AUDIODAEMONMAIN_H
#define __ORCUS_PLAYERAPP_AUDIODAEMON_AUDIODAEMONMAIN_H
//-------------------------------------------------------------------------------------------

#include "playerapp/audiodaemon/inc/OmegaAudioDaemon.h"
#include "playerapp/audiodaemon/inc/OmegaPLBusInterface.h"
#include "playerapp/audiodaemon/inc/OmegaAudioService.h"

#include <QCoreApplication>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class AudioDaemonMain : public QCoreApplication
{
	Q_OBJECT
	
	public:
		AudioDaemonMain(int argc, char **argv);
		virtual ~AudioDaemonMain();
		
		virtual void shutdownDaemon();
		
	private:
		OmegaAudioDaemon *m_pAudio;
		OmegaPLBusInterface *m_pPLInterface;
		OmegaAudioService *m_pAudioIPC;
		
		virtual void printError(const char *strR, const char *strE) const;
		
		virtual bool initAudioIPC();
		
	private slots:
		void onInit();
		void onQuitDaemon();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

