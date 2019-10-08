//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_NETWORK_SOCKET_H
#define __ORCUS_NETWORK_SOCKET_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Service.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace network
{
//-------------------------------------------------------------------------------------------

class NETWORK_EXPORT Socket : public QObject
{
	public:
		Q_OBJECT
	
	public:
        typedef QSharedPointer<SocketPtr> SocketSPtr;

		static const tuint32 c_socketStateRead  = 0x00000001;
		static const tuint32 c_socketStateWrite = 0x00000002;
		static const tuint32 c_socketStateClose = 0x00000004;
	
	public:
		Socket(QSharedPointer<Service>& svr,QObject *parent = 0);
		virtual ~Socket();
		
		QSharedPointer<Service> service() const;
		socket_type socket();
		
		tuint32& state();
		const tuint32& state() const;
		
		virtual bool canRead() const = 0;
		virtual bool canWrite() const = 0;
		
		virtual bool doRead() = 0;
		virtual bool doWrite() = 0;

		virtual void setForRead();
		virtual void setForWrite();
		
		virtual bool isConnected() const;
		virtual bool isComplete() const;
		virtual void markAsComplete();
		
		virtual void deleteServiceConnection();
		
	protected:
		QSharedPointer<Service> m_service;
		socket_type m_socket;
		tuint32 m_state;
		bool m_isComplete;
	
		virtual void printError(const tchar *strR,const tchar *strE) const;
		virtual void printError(const tchar *strR,const tchar *strE,tint eNo) const;
		
		virtual void addServiceConnection();
};

//-------------------------------------------------------------------------------------------

inline QSharedPointer<Service> Socket::service() const
{
	QSharedPointer<Service> pService(m_service);
	return pService;
}

//-------------------------------------------------------------------------------------------

inline socket_type Socket::socket()
{
	return m_socket;
}

//-------------------------------------------------------------------------------------------

inline tuint32& Socket::state()
{
	return m_state;
}

//-------------------------------------------------------------------------------------------

inline const tuint32& Socket::state() const
{
	return m_state;
}

//-------------------------------------------------------------------------------------------
} // namespace network
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

