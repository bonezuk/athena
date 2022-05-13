#include "playerapp/playerios/inc/PlayerIOSBaseModel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
//-------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;
	
	qmlRegisterType<orcus::PlayerIOSBaseModel>("uk.co.blackomega", 1, 0, "PlayerIOSBaseModel");
	
	orcus::PlayerIOSBaseModel model;
	engine.rootContext()->setContextProperty("playListModel", &model);
	
	QFile page(":/Resources/frontpage1.qml");
	if(page.open(QIODeviceBase::ReadOnly))
	{
		engine.load(":/Resources/frontpage1.qml");
		app.exec();
	}
	return 0;
}

//-------------------------------------------------------------------------------------------
