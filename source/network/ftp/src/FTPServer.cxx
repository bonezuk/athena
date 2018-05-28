#include "network/ftp/inc/FTPServer.h"
#include "network/ftp/inc/FTPSession.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace ftp
{
//-------------------------------------------------------------------------------------------

FTPServer::FTPServer(Service *svr,QObject *parent) : TCPServerSocket(svr,parent),
	m_config()
{}

//-------------------------------------------------------------------------------------------

FTPServer::~FTPServer()
{}

//-------------------------------------------------------------------------------------------

FTPConfiguration& FTPServer::config()
{
	return m_config;
}

//-------------------------------------------------------------------------------------------

const FTPConfiguration& FTPServer::config() const
{
	return m_config;
}

//-------------------------------------------------------------------------------------------

TCPConnServerSocket *FTPServer::newIO()
{
	FTPSession *session = new FTPSession(this,this);
	return reinterpret_cast<TCPConnServerSocket *>(session);
}

//-------------------------------------------------------------------------------------------
} // namespace ftp
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
