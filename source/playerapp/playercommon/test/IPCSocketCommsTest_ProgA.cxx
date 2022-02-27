#include "playerapp/playercommon/inc/IPCSocketComms.h"

using namespace orcus;

//-------------------------------------------------------------------------------------------

void sendAndReceiveOnUNIXDomainSocket_TestProcess(const char *socketPath)
{
	IPCSocketComms comm(IPCSocketComms::e_Client);
	
	while(!common::DiskOps::exist(socketPath))
	{
		fprintf(stdout, "B - waiting on server\n");
		::sleep(1);
	}

	fprintf(stdout, "B - open client to '%s'\n", socketPath);
	if(comm.open(QString::fromUtf8(socketPath)))
	{
		for(int i = 0; i < 10000; i++)
		{
			int res;
			QString testStr = QString("test no=%1").arg(i);
			QByteArray txArr, rxArr;
			
			txArr = testStr.toUtf8();
			fprintf(stdout, "B(%d) - send: %s\n", getpid(), txArr.constData());
			res = comm.write(txArr);
			if(res == txArr.size())
			{
				fprintf(stdout, "B(%d) - ready to receive\n", getpid());
				res = comm.read(rxArr);
				if(res > 0)
				{
					fprintf(stdout, "B(%d) - receive: %s\n", getpid(), rxArr.constData());
				}
				else if(res < 0)
				{
					fprintf(stdout, "B(%d) - failed to read data from comms\n", getpid());
				}
			}
			else
			{
				fprintf(stdout, "B(%d) - failed to write data to comms\n", getpid());
			}
		}
		fprintf(stdout, "B(%d) - closing client\n", getpid());
		comm.close();
	}
	else
	{
		fprintf(stdout, "B(%d) - failed to open client connection to '%s'\n", getpid(), socketPath);
	}
}

//-------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	fprintf(stdout, "B - process started\n");
	if(argc < 2)
	{
		fprintf(stdout, "B - no path to socket defined\n");
	}
	sendAndReceiveOnUNIXDomainSocket_TestProcess(argv[1]);
	fprintf(stdout, "B - process complete\n");
	return 0;
}

//-------------------------------------------------------------------------------------------
