//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_DLNA_UPNPIF_H
#define __OMEGA_DLNA_UPNPIF_H
//-------------------------------------------------------------------------------------------

#include "upnp/upnp.h"

#include "common/inc/Factory.h"
#include "dlna/inc/DLNADLL.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

class DLNA_EXPORT UPnPIF
{
	public:
		UPnPIF();
		virtual ~UPnPIF();
		
		static QSharedPointer<UPnPIF> instance();
		static QSharedPointer<UPnPIF> instance(const char *factoryName);
		
		static void release();
		
		virtual int UpnpInit(const tchar *hostIP,tuint16 port) = 0;
		virtual int UpnpFinish() = 0;
		
	protected:
	
		static QSharedPointer<UPnPIF> m_instance;
};

typedef QSharedPointer<UPnPIF> UPnPIFSPtr;

//-------------------------------------------------------------------------------------------

ABSTRACT_FACTORY_CLASS(DLNA_EXPORT,UPnPIFFactory,UPnPIF)

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

