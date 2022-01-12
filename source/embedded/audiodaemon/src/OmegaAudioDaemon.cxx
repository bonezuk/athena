#include "embedded/audiodaemon/inc/OmegaAudioDaemon.h"

#include <QtDBus/QtDBus>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaAudioDaemon::OmegaAudioDaemon(int argc, char **argv, OmegaAudioDBusAdaptor *pInterface) : QCoreApplication(argc, argv),
	m_audio(),
	m_pInterface(pInterface)
{
	QTimer::singleShot(100, this, SLOT(onInit()));
	QObject::connect(this, SIGNAL(aboutToQuit()), this, SLOT(onQuitDaemon()));
	m_pInterface->setAudioDaemon(this);
}

//-------------------------------------------------------------------------------------------

OmegaAudioDaemon::~OmegaAudioDaemon()
{}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaAudioDaemon::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onInit()
{
#if defined(ORCUS_WIN32)
	m_audio = audioio::AOBase::get("win32");
#elif defined(OMEGA_MACOSX)
	m_audio = audioio::AOBase::get("coreaudio");
#elif defined(OMEGA_LINUX)
	m_audio = audioio::AOBase::get("alsa");
#endif

	if(!m_audio.isNull())
	{
		connect(m_audio.data(),SIGNAL(onStart(const QString&)),this,SLOT(onAudioStart(const QString&)));
		connect(m_audio.data(),SIGNAL(onStop()),this,SLOT(onStop()));
		connect(m_audio.data(),SIGNAL(onPlay()),this,SLOT(onAudioPlay()));
		connect(m_audio.data(),SIGNAL(onPause()),this,SLOT(onAudioPause()));
		connect(m_audio.data(),SIGNAL(onTime(quint64)),this,SLOT(onAudioTime(quint64)));
		connect(m_audio.data(),SIGNAL(onBuffer(tfloat32)),this,SLOT(onAudioBuffer(tfloat32)));
		connect(m_audio.data(),SIGNAL(onReadyForNext()),this,SLOT(onAudioReadyForNext()));
		connect(m_audio.data(),SIGNAL(onNoNext()),this,SLOT(onAudioNoNext()));
		connect(m_audio.data(),SIGNAL(onCrossfade()),this,SLOT(onAudioCrossfade()));
	
	}
	else
	{
		printError("OmegaAudioDaemon", "Failed to start audio engine");
		shutdownDaemon();
	}
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::shutdownDaemon()
{
	onQuitDaemon();
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onQuitDaemon()
{
	if(m_pInterface != 0)
	{
		m_pInterface->setAudioDaemon(0);
		m_pInterface = 0;
	}
	if(!m_audio.isNull())
	{
		m_audio->stop();
		m_audio.clear();
	}
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::playFileDBUS(const QString& fileName)
{
	if(common::DiskOps::exist(fileName))
	{
		m_audio->open(fileName);
	}
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioStart(const QString& name)
{
	common::Log::g_Log << "onAudioStart - " << name << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioPlay()
{
	common::Log::g_Log << "onAudioPlay" << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioPause()
{
	common::Log::g_Log << "onAudioPause" << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioTime(quint64 t)
{
	common::TimeStamp tS(t);
	common::Log::g_Log.print("onAudioTime - %.2f\n", static_cast<tfloat64>(tS));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioBuffer(tfloat32 percent)
{
	common::Log::g_Log << "onAudioBuffer" << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioReadyForNext()
{
	common::Log::g_Log << "onAudioReadyForNext" << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioNoNext()
{
	common::Log::g_Log << "onAudioNoNext" << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioCrossfade()
{
	common::Log::g_Log << "onAudioCrossfade" << common::c_endl;
}


//-------------------------------------------------------------------------------------------
// OmegaAudioDBusAdaptor
//-------------------------------------------------------------------------------------------

OmegaAudioDBusAdaptor::OmegaAudioDBusAdaptor(QObject *obj) : QDBusAbstractAdaptor(obj),
	m_pDaemon(0)
{}

//-------------------------------------------------------------------------------------------

OmegaAudioDBusAdaptor::~OmegaAudioDBusAdaptor()
{}

//-------------------------------------------------------------------------------------------

void OmegaAudioDBusAdaptor::setAudioDaemon(OmegaAudioDaemon *pDaemon)
{
	m_pDaemon = 0;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDBusAdaptor::playFile(const QString& fileName)
{
	if(m_pDaemon != 0)
	{
		m_pDaemon->playFileDBUS(fileName);
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------

static orcus::OmegaAudioDaemon *g_audioDaemonApp = 0;

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

	QObject obj;
	orcus::OmegaAudioDBusAdaptor *pIface = new orcus::OmegaAudioDBusAdaptor(&obj);
	QDBusConnection::sessionBus().registerObject("/", &obj);
	
	if(QDBusConnection::sessionBus().registerService(OMEGAAUDIODAEMON_SERVICE_NAME))
	{
		g_audioDaemonApp = new orcus::OmegaAudioDaemon(argc, argv, pIface);
		res = g_audioDaemonApp->exec();
		delete g_audioDaemonApp;
		g_audioDaemonApp = 0;
	}
	else
	{
		fprintf(stdout, "::main - %s\n", qPrintable(QDBusConnection::sessionBus().lastError().message()));
		res = -1;
	}

    orcus::releaseCodecs();
#if defined(ORCUS_WIN32)
	orcus::audioio::WasAPIIF::release();
#endif

    return res;
}

//-------------------------------------------------------------------------------------------
