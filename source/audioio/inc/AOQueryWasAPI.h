//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_AUDIOIO_AOQUERYWASAPI_H
#define __ORCUS_AUDIOIO_AOQUERYWASAPI_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOQueryDevice.h"
#include "audioio/inc/WasAPIIF.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT AOQueryWasAPI : public AOQueryDevice
{
	public:
		class DeviceWasAPI;

	public:
		AOQueryWasAPI();
		virtual ~AOQueryWasAPI();
		
		virtual bool queryNames();
		virtual bool queryDevice(int idx);
		
		virtual int defaultDeviceIndex();
};

//------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT AOQueryWasAPI::DeviceWasAPI : public AOQueryDevice::Device
{
	public:
		DeviceWasAPI();
		DeviceWasAPI(const DeviceWasAPI& rhs);
		virtual ~DeviceWasAPI();

		WasAPIDeviceSPtr deviceInterface() const;
		void setDeviceInterface(WasAPIDeviceSPtr pDeviceIF);

		virtual void print() const;

	protected:

		WasAPIDeviceSPtr m_pDeviceInterface;

		virtual void copy(const AOQueryDevice::Device& rhs);
};

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
