#include "track/info/inc/DaemonInfo.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace track
{
namespace info
{
//-------------------------------------------------------------------------------------------

DaemonInfo::DaemonInfo(tint id, void *client) : Info(),
	m_id(id),
	m_imageUrl(),
	m_client(client)
{
	m_type = Info::e_InfoDaemon;
}

//-------------------------------------------------------------------------------------------

DaemonInfo::~DaemonInfo()
{}

//-------------------------------------------------------------------------------------------

void DaemonInfo::printError(const tchar *strR,const tchar *strE) const
{
	common::Log::g_Log << "DaemonInfo::" << strR << " - " << strE << "." << common::c_endl;
}

//-------------------------------------------------------------------------------------------

tint DaemonInfo::id() const
{
	return m_id;
}

//-------------------------------------------------------------------------------------------

void DaemonInfo::setBitrate(tint rate)
{
	m_bitrate = rate;
}

//-------------------------------------------------------------------------------------------

void DaemonInfo::setNoChannels(tint noChs)
{
	m_noChannels = noChs;
}

//-------------------------------------------------------------------------------------------

void DaemonInfo::setFrequency(tint freq)
{
	m_frequency = freq;
}

//-------------------------------------------------------------------------------------------

const QString& DaemonInfo::imageUrl() const
{
	return m_imageUrl;
}

//-------------------------------------------------------------------------------------------

QString& DaemonInfo::imageUrl()
{
	return m_imageUrl;
}

//-------------------------------------------------------------------------------------------

void *DaemonInfo::client()
{
	return m_client;
}

//-------------------------------------------------------------------------------------------
} // namespace info
} // namespace track
} // namespace orcus
//-------------------------------------------------------------------------------------------
