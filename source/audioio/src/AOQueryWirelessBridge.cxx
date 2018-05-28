//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_AUDIOIO_AOQUERYWIRELESSBRIDGE_H
#define __ORCUS_AUDIOIO_AOQUERYWIRELESSBRIDGE_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOQueryDevice.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT AOQueryWirelessBridge : public AOQueryDevice
{
	public:
		AOQueryWirelessBridge();
		virtual ~AOQueryWirelessBridge();
		
		virtual bool queryNames();
		virtual bool queryDevice(int idx);
		
		virtual int defaultDeviceIndex();
};

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

AOQueryWirelessBridge::AOQueryWirelessBridge() : AOQueryDevice()
{}

//-------------------------------------------------------------------------------------------

AOQueryWirelessBridge::~AOQueryWirelessBridge()
{}

//-------------------------------------------------------------------------------------------

bool AOQueryWirelessBridge::queryNames()
{
	
}

//-------------------------------------------------------------------------------------------

bool AOQueryWirelessBridge::queryDevice(int idx)

//-------------------------------------------------------------------------------------------

int AOQueryWirelessBridge::defaultDeviceIndex()

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace orcus
//-------------------------------------------------------------------------------------------
