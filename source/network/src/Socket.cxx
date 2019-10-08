#include "network/inc/Socket.h"
#include "network/inc/Controller.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
//-------------------------------------------------------------------------------------------

Socket::Socket(QSharedPointer<Service>& svr,QObject *parent) : QObject(parent),
	m_service(svr),
	m_socket(c_invalidSocket),
	m_state(0),
	m_isComplete(false)
{}

//-------------------------------------------------------------------------------------------

Socket::~Socket()
{}

//-------------------------------------------------------------------------------------------

void Socket::printError(const tchar *strR,const tchar *strE) const
{
	common::Log::g_Log << "Socket::" << strR << " - " << strE << "." << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void Socket::printError(const tchar *strR,const tchar *strE,tint eNo) const
{
	Resource::instance().error("Socket",strR,strE,eNo);
}

//-------------------------------------------------------------------------------------------

void Socket::addServiceConnection()
{
	if(!m_service.isNull() && !m_service->controller().isNull() && !m_self.isNull())
	{
		m_service->controller()->addSocket(m_service, m_self);
	}
}

//-------------------------------------------------------------------------------------------

void Socket::deleteServiceConnection()
{
	if(!m_service.isNull() && !m_service->controller().isNull() && !m_self.isNull())
	{
		m_service->controller()->delSocket(m_service, m_self);
	}
	m_service.clear();
}

//-------------------------------------------------------------------------------------------

void Socket::setForRead()
{
	m_state |= c_socketStateRead;
}

//-------------------------------------------------------------------------------------------

void Socket::setForWrite()
{
	m_state |= c_socketStateWrite;
}

//-------------------------------------------------------------------------------------------

bool Socket::isConnected() const
{
	return (m_socket != c_invalidSocket);
}

//-------------------------------------------------------------------------------------------

bool Socket::isComplete() const
{
	return m_isComplete;
}

//-------------------------------------------------------------------------------------------

void Socket::markAsComplete()
{
	m_isComplete = true;
}

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
