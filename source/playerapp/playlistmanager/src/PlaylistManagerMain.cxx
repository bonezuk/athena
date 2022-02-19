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
	m_pAudioInterface(),
	m_pPLInterface()
{}

//-------------------------------------------------------------------------------------------

PlaylistManagerApp::~PlaylistManagerApp()
{}

//-------------------------------------------------------------------------------------------

void PlaylistManagerApp::initPlaylistManager(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB)
{	
	m_pPLInterface = QSharedPointer<OmegaPlaylistInterface>(new OmegaPlaylistInterface(this));
	m_pAudioInterface = QSharedPointer<OmegaAudioBusInterface>(new OmegaAudioBusInterface(this));
	QSharedPointer<OmegaAudioInterface> pAInterface = m_pAudioInterface.dynamicCast<OmegaAudioInterface>();
	m_pModel = QSharedPointer<PlayListModel>(new PlayListModel(playListDB, pAInterface, this));
	m_pPLInterface->init(m_pModel);
}

//-------------------------------------------------------------------------------------------

QSharedPointer<PlaybackStateController>& PlaylistManagerApp::getPlaybackState()
{
	return m_pModel->playbackState();
}

//-------------------------------------------------------------------------------------------

QSharedPointer<PlayListModel>& PlaylistManagerApp::getPlayListModel()
{
	return m_pModel;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<OmegaPlaylistInterface>& PlaylistManagerApp::getPlayListInterface()
{
	return m_pPLInterface;
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

#if defined(OMEGA_LINUX)
			QDBusConnection bus = QDBusConnection::systemBus();
#else
			QDBusConnection bus = QDBusConnection::sessionBus();
#endif
			if(bus.isConnected())
			{
				QObject plIFaceObj;
				OmegaPLDBusAdaptor *pIFace = new OmegaPLDBusAdaptor(app->getPlayListInterface(), &plIFaceObj);
				bus.registerObject("/", &plIFaceObj);
				if(bus.registerService(OMEGAPLAYLISTMANAGER_SERVICE_NAME))
				{
					res = app->exec();
				}
				else
				{
					common::Log::g_Log << "Failed to present register dbus service for Playlist Manager." << common::c_endl;
					common::Log::g_Log << qPrintable(bus.lastError().message()) << common::c_endl;
				}
			}
			else
			{
				common::Log::g_Log << "Failed to connect to session D-Bus" << common::c_endl;
				common::Log::g_Log << qPrintable(bus.lastError().message()) << common::c_endl;
			}
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
