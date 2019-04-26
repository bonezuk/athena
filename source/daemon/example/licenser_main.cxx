#include "licenser/licenselib/inc/Licenser.h"

//-------------------------------------------------------------------------------------------

orcus::license::Licenser *gLicenser = 0;

//-------------------------------------------------------------------------------------------
#if defined(OMEGA_WIN32)
//-------------------------------------------------------------------------------------------

BOOL WINAPI ctrlHandler(DWORD dwCtrlType)
{
	if(gLicenser!=0)
	{
		gLicenser->quit();
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
	QCoreApplication::setApplicationName("BlackOmega2_LicenseServer");

	orcus::common::Log::g_Log << "Start Black Omega license server\r\n";
	{
		gLicenser = new orcus::license::Licenser(argc,argv);
		gLicenser->exec();
	}
	orcus::common::Log::g_Log << "Stop Black Omega license server\r\n";
	return 0;
}

//-------------------------------------------------------------------------------------------
