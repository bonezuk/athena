//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_FTP_FTPSERVER_H
#define __ORCUS_NETWORK_FTP_FTPSERVER_H
//-------------------------------------------------------------------------------------------

#include "network/inc/TCPServerSocket.h"
#include "network/inc/TCPService.h"
#include "network/ftp/inc/FTPTransfer.h"
#include "network/ftp/inc/FTPConfiguration.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace ftp
{
//-------------------------------------------------------------------------------------------

class FTP_EXPORT FTPServer : public TCPServerSocket
{
	public:
		Q_OBJECT
		
	public:
		FTPServer(Service *svr,QObject *parent = 0);
		virtual ~FTPServer();
		
		FTPConfiguration& config();
		const FTPConfiguration& config() const;
		
	protected:
		
		FTPConfiguration m_config;
		
		virtual TCPConnServerSocket *newIO();
};

//-------------------------------------------------------------------------------------------
} // namespace ftp
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

