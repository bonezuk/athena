#include "playerapp/webservice/inc/OmegaWebService.h"
#include "playerapp/playercommon/inc/EmbeddedEnv.h"

#include <QFileInfo>
#include <QDir>

using namespace orcus;

//-------------------------------------------------------------------------------------------

QString pathToRootWWWDirectory(char *appPath)
{
	QFileInfo appFile(appPath);
	QDir d = appFile.absolutePath();
	d.cdUp();
	d.cd("www");
	return d.absolutePath();
}

//-------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	QString rootWWW = pathToRootWWWDirectory(argv[0]);
	setupEnviroment(argv[0]);
	
	OmegaWebService app(rootWWW, argc, argv);
	app.exec();
	
	return 0;
}

//-------------------------------------------------------------------------------------------
