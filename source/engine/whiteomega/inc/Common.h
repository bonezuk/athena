//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_WHITEOMEGA_COMMON_H
#define __ORCUS_WHITEOMEGA_COMMON_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"
#include "engine/whiteomega/inc/ICS.h"
#include "engine/whiteomega/inc/WhiteOmegaDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace engine
{
namespace whiteomega
{
//-------------------------------------------------------------------------------------------

tint maxPredSfb(tint idx);

//-------------------------------------------------------------------------------------------

inline tint minV(tint a,tint b)
{
	return (a < b) ? a : b;
}


//-------------------------------------------------------------------------------------------

inline tint maxV(tint a,tint b)
{
	return (a > b) ? a : b;
}

//-------------------------------------------------------------------------------------------

inline tint bitSet(tint a,tint b)
{
	return static_cast<tint>(static_cast<tuint>(a) & (1 << b));
}

//-------------------------------------------------------------------------------------------
} // namespace whiteomega
} // namespace engine
} // namespace whiteomega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------