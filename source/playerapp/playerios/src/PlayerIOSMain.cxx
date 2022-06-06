#include "playerapp/playerios/inc/PlayerUISettings.h"
#include "playerapp/playerios/inc/PlayerIOSBaseModel.h"
#include "playerapp/playerios/inc/PlayerIOSTrackDBManager.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>

//-------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	
	qmlRegisterType<omega::PlayerIOSBaseModel>("uk.co.blackomega", 1, 0, "PlayerIOSBaseModel");
	qmlRegisterType<omega::PlayerUISettings>("uk.co.blackomega", 1, 0, "PlayerUISettings");
		
	omega::PlayerIOSTrackDBManager *trackDBManager = omega::PlayerIOSTrackDBManager::instance();
	if(trackDBManager != 0)
	{	
		QFile page(":/Resources/frontpage1.qml");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
		if (page.open(QIODevice::ReadOnly))
#else
		if(page.open(QIODeviceBase::ReadOnly))
#endif
		{
			omega::PlayerIOSBaseModel model;
			engine.rootContext()->setContextProperty("playListModel", &model);
	
			omega::PlayerUISettings settings;
			engine.rootContext()->setContextProperty("settings", &settings);
			
			QObject::connect(trackDBManager, SIGNAL(newtrack(const QString&)), &model, SLOT(appendTrack(const QString&)));

			engine.load(":/Resources/frontpage1.qml");
			app.exec();
		}
		trackDBManager->release();
	}
	return 0;
}

//-------------------------------------------------------------------------------------------
