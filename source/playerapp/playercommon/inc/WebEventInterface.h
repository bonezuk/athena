//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_WEBEVENTINTERFACE_H
#define __PLAYERAPP_PLAYERCOMMON_WEBEVENTINTERFACE_H
//-------------------------------------------------------------------------------------------

#include "common/inc/Log.h"
#include "playerapp/playercommon/inc/OmegaWebInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT WebEventInterface
{
	public:
		WebEventInterface();
		virtual ~WebEventInterface();
		
		virtual void onPlaybackTime(const common::TimeStamp& tS);
		
		virtual void onAudioPlaybackEvent(bool isPlaying, tuint64 id);
		virtual void onAudioStart(tuint64 id);
		
	protected:
	
		virtual void printError(const char *strR, const char *strE) const;
		virtual void sendEvent(const QJsonDocument& doc) = 0;
		virtual QJsonDocument createJsonEvent(const QString& eventName, const QJsonObject& data);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
