//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_AUDIOIO_AOQUERYWIN32_H
#define __ORCUS_AUDIOIO_AOQUERYWIN32_H
//-------------------------------------------------------------------------------------------

#include "audioio/inc/AOQueryASIO.h"
#include "audioio/inc/AOQueryWasAPI.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace audioio
{
//-------------------------------------------------------------------------------------------

class AUDIOIO_EXPORT AOQueryWin32 : public AOQueryDevice
{
	public:
		AOQueryWin32();
		virtual ~AOQueryWin32();
		
		virtual bool queryNames();
		virtual bool queryDevice(int idx);
		
		virtual int defaultDeviceIndex();

		virtual int noDevices() const;
		virtual const AOQueryDevice::Device& device(int idx) const;

	protected:
		
		AOQueryASIO *m_devicesASIO;
		AOQueryWasAPI *m_devicesWasAPI;
};

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
