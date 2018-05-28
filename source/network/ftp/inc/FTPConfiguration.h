//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_FTP_FTPCONFIGURATION_H
#define __ORCUS_NETWORK_FTP_FTPCONFIGURATION_H
//-------------------------------------------------------------------------------------------

#include "network/ftp/inc/FTPTransfer.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
namespace ftp
{
//-------------------------------------------------------------------------------------------

class FTP_EXPORT FTPConfiguration
{
	public:
		FTPConfiguration();
		virtual ~FTPConfiguration();
		
		virtual const QString& rootPath() const;
		virtual void setRootPath(const QString& p);
				
		virtual tint passivePort() const;
		virtual void setPassivePort(tint p);
		
		virtual bool isUser(const QString& username,const QString& password);
		
	protected:
	
		QString m_rootPath;
		tint m_port;
};

//-------------------------------------------------------------------------------------------
} // namespace ftp
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

