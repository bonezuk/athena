#include "network/inc/Resource.h"

#include <QtQml/qqmlapplicationengine.h>
#include <QtGui/qsurfaceformat.h>
#include <QtQml/qqml.h>
#include <QtQml/qqmlcontext.h>

#if defined(ORCUS_WIN32)
#include "audioio/inc/WasAPIIF.h"
#endif

#include "playerapp/playercommon/inc/EmbeddedEnv.h"
#include "playerapp/playerapp/inc/PlayerAppMain.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
// PlayerAppMain
//-------------------------------------------------------------------------------------------

PlayerAppMain::PlayerAppMain(int& argc, char **argv) : QGuiApplication(argc, argv),
	m_pModel(),
	m_pAudioInterface()
{}

//-------------------------------------------------------------------------------------------

PlayerAppMain::~PlayerAppMain()
{
	if(!m_pModel.isNull())
	{
		m_pModel.clear();
	}
	if(!m_pAudioInterface.isNull())
	{
		m_pAudioInterface.clear();
	}
	if(!m_pPLInterface.isNull())
	{
		m_pPLInterface.clear();
	}
}

//-------------------------------------------------------------------------------------------

void PlayerAppMain::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "PlayerAppMain::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool PlayerAppMain::initPlaylistManager(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB)
{	
	QSharedPointer<OmegaPlaylistInterface> plInterface(new OmegaPlaylistInterface());
	bool res = false;
	
	m_pAudioInterface = QSharedPointer<OmegaAudioIOInterface>(new OmegaAudioIOInterface(plInterface));
	QSharedPointer<OmegaAudioInterface> pAInterface = m_pAudioInterface.dynamicCast<OmegaAudioInterface>();
	m_pModel = QSharedPointer<PlayListModel>(new PlayListModel(playListDB, pAInterface));
	m_pModel->initialise();
	plInterface->init(m_pModel);
	if(m_pAudioInterface->init())
	{
		res = true;
	}
	else
	{
		printError("initPlaylistManager", "Failed to initialise audio layer");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<PlaybackStateController>& PlayerAppMain::getPlaybackState()
{
	return m_pModel->playbackState();
}

//-------------------------------------------------------------------------------------------

QSharedPointer<PlayListModel>& PlayerAppMain::getPlayListModel()
{
	return m_pModel;
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

#if defined(OMEGA_WIN32)
	orcus::audioio::WasAPIIF::instance("wasapi");
#endif

	QString trackDBFilename = userApplicationDataDirectory() + "track_playlist_dev.db";
	track::db::TrackDB *trackDB = track::db::TrackDB::instance(trackDBFilename);
	if(trackDB != 0)
	{
		QSharedPointer<PlayerAppMain> app(new PlayerAppMain (argc, argv));
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

			if(app->initPlaylistManager(playListDB))
			{
				qmlRegisterType<PlayListModel>("uk.co.blackomega", 1, 0, "PlayListModel");
				qmlRegisterType<PlaybackStateController>("uk.co.blackomega", 1, 0, "PlaybackStateController");

				engine.rootContext()->setContextProperty("playListModel", app->getPlayListModel().data());
				engine.rootContext()->setContextProperty("playbackStateController", app->getPlaybackState().data());

				engine.load(QUrl("qrc:/Resources/playlist.qml"));

				res = app->exec();
			}
			else
			{
				common::Log::g_Log << "Application initialisation FAILED!!!" << common::c_endl;
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