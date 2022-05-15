#include "audioio/test/CoreAudioMockIF.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(CoreAudioIFFactory,CoreAudioIF, \
                            CoreAudioMockIFFactory,CoreAudioMockIF, \
                            "mock",false)

//-------------------------------------------------------------------------------------------

CoreAudioMockIF::CoreAudioMockIF() : CoreAudioIF()
{}

//-------------------------------------------------------------------------------------------

CoreAudioMockIF::~CoreAudioMockIF()
{}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace omega
//-------------------------------------------------------------------------------------------
