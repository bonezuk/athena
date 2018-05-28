#include "audioio/inc/AOQueryDevice.h"
#include "common/inc/Log.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace audioio
{
//-------------------------------------------------------------------------------------------
// AOQueryDevice::Channel
//-------------------------------------------------------------------------------------------

AOQueryDevice::Channel::Channel() : m_name()
{}

//-------------------------------------------------------------------------------------------

AOQueryDevice::Channel::Channel(const Channel& rhs) : m_name()
{
	copy(rhs);
}

//-------------------------------------------------------------------------------------------

AOQueryDevice::Channel::~Channel()
{}

//-------------------------------------------------------------------------------------------

const AOQueryDevice::Channel& AOQueryDevice::Channel::operator = (const Channel& rhs)
{
	if(this!=&rhs)
	{
		copy(rhs);
	}
	return *this;
}

//-------------------------------------------------------------------------------------------

QString& AOQueryDevice::Channel::name()
{
	return m_name;
}

//-------------------------------------------------------------------------------------------

const QString& AOQueryDevice::Channel::name() const
{
	return m_name;
}

//-------------------------------------------------------------------------------------------

void AOQueryDevice::Channel::copy(const Channel& rhs)
{
	m_name = rhs.m_name;
}

//-------------------------------------------------------------------------------------------
// AOQueryDevice::Device
//-------------------------------------------------------------------------------------------

AOQueryDevice::Device::Device() : m_initFlag(false),
	m_type(e_deviceUnknown),
	m_id(),
	m_name(),
	m_frequencySet(),
	m_channels(),
	m_hasExclusive(false)
{}

//-------------------------------------------------------------------------------------------

AOQueryDevice::Device::Device(Type type) : m_initFlag(false),
	m_type(type),
	m_id(),
	m_name(),
	m_frequencySet(),
	m_channels(),
	m_hasExclusive(false)
{}

//-------------------------------------------------------------------------------------------

AOQueryDevice::Device::Device(const Device& rhs) : m_initFlag(false),
	m_type(e_deviceUnknown),
	m_id(),
	m_name(),
	m_frequencySet(),
	m_channels(),
	m_hasExclusive(false)
{
	AOQueryDevice::Device::copy(rhs);
}

//-------------------------------------------------------------------------------------------

AOQueryDevice::Device::~Device()
{}

//-------------------------------------------------------------------------------------------

const AOQueryDevice::Device& AOQueryDevice::Device::operator = (const Device& rhs)
{
	if(this!=&rhs)
	{
		copy(rhs);
	}
	return *this;
}

//-------------------------------------------------------------------------------------------

void AOQueryDevice::Device::copy(const Device& rhs)
{
	m_initFlag = rhs.m_initFlag;
	m_type = rhs.m_type;
	m_id = rhs.m_id;
	m_name = rhs.m_name;
	m_frequencySet = rhs.m_frequencySet;
	m_channels = rhs.m_channels;
	m_hasExclusive = rhs.m_hasExclusive;
}

//-------------------------------------------------------------------------------------------

bool AOQueryDevice::Device::isInitialized() const
{
	return m_initFlag;
}

//-------------------------------------------------------------------------------------------

void AOQueryDevice::Device::setInitialized()
{
	m_initFlag = true;
}

//-------------------------------------------------------------------------------------------

const AOQueryDevice::Device::Type& AOQueryDevice::Device::type() const
{
	return m_type;
}

//-------------------------------------------------------------------------------------------

QString& AOQueryDevice::Device::id()
{
	return m_id;
}

//-------------------------------------------------------------------------------------------

const QString& AOQueryDevice::Device::id() const
{
	return m_id;
}

//-------------------------------------------------------------------------------------------

QString& AOQueryDevice::Device::name()
{
	return m_name;
}

//-------------------------------------------------------------------------------------------

const QString& AOQueryDevice::Device::name() const
{
	return m_name;
}

//-------------------------------------------------------------------------------------------

bool AOQueryDevice::Device::isFrequencySupported(int freq) const
{
	return (m_frequencySet.find(freq)!=m_frequencySet.end());
}

//-------------------------------------------------------------------------------------------

void AOQueryDevice::Device::addFrequency(int freq)
{
	m_frequencySet.insert(freq);
}

//-------------------------------------------------------------------------------------------

const QSet<int>& AOQueryDevice::Device::frequencies() const
{
	return m_frequencySet;
}

//-------------------------------------------------------------------------------------------

AOQueryDevice::Channel& AOQueryDevice::Device::channel(int idx)
{
	return m_channels[idx];
}

//-------------------------------------------------------------------------------------------

const AOQueryDevice::Channel& AOQueryDevice::Device::channel(int idx) const
{
	return m_channels.at(idx);
}

//-------------------------------------------------------------------------------------------

int AOQueryDevice::Device::noChannels() const
{
	return m_channels.size();
}

//-------------------------------------------------------------------------------------------

void AOQueryDevice::Device::setNoChannels(int noCh)
{
	int i;
	
	m_channels.clear();
	for(i=0;i<noCh;i++)
	{
		Channel ch;
		m_channels.append(ch);
	}
}

//-------------------------------------------------------------------------------------------

bool AOQueryDevice::Device::hasExclusive() const
{
	return m_hasExclusive;
}

//-------------------------------------------------------------------------------------------

void AOQueryDevice::Device::setHasExclusive(bool flag)
{
	m_hasExclusive = flag;
}

//-------------------------------------------------------------------------------------------

void AOQueryDevice::Device::print() const
{
	int i;
	QList<int> sFreq;
	QSet<int>::const_iterator ppI;
	
	for(ppI=m_frequencySet.begin();ppI!=m_frequencySet.end();++ppI)
	{
		sFreq.append(*ppI);
	}
	qSort(sFreq.begin(),sFreq.end());
	
	common::Log::g_Log.print("Device UUID : %s\n",m_id.toUtf8().constData());
	common::Log::g_Log.print("Device Name : %s\n",m_name.toUtf8().constData());
	common::Log::g_Log.print("Channels :");
	for(i=0;i<m_channels.size();i++)
	{
		common::Log::g_Log.print(" %d %s,",i,m_channels.at(i).name().toUtf8().constData());
	}
	common::Log::g_Log.print("\n");
	common::Log::g_Log.print("Frequencies : ");
	for(i=0;i<sFreq.size();i++)
	{
		common::Log::g_Log.print(" %d",sFreq.at(i));
	}
	common::Log::g_Log.print("\n");
}

//-------------------------------------------------------------------------------------------
// AOQueryDevice
//-------------------------------------------------------------------------------------------

AOQueryDevice::AOQueryDevice() : m_devices()
{}

//-------------------------------------------------------------------------------------------

AOQueryDevice::~AOQueryDevice()
{
	for(int i=0;i<m_devices.size();i++)
	{
		Device *dev = m_devices[i];
		delete dev;
	}
	m_devices.clear();
}

//-------------------------------------------------------------------------------------------

int AOQueryDevice::noDevices() const
{
	return m_devices.size();
}

//-------------------------------------------------------------------------------------------

const AOQueryDevice::Device& AOQueryDevice::device(int idx) const
{
	return *(m_devices.at(idx));
}

//-------------------------------------------------------------------------------------------

void AOQueryDevice::print() const
{
	int i;

	common::Log::g_Log.print("Number of Devices : %d\n\n",noDevices());
	for(i=0;i<noDevices();i++)
	{
		device(i).print();
	}
}

//-------------------------------------------------------------------------------------------
} // namespace audioio
} // namespace orcus
//-------------------------------------------------------------------------------------------
