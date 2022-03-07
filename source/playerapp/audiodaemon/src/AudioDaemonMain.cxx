#include "network/inc/Resource.h"
#if defined(ORCUS_WIN32)
#include "audioio/inc/WasAPIIF.h"
#endif
#include "playerapp/audiodaemon/inc/AudioDaemonMain.h"
#include "playerapp/audiodaemon/inc/OmegaAudioService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

AudioDaemonMain::AudioDaemonMain(int argc, char **argv) : QCoreApplication(argc, argv),
	m_pAudio(0),
	m_pPLInterface(0),
	m_pAudioIPC(0)
{
	QTimer::singleShot(100, this, SLOT(onInit()));
	QObject::connect(this, SIGNAL(aboutToQuit()), this, SLOT(onQuitDaemon()));
}

//-------------------------------------------------------------------------------------------

AudioDaemonMain::~AudioDaemonMain()
{
	AudioDaemonMain::shutdownDaemon();
}

//-------------------------------------------------------------------------------------------

void AudioDaemonMain::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "AudioDaemonMain::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void AudioDaemonMain::onInit()
{
	bool res = false;
	
	m_pPLInterface = new OmegaPLBusInterface(this);
	
	m_pAudio = new OmegaAudioDaemon(m_pPLInterface, this);
	if(m_pAudio->init())
	{
		if(initAudioIPC())
		{		
			res = true;
		}
	}
	if(!res)
	{
		quit();
	}
}

//-------------------------------------------------------------------------------------------

void AudioDaemonMain::onQuitDaemon()
{
	shutdownDaemon();
}

//-------------------------------------------------------------------------------------------

void AudioDaemonMain::shutdownDaemon()
{
	if(m_pAudioIPC != 0)
	{
		m_pAudioIPC->stop();
		delete m_pAudioIPC;
		m_pAudioIPC = 0;
	}
	if(m_pAudio != 0)
	{
		m_pAudio->quitDaemon();
		delete m_pAudio;
		m_pAudio = 0;
	}
	if(m_pPLInterface != 0)
	{
		delete m_pPLInterface;
		m_pPLInterface = 0;
	}
}

//-------------------------------------------------------------------------------------------

bool AudioDaemonMain::initAudioIPC()
{
	bool res = false;
	
	m_pAudioIPC = new OmegaAudioService(m_pAudio, this);
	if(m_pAudioIPC->start())
	{
		res = true;
	}
	else
	{
		printError("initAudioIPC", "Failed to start IPC service");
	}
	return res;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------

static orcus::AudioDaemonMain *g_audioDaemonApp = 0;

//-------------------------------------------------------------------------------------------

#if defined(OMEGA_WIN32)
BOOL WINAPI termSignalHandler(DWORD)
#else
void termSignalHandler(int)
#endif
{
	if(g_audioDaemonApp != 0)
	{
		g_audioDaemonApp->shutdownDaemon();
	}
#if defined(OMEGA_WIN32)
	return TRUE;
#endif
}

//-------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	int res;
	
	orcus::setupEnviroment(argv[0]);
	orcus::initCodecs();

#if defined(OMEGA_WIN32)
	::SetConsoleCtrlHandler(termSignalHandler,TRUE);
	orcus::audioio::WasAPIIF::instance("wasapi");
#else
	signal(SIGTERM, termSignalHandler);
#endif

	g_audioDaemonApp = new orcus::AudioDaemonMain(argc, argv);
	res = g_audioDaemonApp->exec();
	delete g_audioDaemonApp;
	g_audioDaemonApp = 0;

    orcus::releaseCodecs();
#if defined(ORCUS_WIN32)
	orcus::audioio::WasAPIIF::release();
#endif

    return res;
}

//-------------------------------------------------------------------------------------------
