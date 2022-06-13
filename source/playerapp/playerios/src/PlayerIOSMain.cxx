#include "common/inc/CommonFunctions.h"
#include "common/inc/DiskIF.h"
#include "engine/blackomega/inc/MPCodec.h"
#include "engine/silveromega/inc/SilverCodec.h"
#include "engine/whiteomega/inc/WhiteCodec.h"
#include "playerapp/playerios/inc/PlayerUISettings.h"
#include "playerapp/playerios/inc/PlayerIOSBaseModel.h"
#include "playerapp/playerios/inc/PlayerIOSTrackDBManager.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>

using namespace omega;

//-------------------------------------------------------------------------------------------

void setupPlatform()
{
	common::loadSharedLibrary("blackomega");
	common::loadSharedLibrary("blueomega");
	common::loadSharedLibrary("cyanomega");
	common::loadSharedLibrary("greenomega");
	common::loadSharedLibrary("redomega");
	common::loadSharedLibrary("silveromega");
	common::loadSharedLibrary("violetomega");
	common::loadSharedLibrary("whiteomega");
	common::loadSharedLibrary("widget");
#if defined(OMEGA_WIN32)
	CoInitialize(NULL);
#endif
}

//-------------------------------------------------------------------------------------------

void initCodecs()
{
	engine::CodecInitialize::start();
	engine::blackomega::MPCodecInitialize::start();
	engine::silveromega::SilverCodecInitialize::start();
	engine::whiteomega::WhiteCodecInitialize::start();
}

//-------------------------------------------------------------------------------------------

void releaseCodecs()
{
	engine::whiteomega::WhiteCodecInitialize::end();
	engine::silveromega::SilverCodecInitialize::end();
	engine::blackomega::MPCodecInitialize::end();
	engine::CodecInitialize::end();
}

//-------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	
	setupPlatform();
	initCodecs();
	
	common::DiskIFSPtr diskIF = common::DiskIF::instance("disk");
	if(!diskIF.isNull())
	{
		qmlRegisterType<PlayerIOSBaseModel>("uk.co.blackomega", 1, 0, "PlayerIOSBaseModel");
		qmlRegisterType<PlayerUISettings>("uk.co.blackomega", 1, 0, "PlayerUISettings");
		
		PlayerIOSTrackDBManager *trackDBManager = PlayerIOSTrackDBManager::instance();
		if(trackDBManager != 0)
		{	
			QFile page(":/Resources/frontpage1.qml");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
			if (page.open(QIODevice::ReadOnly))
#else
			if(page.open(QIODeviceBase::ReadOnly))
#endif
			{
				PlayerIOSBaseModel model;
				if(model.loadPlaylist())
				{
					engine.rootContext()->setContextProperty("playListModel", &model);
			
					PlayerUISettings settings;
					engine.rootContext()->setContextProperty("settings", &settings);
			
					QObject::connect(trackDBManager, SIGNAL(newtrack(const QString&)), &model, SLOT(appendTrack(const QString&)));
					QObject::connect(trackDBManager, SIGNAL(removetrack(const QString&)), &model, SLOT(deleteTrack(const QString&)));
		
					engine.load(":/Resources/frontpage1.qml");
					app.exec();
				}
			}
			trackDBManager->release();
		}
		common::DiskIF::release();
	}
	releaseCodecs();
	return 0;
}

//-------------------------------------------------------------------------------------------
