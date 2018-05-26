//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_COMMON_TMEMORYOBJECTS_H
#define __ORCUS_COMMON_TMEMORYOBJECTS_H
//-------------------------------------------------------------------------------------------

#include "common/inc/BString.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace common
{
namespace memory
{
//-------------------------------------------------------------------------------------------

COMMON_EXPORT tint32 intFromMemory(const tchar *mem);
COMMON_EXPORT void intToMemory(tint32 x,tchar *mem);

COMMON_EXPORT tint16 shortFromMemory(const tchar *mem);
COMMON_EXPORT void shortToMemory(tint16 x,tchar *mem);

COMMON_EXPORT tint stringFromMemory(BString& str,const tchar *mem,tint total);
COMMON_EXPORT void stringToMemory(const BString& str,tchar *mem,tint len);

//-------------------------------------------------------------------------------------------
} // namespace memory
} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

