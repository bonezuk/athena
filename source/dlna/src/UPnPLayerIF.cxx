#include "dlna/inc/UPnPLayerIF.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(UPnPIFFactory,UPnPIF, \
                            UPnPLayerIFFactory,UPnPLayerIF, \
                            "upnp",false)

//-------------------------------------------------------------------------------------------

UPnPLayerIF::UPnPLayerIF() : UPnPIF()
{}

//-------------------------------------------------------------------------------------------

UPnPLayerIF::~UPnPLayerIF()
{}

//-------------------------------------------------------------------------------------------

int UPnPLayerIF::UpnpInit(const tchar *hostIP,tuint16 port)
{
//#if defined(OMEGA_MSVC16)
	return ::UpnpInit2(hostIP,port);
/*
#else
	return ::UpnpInit(hostIP, port);
#endif
*/
}

//-------------------------------------------------------------------------------------------

int UPnPLayerIF::UpnpFinish()
{
	return ::UpnpFinish();
}

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace omega
//-------------------------------------------------------------------------------------------
