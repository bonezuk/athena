#include "player/inc/Player.h"
#include "engine/inc/Codec.h"
#include "engine/blackomega/inc/MPCodec.h"
#include "engine/silveromega/inc/SilverCodec.h"
#include "engine/whiteomega/inc/WhiteCodec.h"
#include "track/db/inc/TrackDB.h"
#include "common/inc/DiskOps.h"
#include "common/inc/HardwareIdentification.h"

#if defined(Q_OS_MAC)
#include "player/inc/CocoaInitializer.h"
#endif

#include "common/inc/SBService.h"
#include "player/inc/PlayerController.h"
#include "common/inc/ProductVersionInfo.h"
#include "dlna/inc/XMLLibIF.h"
#include "dlna/inc/DiskIF.h"
#include "player/inc/QPlayerApplication.h"

#if defined(Q_OS_MAC)
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

#if defined(OMEGA_MAC_STORE)
#include "track/db/inc/SBBookmarkTrackDB.h"
#include "common/inc/Bundle.h"
#endif

//-------------------------------------------------------------------------------------------

QString userApplicationDataDirectory()
{
	return orcus::common::SBService::applicationDataDirectory();
}

//-------------------------------------------------------------------------------------------
#if defined(ORCUS_WIN32)
//-------------------------------------------------------------------------------------------

bool processCLI(int argc,char **argv)
{
	orcus::player::CLIPipe *cliPipe = orcus::player::CLIPipe::instance();
	bool res = false;

#if defined(OMEGA_DEBUG)
	QString cmdLine = "Command Line : ";
	for(tint j=0;j<argc;j++)
	{
		cmdLine += QString::fromUtf8(argv[j]) + " ";
	}
	orcus::common::Log::g_Log << cmdLine.toUtf8().constData() << orcus::common::c_endl;
#endif

	if(cliPipe!=0)
	{
		tint i;
		QStringList cmdList;

		for(i=1;i<argc;i++)
		{
			QString aStr = QString::fromUtf8(argv[i]);
			cmdList.append(aStr);
		}
		cliPipe->cli(cmdList);
		res = cliPipe->isServer();
	}
	return res;
}

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

#if defined(ORCUS_WIN32)
int CALLBACK WinMain(HINSTANCE ,HINSTANCE ,LPSTR ,int)
{
	int argc = 0;
	char **argv = 0;
	LPWSTR *szArgList;

	szArgList = CommandLineToArgvW(GetCommandLineW(),&argc);
	if(szArgList!=0)
	{
		argv = reinterpret_cast<char **>(malloc(argc * sizeof(char *)));
		for(tint i=0;i<argc;i++)
		{
			char *cmdArg;
			QString s = QString::fromUtf16(reinterpret_cast<unsigned short *>(szArgList[i]));
			QByteArray oArg = s.toUtf8();
			argv[i] = cmdArg = new char[oArg.size() + 1];
			memcpy(cmdArg,oArg.data(),oArg.size());
			cmdArg[oArg.size()] = '\0';
		}
		LocalFree(szArgList);
	}

#else
int main(int argc,char **argv)
{
#endif

#if defined(OMEGA_MAC_STORE)
	QString settingPath = userApplicationDataDirectory();
	QSettings::setPath(QSettings::IniFormat,QSettings::UserScope,settingPath);
	QSettings::setPath(QSettings::NativeFormat,QSettings::UserScope,settingPath);
		
	QCoreApplication::setOrganizationName("Stuart MacLean");
	QCoreApplication::setOrganizationDomain("www.blackomega.co.uk");
	QCoreApplication::setApplicationName("Black Omega");
#else
	QCoreApplication::setOrganizationName("Tiger-Eye");
	QCoreApplication::setOrganizationDomain("www.blackomega.co.uk");
	QCoreApplication::setApplicationName("BlackOmega2");
#endif

#if defined(OMEGA_DEBUG)
	QString logFilename = userApplicationDataDirectory() + "log_debug.txt";
#else
	QString logFilename = userApplicationDataDirectory() + "log.txt";
#endif
	orcus::common::Log::g_Log.set(logFilename.toUtf8().constData());

#if defined(ORCUS_WIN32)
	_putenv_s("QT_DEVICE_PIXEL_RATIO","auto");

	if(processCLI(argc,argv))
	{
#endif
		orcus::common::HardwareIdentification::instance();

#if defined(ORCUS_WIN32)
		LoadLibraryA("widget.dll");
		LoadLibraryA("blueomega.dll");
		LoadLibraryA("silveromega.dll");
		LoadLibraryA("blackomega.dll");
		LoadLibraryA("whiteomega.dll");
		LoadLibraryA("redomega.dll");
		LoadLibraryA("greenomega.dll");
		LoadLibraryA("violetomega.dll");
		LoadLibraryA("cyanomega.dll");
		LoadLibraryA("monkeyomega.dll");
		LoadLibraryA("wavpackomega.dll");
		LoadLibraryA("toneomega.dll");
		LoadLibraryA("rtp_silveromega.dll");
#endif

#if defined(Q_OS_MAC)
		{
			struct rlimit limit;
			int err;

			err = getrlimit(RLIMIT_NOFILE, &limit);
			if(err==0)
			{
				limit.rlim_cur = (OPEN_MAX < limit.rlim_max) ? OPEN_MAX : limit.rlim_max;
				err = setrlimit(RLIMIT_NOFILE,&limit);
			}
		}
#endif

		orcus::dlna::XMLLibIF::instance("xml");
		orcus::dlna::DiskIF::instance("disk");

#if defined(OMEGA_MACOSX)
		orcus::player::CocoaInitializer cocoaMemPool;
#endif

#if defined(Q_OS_MAC)
        QString pluginDir;
#if defined(OMEGA_MAC_STORE)
		pluginDir = orcus::common::Bundle::pluginDirectory();
		orcus::common::Log::g_Log.print("%s\n",pluginDir.toUtf8().constData());
#else
        QFileInfo appFile(argv[0]);
        QDir d = appFile.absolutePath();
#if defined(BUNDLE_PLUGINS)
        d.cdUp();
#endif
        d.cd("Plugins");
        pluginDir = d.absolutePath();
		orcus::common::Log::g_Log.print("%s\n",pluginDir.toUtf8().constData());
#endif
        QApplication::setLibraryPaths(QStringList(pluginDir));
#else
        QFileInfo appFile(argv[0]);
        QDir d = appFile.absolutePath();
        QString pluginDir;
		d.cdUp();
        d.cd("plugins");
        pluginDir = d.absolutePath();
		pluginDir = QDir::toNativeSeparators(pluginDir);
        QApplication::setLibraryPaths(QStringList(pluginDir));
#endif

		orcus::engine::CodecInitialize::start();
		orcus::engine::blackomega::MPCodecInitialize::start();
		orcus::engine::silveromega::SilverCodecInitialize::start();
		orcus::engine::whiteomega::WhiteCodecInitialize::start();

#if defined(OMEGA_DEBUG)
		QString trackDBFilename = userApplicationDataDirectory() + "track_debug.db";
#else
		QString trackDBFilename = userApplicationDataDirectory() + "track.db";
#endif
		orcus::track::db::TrackDB *trackDB = orcus::track::db::TrackDB::instance(trackDBFilename);
		if(trackDB!=0)
		{
			if(trackDB->dbVersion()<4)
			{
				delete trackDB;	
				orcus::common::DiskOps::remove(trackDBFilename);
				trackDB = orcus::track::db::TrackDB::instance(trackDBFilename);
			}
			else if(trackDB->dbVersion()==4)
			{
				delete trackDB;	
				
				if(orcus::track::db::Schema::upgradeVersion4To5(trackDBFilename))
				{
					trackDB = orcus::track::db::TrackDB::instance(trackDBFilename);
				}
				else
				{
					orcus::common::DiskOps::remove(trackDBFilename);
					trackDB = orcus::track::db::TrackDB::instance(trackDBFilename);					
				}
			}
		}

		orcus::common::ProductVersionInfo::instance(":/build/Resources/buildInfo.xml");

		if(trackDB!=0)
		{
#if defined(OMEGA_MAC_STORE)
			orcus::track::db::SBBookmarkService::instance();
#endif

			{
				QSettings wSettings;
				wSettings.beginGroup("appearance");
				wSettings.setValue("frame",QVariant(true));
				wSettings.endGroup();
			}

			{
				QFile confFile(":/qt/etc/qt.conf");
				if(confFile.open(QIODevice::ReadOnly))
				{
					QByteArray confArr = confFile.readAll();
					QString confData = QString::fromLatin1(confArr.data(),confArr.size());
					orcus::common::Log::g_Log.print("%s\n",confData.toLatin1().constData());
				}
			}

			{
				orcus::player::QPlayerApplication app(argc, argv);

#if defined(ORCUS_WIN32)
				QPixmap splashPixmap(":/splash/Resources/splash.png");
				QSplashScreen splash(splashPixmap);
				splash.show();
#endif

				orcus::player::PlayerController *pCtrl = orcus::player::PlayerController::instance();

#if defined(ORCUS_WIN32)
				pCtrl->setStartupSplashScreen(splash);
#endif

			    app.exec();
			    delete pCtrl;
			}

#if defined(OMEGA_MAC_STORE)
			orcus::track::db::SBBookmarkService::release();
#endif
		}
		delete trackDB;

		orcus::engine::whiteomega::WhiteCodecInitialize::end();
		orcus::engine::silveromega::SilverCodecInitialize::end();
		orcus::engine::blackomega::MPCodecInitialize::end();
		orcus::engine::CodecInitialize::end();

#if defined(ORCUS_WIN32)
		orcus::player::CLIPipe *cliPipe = orcus::player::CLIPipe::instance();
		if(cliPipe!=0)
		{
			delete cliPipe;
		}

		orcus::dlna::DiskIF::release();
		orcus::dlna::XMLLibIF::release();
	}
#endif

	return 0;
}

//-------------------------------------------------------------------------------------------
