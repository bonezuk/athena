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
		const tfloat64 c_timeoutDefault = 0.2;
		
	public:
		IPCSocketComms(Type type, QObject *parent = 0);
		virtual ~IPCSocketComms();
		
		virtual bool open(const QString& socketPath);
		virtual void close();
		
		virtual int read(QByteArray& arr);
		virtual int write(const QByteArray& arr);
		
		virtual const Type& type() const;
		virtual const common::TimeStamp& timeoutRead() const;
		virtual void setReadTimeout(const common::TimeStamp& t);
		virtual const common::TimeStamp& timeoutWrite() const;
		virtual void setWriteTimeout(const common::TimeStamp& t);
		virtual void setNoTimeout(bool isTimeout);
		
	private:
		Type m_type;
		QString m_socketPath;
		network::socket_type m_socket;
		network::socket_type m_clientSocket;
		common::TimeStamp m_timeoutRead;
		common::TimeStamp m_timeoutWrite;
		bool m_isTimeout;
		bool m_isMessage;
		
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
