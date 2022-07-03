//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYLISTMANAGER_OMEGAAUDIOBUSINTERFACE_H
#define __PLAYERAPP_PLAYLISTMANAGER_OMEGAAUDIOBUSINTERFACE_H
//-------------------------------------------------------------------------------------------

#include "playerapp/playercommon/inc/IPCInterfaceBase.h"
#include "playerapp/playercommon/inc/OmegaAudioInterface.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class OmegaAudioBusInterface : public OmegaAudioInterface, public IPCInterfaceBase
{
	Q_OBJECT
	public:
		OmegaAudioBusInterface(QObject *parent = 0);
		virtual ~OmegaAudioBusInterface();
		
		virtual void playFile(const QString& fileName, bool isNext);
		virtual void playFileWithTime(const QString& fileName, const common::TimeStamp& start,const common::TimeStamp& length, bool isNext);
		virtual void play();
		virtual void pause();
		virtual void stop();
		virtual void seek(const common::TimeStamp& t);
		
	private:
		virtual void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
