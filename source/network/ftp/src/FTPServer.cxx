#include "network/ftp/inc/FTPServer.h"
#include "network/ftp/inc/FTPSession.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace network
{
namespace ftp
{
//-------------------------------------------------------------------------------------------

FTPFileFilter::FTPFileFilter()
{}

//-------------------------------------------------------------------------------------------

FTPFileFilter::~FTPFileFilter()
{}

//-------------------------------------------------------------------------------------------

FTPServer::FTPServer(Service *svr,QObject *parent) : TCPServerSocket(svr,parent),
	m_config(),
	m_filter(0)
{}

//-------------------------------------------------------------------------------------------

FTPServer::~FTPServer()
{
	if(m_filter != 0)
	{
		delete m_filter;
		m_filter = 0;
	}
}

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

bool FTPServer::canFileBeUploaded(const QString& fileName)
{
	bool res = true;
	
	if(m_filter != 0)
	{
		res = m_filter->canFileBeUploaded(fileName);
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void FTPServer::setFileFilter(FTPFileFilter *filter)
{
	m_filter = filter;
}

//-------------------------------------------------------------------------------------------

void FTPServer::signalUploadComplete(const QString& fileName)
{
	if(common::DiskOps::exist(fileName))
	{
		emit uploaded(fileName);
	}
}

//-------------------------------------------------------------------------------------------
} // namespace ftp
} // namespace network
} // namespace omega
//-------------------------------------------------------------------------------------------
