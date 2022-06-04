#include "playerapp/playerios/inc/PlayerUISettings.h"
#include "playerapp/playerios/inc/PlayerIOSBaseModel.h"

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
	
	omega::PlayerIOSBaseModel model;
	engine.rootContext()->setContextProperty("playListModel", &model);
	
	omega::PlayerUISettings settings;
	engine.rootContext()->setContextProperty("settings", &settings);
	
	QFile page(":/Resources/frontpage1.qml");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	if (page.open(QIODevice::ReadOnly))
#else
	if(page.open(QIODeviceBase::ReadOnly))
#endif
	{
		engine.load(":/Resources/frontpage1.qml");
		app.exec();
	}
	return 0;
}

//-------------------------------------------------------------------------------------------
