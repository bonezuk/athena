#include "daemon/inc/MusicDaemon.h"

//-------------------------------------------------------------------------------------------

orcus::daemon::MusicDaemon *g_PhoenixDaemon = 0;

//-------------------------------------------------------------------------------------------
#if defined(OMEGA_WIN32)
//-------------------------------------------------------------------------------------------

BOOL WINAPI ctrlHandler(DWORD dwCtrlType)
{
	if(g_PhoenixDaemon!=0)
	{
		g_PhoenixDaemon->quit();
	}
	return TRUE;
}

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

int main(int argc,char **argv)
{
#if defined(OMEGA_WIN32)
	::SetConsoleCtrlHandler(ctrlHandler,TRUE);
#endif

	QCoreApplication::setOrganizationName("Tiger-Eye");
	QCoreApplication::setOrganizationDomain("www.blackomega.co.uk");
	QCoreApplication::setApplicationName("PhoenixMusicDaemon");

	orcus::common::Log::g_Log << "Start Black Omega Phoenix music server\r\n";
	{
		g_PhoenixDaemon = new orcus::daemon::MusicDaemon(argc,argv);
		g_PhoenixDaemon->exec();
	}
	orcus::common::Log::g_Log << "Stop Black Omega Phoenix music server\r\n";
	return 0;
}

//-------------------------------------------------------------------------------------------
