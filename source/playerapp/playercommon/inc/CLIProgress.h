//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_PLAYERAPP_PLAYERCOMMON_CLIPROGRESS_H
#define __ORCUS_PLAYERAPP_PLAYERCOMMON_CLIPROGRESS_H
//-------------------------------------------------------------------------------------------

#include "common/inc/AbstractProgressInterface.h"
#include "playerapp/playercommon/inc/PlayerCommonDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT CLIProgress : public common::AbstractProgressInterface
{
	public:
		CLIProgress();
		virtual ~CLIProgress();
		
		virtual void setProgress(tfloat32 v);
		virtual tfloat32 getProgress();
		virtual bool isCancelled();		
		
		virtual bool isActive();
		virtual void activate(bool useReference = false);
		virtual void deactivate(bool useReference = false);
		
	private:
		tfloat32 m_progress;
		
		void printProgressToConsole();
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

