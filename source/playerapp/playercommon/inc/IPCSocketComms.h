//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_PLAYERCOMMON_IPCSOCKETCOMMS_H
#define __OMEGA_PLAYERAPP_PLAYERCOMMON_IPCSOCKETCOMMS_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"
#include "common/inc/BIOStream.h"
#include "common/inc/DiskOps.h"
#include "common/inc/TimeStamp.h"
#include "playerapp/playercommon/inc/PlayerCommonDLL.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

class PLAYERCOMMON_EXPORT IPCSocketComms : public QObject
{
	Q_OBJECT
	public:
		typedef enum
		{
			e_Client = 0,
			e_Server
		} Type;
		
		// Set socket up such that only one client can connect at any given time
		const int c_maxBacklog = 1;
		const tfloat64 c_timeoutDefault = 1.0;
		
	public:
		IPCSocketComms(Type type, QObject *parent = 0);
		virtual ~IPCSocketComms();
		
		bool open(const QString& socketPath);
		void close();
		
		int read(QByteArray& arr);
		int write(const QByteArray& arr);
		
		const Type& type() const;
		const common::TimeStamp& timeout() const;
		void setTimeout(const common::TimeStamp& t);
		void setNoTimeout(bool isTimeout);
		
	private:
		Type m_type;
		QString m_socketPath;
		network::socket_type m_socket;
		network::socket_type m_clientSocket;
		common::TimeStamp m_timeout;
		bool m_isTimeout;
		
		virtual void printError(const char *strR, const char *strE) const;
		virtual bool removePreviousSocket(const QString& socketPath);
		virtual bool openServer(const QString& socketPath, struct sockaddr_un *addr);
		virtual bool openClient(const QString& socketPath, struct sockaddr_un *addr);
		virtual bool socketToNonblocking(network::socket_type s);
		virtual network::socket_type openConnectionFromClient();
		virtual network::socket_type getClientConnection();
		virtual network::socket_type closeSocket(network::socket_type s);
		virtual void closeConnection();
		virtual bool canDoSocketIO(network::socket_type s, bool isRead) const;
		virtual bool canReadFromSocket(network::socket_type s) const;
		virtual bool canWriteToSocket(network::socket_type s) const;
		virtual bool isValidSocketAndBuffer(network::socket_type s, const uint8_t *data, int len, bool isEof) const;
		virtual int readFromSocket(network::socket_type s, uint8_t *data, int len, bool& isEof);
		virtual tint32 readMessageSize(network::socket_type s, bool& isEof);
		virtual tint32 readMessageHeader(network::socket_type s, bool& isEof);
		virtual int writeToSocket(network::socket_type s, const uint8_t *data, int len, bool& isEof);
		virtual bool writeMessageHeader(network::socket_type s, tint32 msgLen, bool& isEof);
		virtual int doWrite(const QByteArray& arr);
};

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
