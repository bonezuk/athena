#include "track/model/test/ImageRepositaryMock.h"

//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(ImageRepositaryFactory,ImageRepositary, \
                            ImageRepositaryMockFactory,ImageRepositaryMock, \
                            "mock",false)

//-------------------------------------------------------------------------------------------
