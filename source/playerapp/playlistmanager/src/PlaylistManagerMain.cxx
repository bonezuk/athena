#include "network/inc/Resource.h"

#include <QtQml/qqmlapplicationengine.h>
#include <QtGui/qsurfaceformat.h>
#include <QtQml/qqml.h>
#include <QtQml/qqmlcontext.h>

#include "playerapp/playercommon/inc/EmbeddedEnv.h"
#include "playerapp/playlistmanager/inc/PlaylistManagerMain.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlaylistManagerApp::PlaylistManagerApp(int& argc, char **argv) : QGuiApplication(argc, argv),
	m_pModel(),
	m_pPLInterface()
{}

//-------------------------------------------------------------------------------------------

PlaylistManagerApp::~PlaylistManagerApp()
{}

//-------------------------------------------------------------------------------------------

void PlaylistManagerApp::initPlaylistManager(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB)
{	
	m_pPLInterface = QSharedPointer<OmegaPlaylistInterface>(new OmegaPlaylistInterface());
	QSharedPointer<OmegaAudioInterface> pAInterface(new OmegaAudioBusInterface());
	m_pModel = QSharedPointer<PlayListWebModel>(new PlayListWebModel(playListDB, pAInterface));
	m_pModel->initialise();
	QSharedPointer<PlayListModel> pModelBase = m_pModel.dynamicCast<PlayListModel>();
	m_pPLInterface->init(pModelBase);
}

//-------------------------------------------------------------------------------------------

QSharedPointer<PlaybackStateController>& PlaylistManagerApp::getPlaybackState()
{
	return m_pModel->playbackState();
}

//-------------------------------------------------------------------------------------------

QSharedPointer<PlayListWebModel>& PlaylistManagerApp::getPlayListModel()
{
	return m_pModel;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<OmegaPlaylistInterface>& PlaylistManagerApp::getPlayListInterface()
{
	return m_pPLInterface;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<OmegaWebInterface> PlaylistManagerApp::getWebInterface()
{
	QSharedPointer<OmegaWebInterface> pWeb = m_pModel.dynamicCast<OmegaWebInterface>();
	return pWeb;
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------

using namespace orcus;

//-------------------------------------------------------------------------------------------

QStringList listFromArguements(int argc, char **argv)
{
	QStringList args;
	
	for(int i = 0; i < argc; i++)
	{
		args.append(QString::fromUtf8(argv[i]));
	}
	return args;
}

//-------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	QStringList args = listFromArguements(argc, argv);
	int res = -1;
	
	setupEnviroment(argv[0]);
	initCodecs();

	QString trackDBFilename = userApplicationDataDirectory() + "track_playlist_dev.db";
	track::db::TrackDB *trackDB = track::db::TrackDB::instance(trackDBFilename);
	if(trackDB != 0)
	{
		QSharedPointer<PlaylistManagerApp> app(new PlaylistManagerApp(argc, argv));
		QString mountPoint;
		QVector<QPair<track::db::DBInfoSPtr,tint> > playListDB;
		
		mountPoint = mountPointFromArguments(args);
		if(!mountPoint.isEmpty())
		{
			if(trackDB->mountPoints()->addMountPoint(mountPoint))
			{
				res = 0;
			}
			else
			{
				common::Log::g_Log << "Failed to define mount point at '" << mountPoint << "'" << common::c_endl;
			}
		}
		else if(loadPlaylistFromDBOrArguments(args, playListDB))
		{
			QQmlApplicationEngine engine;

			app->initPlaylistManager(playListDB);

			qmlRegisterType<PlayListModel>("uk.co.blackomega", 1, 0, "PlayListModel");
			qmlRegisterType<PlaybackStateController>("uk.co.blackomega", 1, 0, "PlaybackStateController");

			engine.rootContext()->setContextProperty("playListModel", app->getPlayListModel().data());
			engine.rootContext()->setContextProperty("playbackStateController", app->getPlaybackState().data());

			engine.load(QUrl("qrc:/Resources/playlist.qml"));
			
			OmegaPLService *plService = new OmegaPLService(app->getPlayListInterface());
			if(plService->start())
			{
				QSharedPointer<OmegaWebInterface> pWeb = app->getWebInterface();
				OmegaPLWebService *plWebService = new OmegaPLWebService(pWeb);
				if(plWebService->start())
				{
					res = app->exec();
					plWebService->stop();
				}
				else
				{
					common::Log::g_Log << "Failed to start Web IPC service" << common::c_endl;
				}
				plService->stop();
			}
			else
			{
				common::Log::g_Log << "Failed to start IPC service" << common::c_endl;
			}
			delete plService;
    	}
    	delete trackDB;
    }
    else
    {
		common::Log::g_Log << "Failed to create track database file '" << trackDBFilename << "'" << common::c_endl;
    }
    
    releaseCodecs();

    return res;
}

//-------------------------------------------------------------------------------------------
