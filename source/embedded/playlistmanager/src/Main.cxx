#include "embedded/playlistmanager/inc/SortFilterProxyModel.h"
#include <QtQml/qqmlapplicationengine.h>
#include <QtGui/qsurfaceformat.h>
#include <QtGui/qguiapplication.h>
#include <QtQml/qqml.h>
#include <QFileInfo>
#include <QDir>

#include "common/inc/SBService.h"
#include "common/inc/CommonFunctions.h"
#include "network/inc/Resource.h"
#include "track/db/inc/TrackDB.h"
#include "engine/inc/Codec.h"
#include "engine/blackomega/inc/MPCodec.h"
#include "engine/silveromega/inc/SilverCodec.h"
#include "engine/whiteomega/inc/WhiteCodec.h"

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
#elif defined(Q_OS_WIN)
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
	::orcus::common::loadSharedLibrary("blackomega");
	::orcus::common::loadSharedLibrary("blueomega");
	::orcus::common::loadSharedLibrary("cyanomega");
	::orcus::common::loadSharedLibrary("greenomega");
	::orcus::common::loadSharedLibrary("redomega");
	::orcus::common::loadSharedLibrary("silveromega");
	::orcus::common::loadSharedLibrary("toneomega");
	::orcus::common::loadSharedLibrary("violetomega");
	::orcus::common::loadSharedLibrary("wavpackomega");
	::orcus::common::loadSharedLibrary("whiteomega");
	::orcus::common::loadSharedLibrary("widget");
#if defined(ORCUS_WIN32)
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

QString userApplicationDataDirectory()
{
	return orcus::common::SBService::applicationDataDirectory();
}

//-------------------------------------------------------------------------------------------

QString mountPointFromArguments()
{
	int idx, state;
	QString name;
	QStringList args = QCoreApplication::arguments();
	
	for(idx = 0, state = 0; idx < args.size() && name.isEmpty(); idx++)
	{
		if(args.at(idx) == "--add-mount")
		{
			state = 1;
		}
		else if(state == 1)
		{
			name = args.at(idx);
		}
	}
	return name;
}

//-------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	int res = -1;
	
	setupEnviroment(argv[0]);
	
	orcus::engine::CodecInitialize::start();
	orcus::engine::blackomega::MPCodecInitialize::start();
	orcus::engine::silveromega::SilverCodecInitialize::start();
	orcus::engine::whiteomega::WhiteCodecInitialize::start();

	QString trackDBFilename = userApplicationDataDirectory() + "track_playlist_dev.db";
	orcus::track::db::TrackDB *trackDB = orcus::track::db::TrackDB::instance(trackDBFilename);
	if(trackDB != 0)
	{
		QString mountPoint;
		QGuiApplication app(argc, argv);
		
		mountPoint = mountPointFromArguments();
		if(!mountPoint.isEmpty())
		{
			if(trackDB->mountPoints()->addMountPoint(mountPoint))
			{
				res = 0;
			}
			else
			{
				orcus::common::Log::g_Log << "Failed to define mount point at '" << mountPoint << "'" << orcus::common::c_endl;
			}
		}
		else
		{
	    	if (QCoreApplication::arguments().contains(QLatin1String("--coreprofile"))) 
    		{
        		QSurfaceFormat fmt;
	        	fmt.setVersion(4, 4);
	    	    fmt.setProfile(QSurfaceFormat::CoreProfile);
    	    	QSurfaceFormat::setDefaultFormat(fmt);
	    	}
	    	qmlRegisterType<SortFilterProxyModel>("org.qtproject.example", 1, 0, "SortFilterProxyModel");
		    QQmlApplicationEngine engine(QUrl("qrc:/Resources/main.qml"));
    		res = app.exec();
    	}
    	delete trackDB;
    }
    else
    {
		orcus::common::Log::g_Log << "Failed to create track database file '" << trackDBFilename << "'" << orcus::common::c_endl;
    }

	orcus::engine::whiteomega::WhiteCodecInitialize::end();
	orcus::engine::silveromega::SilverCodecInitialize::end();
	orcus::engine::blackomega::MPCodecInitialize::end();
	orcus::engine::CodecInitialize::end();

    return res;
}

//-------------------------------------------------------------------------------------------
