#include "dlna/test/XMLLibMockIF.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(XMLLibIFFactory,XMLLibIF, \
                            XMLLibIFMockFactory,XMLLibMockIF, \
                            "mock",false)

//-------------------------------------------------------------------------------------------

XMLLibMockIF::XMLLibMockIF()
{}

//-------------------------------------------------------------------------------------------

XMLLibMockIF::~XMLLibMockIF()
{}

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace orcus
//-------------------------------------------------------------------------------------------