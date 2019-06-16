//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_TCPCLIENTSERVICE_H
#define __ORCUS_NETWORK_TCPCLIENTSERVICE_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Service.h"
#include "network/inc/TCPConnClientSocket.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
//-------------------------------------------------------------------------------------------

class NETWORK_EXPORT TCPClientService : public Service
{
	public:
		Q_OBJECT

	public:
		TCPClientService(QObject *parent = 0);
		virtual ~TCPClientService();
		
		virtual bool start();
		virtual void stop();

		virtual void delConnection(TCPConnectionSocket *s);

		virtual void addConnection(QSharedPointer<TCPConnectionSocket>& s);
		virtual void delConnection(QSharedPointer<TCPConnectionSocket>& s);

		virtual bool process();
		
		virtual void processComplete();
		
	protected:
		
		QSet<QSharedPointer<TCPConnectionSocket> > m_clientSet;
		
		virtual void printError(const tchar *strR,const tchar *strE) const;
};

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

