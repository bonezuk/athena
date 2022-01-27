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

PlaylistManagerApp::PlaylistManagerApp(int argc, char **argv) : QGuiApplication(argc, argv),
	OmegaPlaylistInterface()
{}

//-------------------------------------------------------------------------------------------

PlaylistManagerApp::~PlaylistManagerApp()
{
	if(m_pModel != 0)
	{
		delete m_pModel;
		m_pModel = 0;
	}
	if(m_pAudioInterface != 0)
	{
		delete m_pAudioInterface;
		m_pAudioInterface = 0;
	}
}

//-------------------------------------------------------------------------------------------

void PlaylistManagerApp::initPlaylistManager(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB)
{	
	m_pAudioInterface = new OmegaAudioBusInterface(this);
	m_pModel = new PlayListModel(playListDB, m_pAudioInterface, this);	
}

//-------------------------------------------------------------------------------------------

void PlaylistManagerApp::playbackTime(quint64 tS)
{
	m_pModel->playbackState()->setPlaybackTime(tS);
}

//-------------------------------------------------------------------------------------------

PlaybackState *PlaylistManagerApp::getPlaybackState()
{
	return m_pModel->playbackState();
}

//-------------------------------------------------------------------------------------------

PlayListModel *PlaylistManagerApp::getPlayListModel()
{
	return m_pModel;
}

//-------------------------------------------------------------------------------------------

void PlaylistManagerApp::onAudioStart(const QString& name)
{
	m_pModel->playbackState()->onAudioStart(name);
}

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------

using namespace orcus;

//-------------------------------------------------------------------------------------------

int runPlayerQMLApp(PlaylistManagerApp *pApp, QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB)
{
	int res = -1;
	
	pApp->initPlaylistManager(playListDB);
	
	qmlRegisterType<PlayListModel>("uk.co.blackomega", 1, 0, "PlayListModel");
	qmlRegisterType<PlaybackState>("uk.co.blackomega", 1, 0, "PlaybackState");
	
#if defined(OMEGA_LINUX)
	QDBusConnection bus = QDBusConnection::systemBus();
#else
	QDBusConnection bus = QDBusConnection::sessionBus();
#endif
	if(bus.isConnected())
	{
		QQmlApplicationEngine engine;

		engine.rootContext()->setContextProperty("playListModel", pApp->getPlayListModel());
		engine.rootContext()->setContextProperty("playbackState", pApp->getPlaybackState());
		engine.load(QUrl("qrc:/Resources/playlist.qml"));

		QObject plIFaceObj;
		OmegaPLDBusAdaptor *pIFace = new OmegaPLDBusAdaptor(pApp, &plIFaceObj);
		bus.registerObject("/", &plIFaceObj);
		if(bus.registerService(OMEGAPLAYLISTMANAGER_SERVICE_NAME))
		{
			res = pApp->exec();
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
	return res;
}

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
		PlaylistManagerApp app(argc, argv);
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
			res = runPlayerQMLApp(&app, playListDB);
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
