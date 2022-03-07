//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERAPP_OMEGAAUDIOIOINTERFACE_H
#define __PLAYERAPP_PLAYERAPP_OMEGAAUDIOIOINTERFACE_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOBase.h"

#include "engine/inc/Codec.h"
#include "track/info/inc/Info.h"
#include "common/inc/DiskOps.h"

#include "playerapp/playercommon/inc/EmbeddedEnv.h"
#include "playerapp/playercommon/inc/OmegaAudioInterface.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaAudioIOInterface : public OmegaAudioInterface
{
	Q_OBJECT
	
	public:
		OmegaAudioIOInterface(QSharedPointer<OmegaPlaylistInterface>& pPLInterface, QObject *parent = 0);
		virtual ~OmegaAudioIOInterface();
		
		virtual void playFile(const QString& fileName, bool isNext);
		virtual void play();
		virtual void pause();

		virtual bool init();
		virtual void quitDaemon();
	
	private:
	
		QSharedPointer<audioio::AOBase> m_audio;
		QSharedPointer<OmegaPlaylistInterface> m_pPLInterface;
		
		virtual void printError(const char *strR, const char *strE) const;
		
	private slots:
				
		void onAudioStart(const QString& name);
		void onAudioPlay();
		void onAudioPause();
		void onAudioStop();
		void onAudioTime(quint64 t);
		void onAudioBuffer(tfloat32 percent);
		void onAudioReadyForNext();
		void onAudioNoNext();
		void onAudioCrossfade();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
