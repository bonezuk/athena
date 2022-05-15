//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_AUDIODAEMON_OMEGAPLDBUSINTERFACE_H
#define __OMEGA_PLAYERAPP_AUDIODAEMON_OMEGAPLDBUSINTERFACE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCInterfaceBase.h"

#include <QSharedPointer>

#include "common/inc/Log.h"
#include "playerapp/playercommon/inc/OmegaPlaylistInterface.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class OmegaPLBusInterface : public OmegaPlaylistInterface, public IPCInterfaceBase
{
	Q_OBJECT
		
	public:
		OmegaPLBusInterface(QObject *parent = 0);
		virtual ~OmegaPLBusInterface();
		
		virtual void playbackTime(quint64 tS);
		virtual void onAudioStart(const QString& name);

		virtual void onAudioPlay();
		virtual void onAudioPause();
		virtual void onAudioStop();
		virtual void onAudioBuffer(tfloat32 percent);
		virtual void onAudioReadyForNext();
		virtual void onAudioNoNext();
		virtual void onAudioCrossfade();
		
	private:
		virtual void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
