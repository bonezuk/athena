#include "network/inc/Resource.h"
#if defined(ORCUS_WIN32)
#include "audioio/inc/WasAPIIF.h"
#endif
#include "playerapp/audiodaemon/inc/OmegaAudioDaemon.h"

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

	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::init- a\n", (tuint64)(QThread::currentThreadId()));		
#if defined(ORCUS_WIN32)
	m_audio = audioio::AOBase::get("win32");
#elif defined(OMEGA_MACOSX)
	m_audio = audioio::AOBase::get("coreaudio");
#elif defined(OMEGA_LINUX)
	m_audio = audioio::AOBase::get("alsa");
#endif

	m_timer	= new QTimer(this);
	m_timer->setInterval(500);
	m_timer->setSingleShot(false);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	m_timer->start();

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
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::init- b\n", (tuint64)(QThread::currentThreadId()));		
	return res;
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::quitDaemon()
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::quitDaemon- a\n", (tuint64)(QThread::currentThreadId()));
	if(!m_audio.isNull())
	{
		m_audio->stop();
		m_audio.clear();
	}
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::quitDaemon- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::playFile(const QString& fileName, bool isNext)
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::playFile- a\n", (tuint64)(QThread::currentThreadId()));
	if(common::DiskOps::exist(fileName))
	{
		if(isNext)
		{
			m_audio->next(fileName);
		}
		else
		{
			m_audio->open(fileName);
		}
	}
	else
	{
		QString err = QString("File '%1' does not exist").arg(fileName);
		printError("playFile", err.toUtf8().constData());
	}
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::playFile- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::playFileWithTime(const QString& fileName, const common::TimeStamp& start,const common::TimeStamp& length, bool isNext)
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::playFileWithTime- a\n", (tuint64)(QThread::currentThreadId()));
	if(common::DiskOps::exist(fileName))
	{
		if(isNext)
		{
			m_audio->next(fileName, start, length);
		}
		else
		{
			m_audio->open(fileName, start, length);
		}
	}
	else
	{
		QString err = QString("File '%1' does not exist").arg(fileName);
		printError("playFileWithTime", err.toUtf8().constData());
	}
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::playFileWithTime- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::play()
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::play- a\n", (tuint64)(QThread::currentThreadId()));
	m_audio->play();
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::play- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::pause()
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::pause- a\n", (tuint64)(QThread::currentThreadId()));
	m_audio->pause();
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::pause- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioStart(const QString& name)
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioStart- a\n", (tuint64)(QThread::currentThreadId()));
	m_pPLInterface->onAudioStart(name);
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioStart- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioPlay()
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioPlay- a\n", (tuint64)(QThread::currentThreadId()));
	m_pPLInterface->onAudioPlay();
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioPlay- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioStop()
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioStop- a\n", (tuint64)(QThread::currentThreadId()));
	m_pPLInterface->onAudioStop();
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioStop- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioPause()
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioPause- a\n", (tuint64)(QThread::currentThreadId()));
	m_pPLInterface->onAudioPause();
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioPause- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onTimer()
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onTimer- a\n", (tuint64)(QThread::currentThreadId()));
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onTimer- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioTime(quint64 t)
{
	common::TimeStamp tS(t);
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioTime- a\n", (tuint64)(QThread::currentThreadId()));
	QString str = QString("onAudioTime - %1").arg(static_cast<tfloat64>(tS));
	common::Log::g_Log << str << common::c_endl;
	m_pPLInterface->playbackTime(t);
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioTime- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioBuffer(tfloat32 percent)
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioBuffer- a\n", (tuint64)(QThread::currentThreadId()));
	m_pPLInterface->onAudioBuffer(percent);
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioBuffer- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioReadyForNext()
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioReadyForNext- a\n", (tuint64)(QThread::currentThreadId()));
	m_pPLInterface->onAudioReadyForNext();
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioReadyForNext- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioNoNext()
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioNoNext- a\n", (tuint64)(QThread::currentThreadId()));
	m_pPLInterface->onAudioNoNext();
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioNoNext- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------

void OmegaAudioDaemon::onAudioCrossfade()
{
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioCrossfade- a\n", (tuint64)(QThread::currentThreadId()));
	m_pPLInterface->onAudioCrossfade();
	common::Log::g_Log.print("(%d) - OmegaAudioDaemon::onAudioCrossfade- b\n", (tuint64)(QThread::currentThreadId()));
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
