#include "playerapp/playercommon/inc/OmegaPLWebInterface.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaPLWebInterface::OmegaPLWebInterface()
{}

//-------------------------------------------------------------------------------------------

OmegaPLWebInterface::~OmegaPLWebInterface()
{}

//-------------------------------------------------------------------------------------------

void OmegaPLWebInterface::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "OmegaPLWebInterface::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
