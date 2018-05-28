//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_AUDIOIO_AUDIOINFO_H
#define __ORCUS_AUDIOIO_AUDIOINFO_H
//-------------------------------------------------------------------------------------------

#include "engine/inc/RData.h"
#include "audioio/inc/AudioIODLL.h"
#include <QMap>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT AudioInfo
{
	public:
		~AudioInfo();
		
		static AudioInfo& instance();
		
		static void start();
		static void stop();
		
		QString error(tint r) const;
		
	protected:
	
		static AudioInfo *m_instance;
		
		QMap<tint,QString> m_errorMap;
		
		AudioInfo();
};

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

