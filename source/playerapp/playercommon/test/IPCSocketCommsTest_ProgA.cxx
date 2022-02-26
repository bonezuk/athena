#include "playerapp/playercommon/inc/IPCSocketComms.h"

using namespace orcus;

//-------------------------------------------------------------------------------------------

void sendAndReceiveOnUNIXDomainSocket_TestProcess(const char *socketPath)
{
	IPCSocketComms comm(IPCSocketComms::e_Client);
	
	fprintf(stdout, "B - open client to '%s'\n", socketPath);
	if(comm.open(QString::fromUtf8(socketPath)))
	{
		for(int i = 0; i < 100; i++)
		{
			int res;
			QString testStr = QString("test no=%1").arg(i);
			QByteArray txArr, rxArr;
			
			txArr = testStr.toUtf8();
			fprintf(stdout, "B - send: %s\n", txArr.constData());
			res = comm.write(txArr);
			if(res == txArr.size())
			{
				fprintf(stdout, "B - ready to receive\n");
				res = comm.read(rxArr);
				if(res > 0)
				{
					fprintf(stdout, "B - receive: %s\n", rxArr.constData());
				}
				else
				{
					fprintf(stdout, "B - failed to read data from comms\n");
				}
			}
			else
			{
				fprintf(stdout, "B - failed to write data to comms\n");
			}
		}
		fprintf(stdout, "B - closing client\n");
		comm.close();
	}
	else
	{
		fprintf(stdout, "B - failed to open client connection to '%s'\n", socketPath);
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
