#include "network/inc/Socket.h"
#include "network/inc/Controller.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace network
{
//-------------------------------------------------------------------------------------------

Socket::Socket(Service *svr,QObject *parent) : QObject(parent),
	m_service(svr),
	m_socket(c_invalidSocket),
	m_state(0)
{
	if(m_service!=0)
	{
		m_service->controller()->addSocket(m_service,this);
	}
}

//-------------------------------------------------------------------------------------------

Socket::~Socket()
{
	try
	{
		if(m_service!=0)
		{
			m_service->controller()->delSocket(m_service,this);
		}
	}
	catch(...) {}
}

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
} // namespace network
} // namespace omega
//-------------------------------------------------------------------------------------------
