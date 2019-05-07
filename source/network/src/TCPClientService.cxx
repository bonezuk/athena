#include "network/inc/TCPClientService.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
//-------------------------------------------------------------------------------------------

TCPClientService::TCPClientService(QObject *parent) : Service(parent),
	m_clientSet()
{}

//-------------------------------------------------------------------------------------------

TCPClientService::~TCPClientService()
{
	try
	{
		TCPClientService::stop();
	}
	catch(...) {}
}

//-------------------------------------------------------------------------------------------

void TCPClientService::printError(const tchar *strR,const tchar *strE) const
{
	common::Log::g_Log << "TCPClientService::" << strR << " - " << strE << "." << common::c_endl;
}

//-------------------------------------------------------------------------------------------

void TCPClientService::addConnection(QSharedPointer<TCPConnectionSocket>& s)
{
	m_clientSet.insert(s);
}

//-------------------------------------------------------------------------------------------

void TCPClientService::delConnection(QSharedPointer<TCPConnectionSocket>& s)
{
	QSet<QSharedPointer<TCPConnectionSocket> >::iterator ppI;
	
	ppI = m_clientSet.find(s);
	if(ppI!=m_clientSet.end())
	{
		m_clientSet.erase(ppI);
	}
}

//-------------------------------------------------------------------------------------------

bool TCPClientService::start()
{
	return true;
}

//-------------------------------------------------------------------------------------------

void TCPClientService::stop()
{
	QSet<QSharedPointer<TCPConnectionSocket> >::iterator ppI;
	
	while(ppI=m_clientSet.begin(),ppI!=m_clientSet.end())
	{
		QSharedPointer<TCPConnectionSocket> s = *ppI;
		m_clientSet.erase(ppI);
		s->close();
	}
}

//-------------------------------------------------------------------------------------------

bool TCPClientService::process()
{
	QSet<QSharedPointer<TCPConnectionSocket> >::iterator ppI;
	bool res = true;
	
	for(ppI=m_clientSet.begin();ppI!=m_clientSet.end();)
	{
		QSharedPointer<TCPConnectionSocket> s = *ppI;
		
		if(!s->process())
		{
			if(!s->isLocked())
			{
				ppI = m_clientSet.erase(ppI);
				s->close();
				res = false;
			}
			else
			{
				++ppI;
			}
		}
		else
		{
			++ppI;
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
