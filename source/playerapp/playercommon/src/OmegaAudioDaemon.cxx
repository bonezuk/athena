#include "network/inc/Resource.h"
#if defined(ORCUS_WIN32)
#include "audioio/inc/WasAPIIF.h"
#endif
#include "playerapp/playercommon/inc/OmegaAudioDaemon.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaAudioDaemon::OmegaAudioDaemon(OmegaPlaylistInterface *pPLInterface, QObject *parent) : QObject(parent),
	m_audio(),
	m_pPLInterface(pPLInterface)
{}

//-------------------------------------------------------------------------------------------

OmegaAudioDaemon::~OmegaAudioDaemon()
{}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaAudioDaemon::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool OmegaAudioDaemon::init()
{
	bool res = false;
	
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
		connect(m_audio.data(),SIGNAL(onStop()),this,SLOT(onAudioStop()));
		connect(m_audio.data(),SIGNAL(onPlay()),this,SLOT(onAudioPlay()));
		connect(m_audio.data(),SIGNAL(onPause()),this,SLOT(onAudioPause()));
		connect(m_audio.data(),SIGNAL(onTime(quint64)),this,SLOT(onAudioTime(quint64)));
		connect(m_audio.data(),SIGNAL(onBuffer(tfloat32)),this,SLOT(onAudioBuffer(tfloat32)));
		connect(m_audio.data(),SIGNAL(onReadyForNext()),this,SLOT(onAudioReadyForNext()));
		connect(m_audio.data(),SIGNAL(onNoNext()),this,SLOT(onAudioNoNext()));
		connect(m_audio.data(),SIGNAL(onCrossfade()),this,SLOT(onAudioCrossfade()));
		common::Log::g_Log << "Audio Daemon running..."<< common::c_endl;
		
		res = true;
	}
	else
	{
		printError("OmegaAudioDaemon", "Failed to start audio engine");
		quitDaemon();
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::quitDaemon()
{
	common::Log::g_Log << "quitDaemon" << common::c_endl;
	if(!m_audio.isNull())
	{
		m_audio->stop();
		m_audio.clear();
	}
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::playFile(const QString& fileName)
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

void OmegaAudioDaemon::onAudioStop()
{
	common::Log::g_Log << "onAudioStop" << common::c_endl;
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
	m_pPLInterface->playbackTime(t);
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
} // namespace orcus
//-------------------------------------------------------------------------------------------
