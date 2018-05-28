//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_ENGINE_MONKEYOMEGA_INTEGRATION_MONKEYPLAYBACK_H
#define __ORCUS_ENGINE_MONKEYOMEGA_INTEGRATION_MONKEYPLAYBACK_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOBase.h"
#include "engine/blackomega/inc/MPCodec.h"
#include "engine/blueomega/inc/WaveEngine.h"
#include "engine/silveromega/inc/SilverCodec.h"
#include "engine/whiteomega/inc/WhiteCodec.h"
#include "engine/greenomega/inc/GreenCodec.h"
#include "engine/redomega/inc/RedCodec.h"
#include "engine/violetomega/inc/VioletCodec.h"
#include "engine/cyanomega/inc/CyanCodec.h"
#include "engine/monkeyomega/inc/MonkeyCodec.h"

#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QThread>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace testspace
{
namespace audio
{
//-------------------------------------------------------------------------------------------

class MonkeyPlayback : public QCoreApplication
{
	public:
		Q_OBJECT
		
	public:
		MonkeyPlayback(int argc,char **argv);
		virtual ~MonkeyPlayback();
	
	protected:
		
		tint m_songIdx;
		QSharedPointer<audioio::AOBase> m_audio;
		common::TimeStamp m_playTime;
		bool m_playZeroFlag;

	protected slots:
	
		void onInit();
		void onStop();

		void onPause();
		void onUnpause();
		void onReset();

		void onSeek();
		
		void onAudioStart(const QString& name);
		void onAudioPlay();
		void onAudioPause();
		void onAudioTime(quint64 t);
		void onAudioBuffer(tfloat32 percent);
		void onAudioReadyForNext();
		void onAudioNoNext();
		void onAudioCrossfade();
};

//-------------------------------------------------------------------------------------------
} // namespace audio
} // namespace testspace
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
