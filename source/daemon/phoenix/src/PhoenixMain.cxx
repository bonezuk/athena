#include "player/oplayer/inc/OPlayerMain.h"
#include "engine/blackomega/inc/MPCodec.h"
#include "engine/silveromega/inc/SilverCodec.h"
#include "engine/whiteomega/inc/WhiteCodec.h"
#include "track/info/inc/Info.h"
#include "common/inc/BIOBufferedStream.h"
#if defined(ORCUS_WIN32)
#include "audioio/inc/WasAPIIF.h"
#endif
#include "daemon/inc/MusicDaemon.h"

//-------------------------------------------------------------------------------------------

orcus::daemon::MusicDaemon *g_PhoenixDaemon = 0;

//-------------------------------------------------------------------------------------------

void setPluginLocation(const char *appPath)
{
#if defined(Q_OS_MAC)
    QFileInfo appFile(appPath);
    QDir d = appFile.absolutePath();
	QString pluginDir;
#if defined(BUNDLE_PLUGINS)
	d.cdUp();
#endif
	d.cd("Plugins");
	pluginDir = d.absolutePath();
	QCoreApplication::setLibraryPaths(QStringList(pluginDir));
#else
	QFileInfo appFile(appPath);
	QDir d = appFile.absolutePath();
	QString pluginDir;
	d.cdUp();
	d.cd("plugins");
	pluginDir = d.absolutePath();
	QCoreApplication::setLibraryPaths(QStringList(pluginDir));
#endif
}

//-------------------------------------------------------------------------------------------

void setupPlatform()
{
#if defined(ORCUS_WIN32)
	LoadLibraryA("blackomega.dll");
	LoadLibraryA("blueomega.dll");
	LoadLibraryA("cyanomega.dll");
	LoadLibraryA("greenomega.dll");
	LoadLibraryA("monkeyomega.dll");
	LoadLibraryA("redomega.dll");
	LoadLibraryA("toneomega.dll");
	LoadLibraryA("silveromega.dll");
	LoadLibraryA("violetomega.dll");
	LoadLibraryA("wavpackomega.dll");
	LoadLibraryA("whiteomega.dll");
	LoadLibraryA("widget.dll");
    CoInitialize(NULL);
#endif
}

//-------------------------------------------------------------------------------------------

void setupSettingsPath()
{
	QCoreApplication::setOrganizationName("Tiger-Eye");
	QCoreApplication::setOrganizationDomain("www.blackomega.co.uk");
	QCoreApplication::setApplicationName("BlackOmega2");
}

//-------------------------------------------------------------------------------------------

void setupEnviroment(const char *appPath)
{
	::orcus::network::Resource::instance();
	setPluginLocation(appPath);
	setupPlatform();
	setupSettingsPath();    
}

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
	int res = 0;

	setupEnviroment(argv[0]);

#if defined(OMEGA_WIN32)
	::SetConsoleCtrlHandler(ctrlHandler,TRUE);
	orcus::audioio::WasAPIIF::instance("wasapi");
#endif

    orcus::engine::CodecInitialize::start();
    orcus::engine::blackomega::MPCodecInitialize::start();
    orcus::engine::silveromega::SilverCodecInitialize::start();
    orcus::engine::whiteomega::WhiteCodecInitialize::start();

	orcus::common::Log::g_Log << "Start Black Omega Phoenix music server\r\n";
	{
		g_PhoenixDaemon = new orcus::daemon::MusicDaemon(argc,argv);
		g_PhoenixDaemon->exec();
		delete g_PhoenixDaemon;
		g_PhoenixDaemon = 0;
	}
	orcus::common::Log::g_Log << "Stop Black Omega Phoenix music server\r\n";
	
    orcus::engine::whiteomega::WhiteCodecInitialize::end();
    orcus::engine::silveromega::SilverCodecInitialize::end();
    orcus::engine::blackomega::MPCodecInitialize::end();
    orcus::engine::CodecInitialize::end();

#if defined(ORCUS_WIN32)
	orcus::audioio::WasAPIIF::release();
#endif

	return res;
}

//-------------------------------------------------------------------------------------------
