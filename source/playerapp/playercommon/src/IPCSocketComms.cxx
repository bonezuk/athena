#include "playerapp/playercommon/inc/IPCSocketComms.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

IPCSocketComms::IPCSocketComms(Type type, QObject *parent) : QObject(parent),
	m_type(type),
	m_socketPath(),
	m_socket(network::c_invalidSocket),
	m_clientSocket(network::c_invalidSocket),
{}

//-------------------------------------------------------------------------------------------

IPCSocketComms::~IPCSocketComms()
{
	IPCSocketComms::close();
}

//-------------------------------------------------------------------------------------------

const IPCSocketComms::Type& IPCSocketComms::type() const
{
	return m_type;
}

//-------------------------------------------------------------------------------------------

const common::TimeStamp& IPCSocketComms::timeout() const
{
	return m_timeout;
}

//-------------------------------------------------------------------------------------------

void IPCSocketComms::setTimeout(const common::TimeStamp& t)
{
	m_timeout = t;
}

//-------------------------------------------------------------------------------------------

void IPCSocketComms::printError(const char *strR, const char *strE) const
{
	common::g_Log << "IPCSocketComms:: - " << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms::removePreviousSocket(const QString& socketPath)
{
	bool res = true;
	
	if(common::DiskOps::exist(socketPath))
	{
		if(common::DiskOps::remove(socketPath) < 0)
		{
			QString err = QString("Failed to remove previous socket file '%1'. %2")
				.arg(socketPath).arg(strerror(errno));
			printError("removePreviousSocket", err.toUtf8().constData());
			res = false;
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms::openServer(const QString& socketPath, struct sockaddr_un *addr)
{
	bool res = false;
	
	if(removePreviousSocket(socketPath))
	{
		m_socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
		if(m_socket != network::c_invalidSocket)
		{
			int len;
			QByteArray sName = socketPath.toUtf8().constData();
			
			if(!bind(m_socket, reinterpret_cast<struct sockaddr *>(addr), sizeof(struct sockaddr_un)))
			{
				if(!listen(m_socket, c_maxBacklog))
				{
					if(socketToNonblocking(m_socket))
					{
						m_socketPath = socketPath;
						res = true;
					}
					else
					{
						printError("openServer", "Failed to set socket to non-blocking");
					}
				}
				else
				{
					QString err = QString("Failed to listen on UNIX domain socket '%1'. %2")
						.arg(socketPath).arg(strerror(errno));
					printError("openServer", err.toUtf8().constData());
				}
			}
			else
			{
				QString err = QString("Failed to bind to UNIX domain socket '%1'. %2")
					.arg(socketPath).arg(strerror(errno));
				printError("openServer", err.toUtf8().constData());
			}
		}
		else
		{
			QString err = QString("Failed to get UNIX domain socket. %1").arg(::strerror(errno));
			printError("openServer", err.toUtf8().constData());
		}
	}
	else
	{
		printError("openServer", "Failed to remove previous socket");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms::openClient(const QString& socketPath, struct sockaddr_un *addr)
{
	bool res = false;
	
	if(common::DiskOps::exist(socketPath))
	{
		m_socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
		if(m_socket != network::c_invalidSocket)
		{
			if(!::connect(m_socket, reinterpret_cast<struct sockaddr *>(addr), sizeof(struct sockaddr_un)))
			{
				if(socketToNonblocking(m_socket))
				{
					res = true;
				}
				else
				{
					printError("openClient", "Failed to set socket to non-blocking");
				}
			}
			else
			{
				QString err = QString("Failed to connect to UNIX domain socket '%1'. %2")
					.arg(socketPath).arg(strerror(errno));
				printError("openClient", err.toUtf8().constData());
			}
		}
		else
		{
			QString err = QString("Failed to get UNIX domain socket. %1").arg(::strerror(errno));
			printError("openClient", err.toUtf8().constData());
		}
	}
	else
	{
		QString err = QString("UNIX domain socket '%1' does not exist. %2")
			.arg(socketPath).arg(strerror(errno));
		printError("openClient", err.toUtf8().constData());
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms::open(const QString& socketPath)
{
	int len;
	struct sockaddr_un addr;
	
	close();
	
	if(socketPath.toUtf8().size() > sizeof(addr.sun_path) - 1)
	{
		QString err = QString("Server socket path '%1' is too long.").arg(socketPath);
		printError("openServer", err.toUtf8().constData());
		return false;
	}
	
	len = (sName.size() < sizeof(addr.sun_path) - 1) ? sName.size() : sizeof(addr.sun_path) - 1;
	::memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	::strncpy(addr.sun_path, sName.data(), len);
	
	return (type() == e_Server) ? openServer(socketPath, &addr) : openClient(socketPath, &addr);
}

//-------------------------------------------------------------------------------------------

network::socket_type IPCSocketComms::closeSocket(network::socket_type s)
{
	if(s != network::c_invalidSocket)
	{
		::shutdown(s, SHUT_RDWR);
		::close(s);	
	}
	return network::c_invalidSocket;
}

//-------------------------------------------------------------------------------------------

void IPCSocketComms::closeConnection()
{
	if(type() == e_Client)
	{
		m_socket = closeSocket(m_socket);
	}
	else
	{
		m_clientSocket = closeSocket(m_clientSocket);
	}
}

//-------------------------------------------------------------------------------------------

void IPCSocketComms::close()
{
	if(type() == e_Server)
	{
		if(m_clientSocket != network::c_invalidSocket)
		{
			m_clientSocket = closeSocket(m_clientSocket);
		}
	}
	if(m_socket != network::c_invalidSocket)
	{
		m_socket = closeSocket(m_socket);
	}
	if(type() == e_Server)
	{
		removePreviousSocket(m_socketPath);
	}
	m_socketPath = "";
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms::socketToNonblocking(network::socket_type s)
{
#if defined(OMEGA_WIN23)
	u_long cmdParameter = 1;
	
	if(::ioctlsocket(s,static_cast<long>(FIONBIO),&cmdParameter)!=0)
	{
		printError("open","Failed to set socket to non-blocking mode");
		return false;
	}
#elif defined(OMEGA_POSIX)
	tint val;
	
	val = ::fcntl(s, F_GETFL, 0);
	if(val!=-1)
	{
		if(::fcntl(s, F_SETFL, val | O_NONBLOCK)==-1)
		{
			printError("open","Failed to set socket to non-blocking mode");
			return false;
		}
	}
	else
	{
		printError("open","Failed to get file descriptor flags");
		return false;
	}
#endif
	return true;
}

//-------------------------------------------------------------------------------------------

network::socket_type IPCSocketComms::openConnectionFromClient()
{
	struct sockaddr_un addr;
	network::socket_type cSocket = network::c_invalidSocket;
	addrlen_type len = sizeof(struct sockaddr_in);

	if(type() == e_Server && m_socket == network::c_invalidSocket)
	{
		printError("openConnectionFromClient", "Invalid state to open connection from client")
		return cSocket;
	}
	cSocket = ::accept(m_socket, reinterpret_cast<struct sockaddr *>(&addr), &len);
	if(cSocket != network::c_invalidSocket)
	{
		if(!socketToNonblocking(cSocket))
		{
			printError("openConnectionFromClient", "Failed to set client connection to nonblocking");
			cSocket = closeSocket(cSocket);
		}
	}
	else
	{
		QStrint err = QString("Error accepting connection from client on socket '%1'. %2")
			.arg(socketPath).arg(strerror(errno));
		printError("openConnectionFromClient", err.toUtf8().constData());
	}
	return cSocket;
}

//-------------------------------------------------------------------------------------------

network::socket_type IPCSocketComms::getClientConnection()
{
	network::socket_type cSocket = network::c_invalidSocket;

	if(type() == e_Server)
	{
		if(m_socket == network::c_invalidSocket)
		{
			printError("getClientConnection", "No server UNIX domain socket has been opened");
			return cSocket;
		}
	
		if(m_clientSocket == network::c_invalidSocket)
		{
			int res;
			fd_set set;
			struct timeval tv;
			
			FD_ZERO(&set);
			FD_SET(m_socket, &set);
			tv.tv_sec = m_timeout.secondsTotal();
			tv.tv_usec = m_timeout.microsecond();
			
			res = ::select(m_socket + 1, &set, 0, 0, &tv);
			if(res > 0)
			{
				cSocket = openConnectionFromClient();
				if(cSocket != network::c_invalidSocket)
				{
					m_clientSocket = cSocket;
				}
				else
				{
					printError("getClientConnection", "Failed to open connection from client");
				}
			}
			else if(res < 0)
			{
				QString err = QString("Error waiting for client connection on socket '%1'. %2")
					.arg(m_socketPath).arg(strerror(errno));
				printError("getClientConnection", err.toUtf8().constData());
			}
		}
		else
		{
			cSocket = m_clientSocket;
		}
	}
	else
	{
		printError("getClientConnection", "Can only sever can receive client connection");
	}
	return cSocket;
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms::canDoSocketIO(network::socket_type s, bool isRead) const
{
	int res;
	fd_set set;
	struct timeval tv;
	
	if(s == network::c_invalidSocket)
	{
		printError("canReadFromSocket", "Invalid socket given");
		return false;
	}

	FD_ZERO(&set);
	FD_SET(s, &set);
	tv.tv_sec = m_timeout.secondsTotal();
	tv.tv_usec = m_timeout.microsecond();
	
	if(isRead)
	{
		res = ::select(s + 1, &set, 0, 0, &tv);
	}
	else
	{
		res = ::select(s + 1, 0, &set, 0, &tv);
	}
	if(res > 0)
	{
		return (FD_ISSET(s, &set)) ? true : false;
	}
	else if(res < 0)
	{
		QString err = QString("Error waiting to read from socket. %1").arg(::strerror(errno));
		printError("canReadFromSocket", err.toUtf8().constData());
	}
	return false;
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms::canReadFromSocket(network::socket_type s) const
{
	return canDoSocketIO(s, true);
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms::canWriteToSocket(network::socket_type s) const
{
	return canDoSocketIO(s, false);
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms::isValidSocketAndBuffer(network::socket_type s, const uint8_t *data, int len, bool isEof) const
{
	if(data == 0)
	{
		printError("isValidSocketAndBuffer", "No data buffer given to write data from");
		return false;
	}
	if(len <= 0)
	{
		QString err = QString("Zero or negative length of %1 is invalid").arg(len);
		printError("isValidSocketAndBuffer", err.toUtf8().constData());
		return false;
	}
	if(s == network::c_invalidSocket)
	{
		printError("isValidSocketAndBuffer", "Invalid socket given");
		return false;
	}
	if(isEof)
	{
		printError("isValidSocketAndBuffer", "Socket has been previously closed");
		return false;
	}
	return true;
}

//-------------------------------------------------------------------------------------------

int IPCSocketComms::readFromSocket(network::socket_type s, uint8_t *data, int len, bool& isEof)
{
	int pos, res;

	if(!isValidSocketAndBuffer(s, data, len, isEof))
	{
		printError("readFromSocket", "Invalid parameters");
		return -1;
	}
	
	for(pos = 0; pos < len;)
	{
		res = ::read(s, &data[pos], len - pos);
		if(res > 0)
		{
			pos += res;
		}
		else if(res == 0)
		{
			isEof = true;
			return pos;
		}
		else
		{
			if(errno == EAGAIN || errno == EWOULDBLOCK)
			{
				if(!canReadFromSocket(s))
				{
					return 0;
				}
			}
			else if(errno != EINTR)
			{
				QString err = QString("Error reading from socket. %1").arg(::strerror(errno));
				printError("readFromSocket", err.toUft8().constData());
				return -1;
			}
		}
	}
	return pos;
}

//-------------------------------------------------------------------------------------------

tint32 IPCSocketComms::readMessageSize(network::socket_type s, bool& isEof)
{
	int res;
	uint8_t dataIn[4];
	tint32 msgSize = -1;
	
	res = readFromSocket(s, dataIn, 4, isEof);
	if(res == 4)
	{
		tint32 *sizeArr = reinterpret_cast<tint32 *>(dataIn);
		msgSize = sizeArr[0];
		if(msgSize <= 0)
		{
			printError("readMessageSize", "Invalid message size");
			msgSize = -1;
		}		
	}
	else if(res < 0)
	{
		pritnError("readMessageSize", "Error reading message size");
	}
	else
	{
		msgSize = 0;
	}
	return msgSize;
}

//-------------------------------------------------------------------------------------------

tint32 IPCSocketComms::readMessageHeader(network::socket_type s, bool& isEof)
{
	const char c_headerID[4] = {'M', '#', 'G', '_'};
	uint8_t dataIn[1];
	int res, pos;
	tint32 msgSize = -1;
	bool loop = true;
	
	pos = 0;
	while(loop && !isEof)
	{
		res = readFromSocket(s, dataIn, 1, isEof);
		if(res == 1)
		{
			if(static_cast<char>(dataIn[0]) ==  c_headerID[pos])
			{
				pos++;
				if(pos >= 4)
				{
					msgSize = readMessageSize(s, isEof);
					loop = false;
				}
			}
			else
			{
				pos = 0;
			}
		}
		else if(!res)
		{
			return 0;
		}
		else
		{
			printError("readMessageHeader", "Error reading from socket");
			loop = false;
		}
	}
	return (!isEof) ? msgSize : 0;
}

//-------------------------------------------------------------------------------------------

int IPCSocketComms::read(QByteArray& arr)
{
	int msgSize;
	bool isEof = false;
	network::socket_type s;
	
	s = (type() == e_Client) ? m_socket : getClientConnection();
	if(s == network::c_invalidSocket)
	{
		if(type() == e_Client)
		{
			printError("read", "No UNIX domain socket open to read from");
			return -1;
		}
		else
		{
			return 0;
		}
	}
	
	msgSize = readMessageHeader(s, isEof);
	if(msgSize > 0)
	{
		int len;
		QByteArray inArr;
		
		inArr.resize(msgSize);
		len = readFromSocket(s, reinterpret_cast<uint8_t *>(inArr.data()), msgSize, isEof);
		if(len != msgSize)
		{
			if(len < 0)
			{
				printError("read", "Error reading message body");
				msgSize = -1;
			}
			else
			{
				msgSize = 0;
			}
		}
	}
	else if(msgSize < 0)
	{
		printError("read", "Error reading message header")
	}
	
	if(isEof)
	{
		closeConnection();
	}
	return msgSize;
}

//-------------------------------------------------------------------------------------------

int IPCSocketComms::writeToSocket(network::socket_type s, const uint8_t *data, int len, bool& isEof)
{
	int pos, res;

	if(!isValidSocketAndBuffer(s, data, len, isEof))
	{
		printError("readFromSocket", "Invalid parameters");
		return -1;
	}

	for(pos = 0; pos < len;)
	{
		res = write(s, &data[pos], len - pos);
		if(res > 0)
		{
			pos += res;
		}
		else if(res == 0)
		{
			isEof = true;
			return pos;
		}
		else
		{
			if(errno == EAGAIN || errno == EWOULDBLOCK)
			{
				if(!canWriteToSocket(s))
				{
					return 0;
				}
			}
			else if(errno != EINTR)
			{
				QString err = QString("Error reading from socket. %1").arg(::strerror(errno));
				printError("readFromSocket", err.toUft8().constData());
				return -1;
			}
		}
	}
	return pos;
}

//-------------------------------------------------------------------------------------------

bool IPCSocketComms::writeMessageHeader(network::socket_type s, tint32 msgLen, bool& isEof)
{
	const char c_headerID[4] = {'M', '#', 'G', '_'};
	bool res = false;
	
	if(writeToSocket(s, c_headerID, 4, isEof) == 4)
	{
		if(writeToSocket(s, reinterpret_cast<const uint8_t *>(&msgLen), 4, isEof) == 4)
		{
			res = true;
		}
		else
		{
			printError("writeMessageHeader", "Failed to write message size");
		}
	}
	else
	{
		printError("writeMessageHeader", "Failed to write message header");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

int IPCSocketComms::write(const QByteArray& arr)
{
	int res;
	
	if(arr.size() > 0)
	{
		s = (type() == e_Client) ? m_socket : getClientConnection();
		if(s == network::c_invalidSocket)
		{
			printError("write", "No UNIX domain socket open to write to");
			return -1;
		}
		
		if(writeMessageHeader(s, static_cast<tint32>(arr.size()), isEof))
		{
			if(writeToSocket(s, reinterpret_cast<const uint8_t *>(arr.constData(), arr.size(), isEof) == arr.size())
			{
				res = arr.size();
			}
			else
			{
				printError("write", "Failed to write message data");
			}
		}
		else
		{
			printError("write", "Failed to write message header");
			res = -1;
		}
		
		if(isEof)
		{
			closeConnection();
		}
	}
	else
	{
		res = 0;
	}
	return res;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
