#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "engine/monkeyomega/test/integration/MonkeyPlayback.h"

#include <stdio.h>
#include <stdlib.h>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace testspace
{
namespace audio
{
//-------------------------------------------------------------------------------------------

MonkeyPlayback::MonkeyPlayback(int argc,char **argv) : QCoreApplication(argc,argv),
	m_songIdx(0),
	m_audio()
{
	QTimer::singleShot(100,this,SLOT(onInit()));
	QObject::connect(this,SIGNAL(aboutToQuit()),this,SLOT(onStop()));
}

//-------------------------------------------------------------------------------------------

MonkeyPlayback::~MonkeyPlayback()
{
	onStop();
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onInit()
{
	QString fileName;

	m_songIdx = 0;

#if defined(ORCUS_WIN32)
	m_audio = audioio::AOBase::get("win32");
	fileName = "C:\\Encode\\Rip\\hobbit\\quest.ape";
#elif defined(OMEGA_MACOSX)
	m_audio = audioio::AOBase::get("coreaudio");
	fileName = "/Users/bonez/VMWare/Musepack/encode/quest.ape";
#endif

	m_audio->disableLicenseCheck();
	m_audio->setCrossfade(0.0);
    m_audio->open(fileName);

	connect(m_audio.data(),SIGNAL(onStop()),this,SLOT(onStop()));
	
	connect(m_audio.data(),SIGNAL(onStart(const QString&)),this,SLOT(onAudioStart(const QString&)));
	connect(m_audio.data(),SIGNAL(onPlay()),this,SLOT(onAudioPlay()));
	connect(m_audio.data(),SIGNAL(onPause()),this,SLOT(onAudioPause()));
	connect(m_audio.data(),SIGNAL(onTime(quint64)),this,SLOT(onAudioTime(quint64)));
	connect(m_audio.data(),SIGNAL(onBuffer(tfloat32)),this,SLOT(onAudioBuffer(tfloat32)));
	connect(m_audio.data(),SIGNAL(onReadyForNext()),this,SLOT(onAudioReadyForNext()));
	connect(m_audio.data(),SIGNAL(onNoNext()),this,SLOT(onAudioNoNext()));
	connect(m_audio.data(),SIGNAL(onCrossfade()),this,SLOT(onAudioCrossfade()));
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onReset()
{
	common::Log::g_Log.print("onReset\n");
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onSeek()
{
	common::Log::g_Log.print("onSeek\n");

	if(m_audio.data()!=0)
	{
		common::TimeStamp tS(93.0);
		m_audio->seek(tS);
	}
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onStop()
{
	common::Log::g_Log.print("onStop\n");
	
	if(m_audio.data()!=0)
	{
		m_audio.clear();
		quit();
	}
}


//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onPause()
{
	if(m_audio.data()!=0)
	{
		m_audio->pause();
	}
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onUnpause()
{
	if(m_audio.data()!=0)
	{
		m_audio->play();
	}
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onAudioStart(const QString& name)
{
	common::Log::g_Log.print("onStart - %s\n",name.toLatin1().constData());
	m_playTime = 0;
	m_playZeroFlag = false;
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onAudioPlay()
{
	common::Log::g_Log.print("onPlay\n");
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onAudioPause()
{
	common::Log::g_Log.print("onPause\n");
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onAudioTime(quint64 t)
{
	common::TimeStamp cT(t);

	if(cT.secondsTotal()!=m_playTime.secondsTotal() || (cT==0 && !m_playZeroFlag))
	{
		QString newT;
		tint min,sec;

		sec = cT.secondsTotal();
		min = sec / 60;
		sec = sec % 60;
		newT  = common::BString::Int(min).GetString();
		newT += ":";
		newT += common::BString::Int(sec,2,true).GetString();

		common::Log::g_Log.print("time - %s\n",newT.toLatin1().constData());
		if(cT==0)
		{
			m_playZeroFlag = true;
		}
	}
	m_playTime = cT;
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onAudioBuffer(tfloat32 percent)
{
	common::Log::g_Log.print("onBuffer - %.8f\n",percent);
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onAudioReadyForNext()
{
	common::Log::g_Log.print("onReadyForNext\n");
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onAudioNoNext()
{
	common::Log::g_Log.print("onNoNext\n");
}

//-------------------------------------------------------------------------------------------

void MonkeyPlayback::onAudioCrossfade()
{
	common::Log::g_Log.print("onCrossfade\n");
}

//-------------------------------------------------------------------------------------------
} // namespace audio
} // namespace testspace
} // namespace orcus
//-------------------------------------------------------------------------------------------

orcus::testspace::audio::MonkeyPlayback *g_coreMonkeyPlayback = 0;

//-------------------------------------------------------------------------------------------
#if defined(OMEGA_WIN32)
//-------------------------------------------------------------------------------------------

BOOL WINAPI ctrlMonkeyHandler(DWORD dwCtrlType)
{
	if(g_coreMonkeyPlayback!=0)
	{
		g_coreMonkeyPlayback->quit();
	}
	return TRUE;
}

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

TEST(MonkeyPlayback,play)
{
#if defined(OMEGA_WIN32)
	::SetConsoleCtrlHandler(ctrlMonkeyHandler,TRUE);
#endif

	{
		int argc = 0;
		char **argv = 0;
		g_coreMonkeyPlayback = new orcus::testspace::audio::MonkeyPlayback(argc,argv);
		g_coreMonkeyPlayback->exec();
		g_coreMonkeyPlayback = 0;
	}
}

//-------------------------------------------------------------------------------------------
