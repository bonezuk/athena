#include "playerapp/webservice/inc/OmegaWebServerMain.h"
#include "playerapp/playercommon/inc/EmbeddedEnv.h"

#include <QFileInfo>
#include <QDir>

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

OmegaWebServerMain::OmegaWebServerMain(const QString& rootDir, int& argc, char **argv)
{
	m_webService = new OmegaWebServiceWS(rootDir;)
}

//-------------------------------------------------------------------------------------------

OmegaWebServerMain::~OmegaWebServerMain()
{
	if(m_webService != 0)
	{
		delete m_webService;
		m_webService = 0;
	}
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------

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
	
	OmegaWebServerMain app(rootWWW, argc, argv);
	app.exec();
	
	return 0;
}

//-------------------------------------------------------------------------------------------
