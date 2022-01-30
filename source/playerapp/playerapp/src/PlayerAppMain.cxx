#include "network/inc/Resource.h"

#include <QtQml/qqmlapplicationengine.h>
#include <QtGui/qsurfaceformat.h>
#include <QtQml/qqml.h>
#include <QtQml/qqmlcontext.h>

#include "playerapp/playercommon/inc/EmbeddedEnv.h"
#include "playerapp/playerapp/inc/PlayerAppMain.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

PlayerAppMain::PlayerAppMain(int& argc, char **argv) : QGuiApplication(argc, argv),
	OmegaPlaylistInterface(),
	m_pState(0),
	m_pModel(0),
	m_pAudioInterface(0)
{}

//-------------------------------------------------------------------------------------------

PlayerAppMain::~PlayerAppMain()
{
	if(m_pAudioInterface != 0)
	{
		delete m_pAudioInterface;
		m_pAudioInterface = 0;
	}
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

void PlayerAppMain::printError(const char *strR, const char *strE) const
{
	common::Log::g_Log << "PlayerAppMain::" << strR << " - " << strE << common::c_endl;
}

//-------------------------------------------------------------------------------------------

bool PlayerAppMain::initPlaylistManager(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB)
{	
	bool res = false;
	
	m_pAudioInterface = new OmegaAudioIOInterface(this);
	if(m_pAudioInterface->init())
	{
		m_pModel = new PlayListModel(playListDB, m_pAudioInterface, this);
		res = true;
	}
	else
	{
		printError("initPlaylistManager", "Failed to initialise audio layer");
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void PlayerAppMain::playbackTime(quint64 tS)
{
	m_pModel->playbackState()->setTime(tS);
}

//-------------------------------------------------------------------------------------------

PlaybackStateController *PlayerAppMain::getPlaybackState()
{
	return m_pModel->playbackState();
}

//-------------------------------------------------------------------------------------------

PlayListModel *PlayerAppMain::getPlayListModel()
{
	return m_pModel;
}

//-------------------------------------------------------------------------------------------

void PlayerAppMain::onAudioStart(const QString& name)
{
	m_pModel->playbackState()->onAudioStart(name);
}

//-------------------------------------------------------------------------------------------

void PlayerAppMain::onAudioPlay()
{
	m_pModel->playbackState()->onAudioPlay();
}

//-------------------------------------------------------------------------------------------

void PlayerAppMain::onAudioPause()
{
	m_pModel->playbackState()->onAudioPause();
}

//-------------------------------------------------------------------------------------------

void PlayerAppMain::onAudioStop()
{
	m_pModel->playNextItem(false);
}

//-------------------------------------------------------------------------------------------

void PlayerAppMain::onAudioBuffer(tfloat32 percent)
{}

//-------------------------------------------------------------------------------------------

void PlayerAppMain::onAudioReadyForNext()
{
	m_pModel->playNextItem(true);
}

//-------------------------------------------------------------------------------------------

void PlayerAppMain::onAudioNoNext()
{}

//-------------------------------------------------------------------------------------------

void PlayerAppMain::onAudioCrossfade()
{}

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
		PlayerAppMain app(argc, argv);
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

			if(app.initPlaylistManager(playListDB))
			{
				qmlRegisterType<PlayListModel>("uk.co.blackomega", 1, 0, "PlayListModel");
				qmlRegisterType<PlaybackStateController>("uk.co.blackomega", 1, 0, "PlaybackStateController");

				engine.rootContext()->setContextProperty("playListModel", app.getPlayListModel());
				engine.rootContext()->setContextProperty("playbackStateController", app.getPlaybackState());

				engine.load(QUrl("qrc:/Resources/playlist.qml"));

				res = app.exec();
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
