//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_OMEGAPLWEBINTERFACE_H
#define __PLAYERAPP_PLAYERCOMMON_OMEGAPLWEBINTERFACE_H
//-------------------------------------------------------------------------------------------

#include <QJsonDocument>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class OmegaPLWebInterface
{
	public:
		OmegaPLWebInterface();
		virtual ~OmegaPLWebInterface();
		
		// { "function": "getFullPlaylist" }
		virtual QJsonDocument getFullPlaylist() = 0;
		
		// { "function": "getPlaybackState" }
		virtual QJsonDocument getPlaybackState() = 0;
		
		// { "function": "onPressPlay" }
		virtual void onPressPlay() = 0;
		
		// { "function": "onStartPlaying", "id": 12345 }
		virtual void onStartPlaying(tuint64 id) = 0;
		
	protected:
	
		virtual void printError(const char *strR, const char *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
