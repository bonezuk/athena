//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TRACK_INFO_FILEINFO_H
#define __ORCUS_TRACK_INFO_FILEINFO_H
//-------------------------------------------------------------------------------------------

#include "track/info/inc/Info.h"
#include "track/info/inc/DirInfo.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace info
{
//-------------------------------------------------------------------------------------------

class TRACK_INFO_EXPORT FileInfo : public Info
{
	public:
		FileInfo();
		virtual ~FileInfo();
		
		virtual bool read(common::BIOStream *input);
		
	protected:
	
		virtual void printError(const tchar *strR,const tchar *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace info
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------