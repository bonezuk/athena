#include "network/inc/Resource.h"

#include <QtQml/qqmlapplicationengine.h>
#include <QtGui/qsurfaceformat.h>
#include <QtGui/qguiapplication.h>
#include <QtQml/qqml.h>
#include <QtQml/qqmlcontext.h>
#include <QFileInfo>
#include <QDir>

#include "common/inc/SBService.h"
#include "common/inc/CommonFunctions.h"
#include "track/db/inc/TrackDB.h"
#include "track/db/inc/DBInfo.h"
#include "track/db/inc/PlaylistAbstractIO.h"
#include "engine/inc/Codec.h"
#include "engine/blackomega/inc/MPCodec.h"
#include "engine/silveromega/inc/SilverCodec.h"
#include "engine/whiteomega/inc/WhiteCodec.h"

#include "embedded/playlistmanager/inc/CLIProgress.h"
#include "embedded/playlistmanager/inc/PlayListModel.h"
#include "embedded/omegapicommon/inc/EmbeddedEnv.h"
#include "embedded/omegapicommon/inc/OmegaPiDBusServiceNames.h"
#include "embedded/playlistmanager/inc/PLAudioDBusAdaptor.h"

using namespace orcus;

//-------------------------------------------------------------------------------------------

QString mountPointFromArguments()
{
	int idx, state;
	QString name;
	QStringList args = QCoreApplication::arguments();
	
	for(idx = 0, state = 0; idx < args.size() && name.isEmpty(); idx++)
	{
		if(args.at(idx) == "--add-mount")
		{
			state = 1;
		}
		else if(state == 1)
		{
			name = args.at(idx);
		}
	}
	return name;
}

//-------------------------------------------------------------------------------------------

QString playlistFromArguments()
{
	int idx, state;
	QString name;
	QStringList args = QCoreApplication::arguments();
	
	for(idx = 0, state = 0; idx < args.size() && name.isEmpty(); idx++)
	{
		if(args.at(idx) == "-p")
		{
			state = 1;
		}
		else if(state == 1)
		{
			name = args.at(idx);
			if(!QFileInfo::exists(name))
			{
				name = QString();
			}
		}
	}
	return name;
}

//-------------------------------------------------------------------------------------------

void playlistToDBList(QVector<track::info::InfoSPtr>& playList, QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB)
{
	for(QVector<track::info::InfoSPtr>::iterator ppI = playList.begin(); ppI != playList.end(); ppI++)
	{
		track::info::InfoSPtr pFInfo = *ppI;
		track::db::DBInfoSPtr pDBInfo = pFInfo.dynamicCast<track::db::DBInfo>();
		if(pDBInfo.isNull())
		{
			pFInfo = track::db::DBInfo::readInfo(pFInfo->getFilename());
			if(!pFInfo.isNull())
			{
				pDBInfo = pFInfo.dynamicCast<track::db::DBInfo>();
			}
		}
		if(!pDBInfo.isNull())
		{
			if(pDBInfo->isChildren())
			{
				for(int i = 0; i < pDBInfo->noChildren(); i++)
				{
					playListDB.append(QPair<track::db::DBInfoSPtr,tint>(pDBInfo, i));
				}
			}
			else
			{
				playListDB.append(QPair<track::db::DBInfoSPtr,tint>(pDBInfo, -1));
			}
		}
	}
}

//-------------------------------------------------------------------------------------------

void playlistSubtrackToDBList(QVector<QPair<track::info::InfoSPtr, tint> >& playList, QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB)
{
	for(QVector<QPair<track::info::InfoSPtr, tint> >::iterator ppI = playList.begin(); ppI != playList.end(); ppI++)
	{
		int subTrack = (*ppI).second;
		track::info::InfoSPtr pFInfo = (*ppI).first;
		track::db::DBInfoSPtr pDBInfo = pFInfo.dynamicCast<track::db::DBInfo>();
		if(pDBInfo.isNull())
		{
			pFInfo = track::db::DBInfo::readInfo(pFInfo->getFilename());
			if(!pFInfo.isNull())
			{
				pDBInfo = pFInfo.dynamicCast<track::db::DBInfo>();
			}
		}
		if(!pDBInfo.isNull())
		{
			playListDB.append(QPair<track::db::DBInfoSPtr,tint>(pDBInfo, subTrack));
		}
	}
}

//-------------------------------------------------------------------------------------------

bool loadPlaylistFromDBOrArguments(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB)
{
	QString plFilename;
	QVector<track::info::InfoSPtr> playList;
	track::db::TrackDB *db = track::db::TrackDB::instance();
	bool res = false;
	
	plFilename = playlistFromArguments();
	if(!plFilename.isEmpty())
	{
		if(track::db::PlaylistAbstractIO::isSupported(plFilename))
		{
			QSharedPointer<track::db::PlaylistAbstractIO> pLoader = track::db::PlaylistIOFactory::createShared(track::db::PlaylistAbstractIO::factoryName(plFilename));
	        if(pLoader.data()!=0)
			{
				CLIProgress progressBar;
				
				pLoader->useMountedDrives();
				if(pLoader->load(plFilename, playList, &progressBar))
				{
					playlistToDBList(playList, playListDB);
					res = true;
				}
				else
				{
					common::Log::g_Log << "Failed to load playlist '" << plFilename << "'." << common::c_endl;
				}
			}
			else
			{
				common::Log::g_Log << "Failed to create playlist loader for '" << plFilename << "'." << common::c_endl;
			}
		}
		else
		{
			common::Log::g_Log << "Unsupported playlist format for '" << plFilename << "'." << common::c_endl;
		}
	}
	
	if(!res && db != NULL)
	{
		QMap<tint,QString> dbPlaylists = db->playlists();
		if(!dbPlaylists.isEmpty())
		{
			QVector<QPair<track::info::InfoSPtr, tint> > playListSubtracks;
			int playlistID = dbPlaylists.lastKey();
			common::Log::g_Log << "Loading previous playlist from database." << common::c_endl;
			playList.clear();
			
			if(db->loadPlaylist(playlistID, playListSubtracks))
			{
				playlistSubtrackToDBList(playListSubtracks, playListDB);
				res = true;
			}
			else
			{
				common::Log::g_Log << "Failed to load pervious playlist from database." << common::c_endl;
			}
		}
		else
		{
			common::Log::g_Log << "No playlist in database. Use -p <playlist> to define a playlist." << common::c_endl;
		}
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void printPlaylistAsQMLModel(QVector<QPair<track::db::DBInfoSPtr,tint> >& playListDB)
{
	QString artistData, titleData, albumData;

	fprintf(stdout, "\t\tListModel {\n");
	for(QVector<QPair<track::db::DBInfoSPtr,tint> >::iterator ppI = playListDB.begin(); ppI != playListDB.end(); ppI++)
	{
		track::db::DBInfoSPtr item = (*ppI).first;
		tint subtrackID = (*ppI).second;
		
		fprintf(stdout, "\t\t\tListElement {\n");
		artistData = QString("\t\t\t\tartist: \"%1\"").arg(item->artist());
		if(subtrackID >= 0 && subtrackID < item->noChildren())
		{
			titleData = QString("\t\t\t\ttitle: \"%1\"").arg(item->child(subtrackID).name());
		}
		else
		{
			titleData = QString("\t\t\t\ttitle: \"%1\"").arg(item->title());
		}
		albumData = QString("\t\t\t\talbum: \"%1\"").arg(item->album());
		fprintf(stdout, "%s\n", artistData.toUtf8().constData());
		fprintf(stdout, "%s\n", titleData.toUtf8().constData());
		fprintf(stdout, "%s\n", albumData.toUtf8().constData());
		fprintf(stdout, "\t\t\t}\n");
	}
	fprintf(stdout, "\t\t}\n");
}

//-------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	int res = -1;
	
	setupEnviroment(argv[0]);
	initCodecs();

	QString trackDBFilename = userApplicationDataDirectory() + "track_playlist_dev.db";
	track::db::TrackDB *trackDB = track::db::TrackDB::instance(trackDBFilename);
	if(trackDB != 0)
	{
		QString mountPoint;
		QGuiApplication app(argc, argv);
		QVector<QPair<track::db::DBInfoSPtr,tint> > playListDB;
		
		mountPoint = mountPointFromArguments();
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
		else if(loadPlaylistFromDBOrArguments(playListDB))
		{
			qmlRegisterType<orcus::PlayListModel>("uk.co.blackomega", 1, 0, "PlayListModel");

#if defined(OMEGA_LINUX)
			QDBusConnection bus = QDBusConnection::systemBus();
#else
			QDBusConnection bus = QDBusConnection::sessionBus();
#endif
			if(bus.isConnected())
			{
				QDBusInterface audioDaemonIFace(OMEGAAUDIODAEMON_SERVICE_NAME, "/", OMEGAAUDIODAEMON_DBUS_IFACE_NAME, bus);
				if(audioDaemonIFace.isValid())
				{
					QQmlApplicationEngine engine;
					PlayListModel playListModel(playListDB, &audioDaemonIFace, &engine);
					
					engine.rootContext()->setContextProperty("playListModel", &playListModel);
					engine.load(QUrl("qrc:/Resources/playlist.qml"));
					
					QObject plIFaceObj;
					orcus::PLAudioDBusAdaptor *pIface = new orcus::PLAudioDBusAdaptor(&playListModel, plIFaceObj);
					bus.registerObject("/", &plIFaceObj);
					if(bus.registerService(OMEGAPLMANAGERAUDIO_DBUS_IFACE_NAME))
					{
						res = app.exec();
					}
					else
					{
						common::Log::g_Log << "Failed to present register dbus service for Playlist Manager." << common::c_endl;
						common::Log::g_Log << qPrintable(bus.lastError().message()) << common::c_endl;					
					}
				}
				else
				{
					common::Log::g_Log << "Failed to connect to Omega Audio Daemon." << common::c_endl;
					common::Log::g_Log << qPrintable(bus.lastError().message()) << common::c_endl;
				}
			}
			else
			{
				common::Log::g_Log << "Failed to connect to session D-Bus" << common::c_endl;
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