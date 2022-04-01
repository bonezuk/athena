//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERCOMMON_OMEGAAUDIOINTERFACE_H
#define __ORCUS_PLAYERAPP_PLAYERCOMMON_OMEGAAUDIOINTERFACE_H
//-------------------------------------------------------------------------------------------

#include <QString>
#include <QObject>

#include "common/inc/CommonTypes.h"
#include "common/inc/TimeStamp.h"
#include "playerapp/playercommon/inc/PlayerCommonDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT OmegaAudioInterface : public QObject
{
	Q_OBJECT
	public:
		OmegaAudioInterface(QObject *parent = 0);
		
		virtual void playFile(const QString& fileName, bool isNext) = 0;
		virtual void playFileWithTime(const QString& fileName, const common::TimeStamp& start,const common::TimeStamp& length, bool isNext) = 0;
		virtual void play() = 0;
		virtual void pause() = 0;
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
