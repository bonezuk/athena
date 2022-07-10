#include "common/inc/CommonFunctions.h"
#include "common/inc/DiskIF.h"
#include "engine/blackomega/inc/MPCodec.h"
#include "engine/silveromega/inc/SilverCodec.h"
#include "engine/whiteomega/inc/WhiteCodec.h"
#include "playerapp/playerios/inc/PlayerUISettings.h"
#include "playerapp/playerios/inc/PlayListIOSModel.h"
#include "playerapp/playerios/inc/PlayerIOSTrackDBManager.h"
#include "playerapp/playerios/inc/PlayerAudioIOInterface.h"
#include "playerapp/playercommon/inc/QAlbumListModel.h"
#include "playerapp/playercommon/inc/QAlbumTrackListModel.h"

#if defined(OMEGA_IOS)
#include "playerapp/playerios/inc/PlayerIOSAudioSession.h"
#endif

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QQmlDebuggingEnabler>

using namespace omega;

//-------------------------------------------------------------------------------------------

void setupPlatform()
{
	common::loadSharedLibrary("blackomega");
	common::loadSharedLibrary("blueomega");
	common::loadSharedLibrary("cyanomega");
	common::loadSharedLibrary("greenomega");
	common::loadSharedLibrary("redomega");
	common::loadSharedLibrary("silveromega");
	common::loadSharedLibrary("violetomega");
	common::loadSharedLibrary("whiteomega");
	common::loadSharedLibrary("widget");
#if defined(OMEGA_WIN32)
	CoInitialize(NULL);
#endif
}

//-------------------------------------------------------------------------------------------

void initCodecs()
{
	engine::CodecInitialize::start();
	engine::blackomega::MPCodecInitialize::start();
	engine::silveromega::SilverCodecInitialize::start();
	engine::whiteomega::WhiteCodecInitialize::start();
}

//-------------------------------------------------------------------------------------------

void releaseCodecs()
{
	engine::whiteomega::WhiteCodecInitialize::end();
	engine::silveromega::SilverCodecInitialize::end();
	engine::blackomega::MPCodecInitialize::end();
	engine::CodecInitialize::end();
}

//-------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	QQmlDebuggingEnabler enabler;
	QQmlApplicationEngine engine;
	
	setupPlatform();
	initCodecs();
	
	common::DiskIFSPtr diskIF = common::DiskIF::instance("disk");
	if(!diskIF.isNull())
	{
		qmlRegisterType<PlayListIOSModel>("uk.co.blackomega", 1, 0, "PlayListIOSModel");
		qmlRegisterType<PlayerUISettings>("uk.co.blackomega", 1, 0, "PlayerUISettings");
		qmlRegisterType<PlaybackStateController>("uk.co.blackomega", 1, 0, "PlaybackStateController");
		qmlRegisterType<QAlbumListModel>("uk.co.blackomega", 1, 0, "QAlbumListModel");
		qmlRegisterType<QAlbumTrackListModel>("uk.co.blackomega", 1, 0, "QAlbumTrackListModel");

		PlayerIOSTrackDBManager *trackDBManager = PlayerIOSTrackDBManager::instance();
		if(trackDBManager != 0)
		{
			QSharedPointer<OmegaPlaylistInterface> plInterface(new OmegaPlaylistInterface());
			QSharedPointer<PlayerAudioIOInterface> pAudioIO(new PlayerAudioIOInterface(plInterface));
			if(pAudioIO->init())
			{
				QSharedPointer<OmegaAudioInterface> pAudioInterface = pAudioIO.dynamicCast<OmegaAudioInterface>();
				QSharedPointer<PlayListIOSModel> pModel(new PlayListIOSModel(pAudioInterface));
				if(pModel->initialise())
				{
					QSharedPointer<PlayListModel> pLModel = pModel.dynamicCast<PlayListModel>();
					plInterface->init(pLModel);
					
					QSharedPointer<QAlbumListModel> pAlbumModel(new QAlbumListModel());
					if(pAlbumModel->initialise())
					{
#if defined(OMEGA_IOS)
						QSharedPointer<PlayerIOSAudioSession> pSession = PlayerIOSAudioSession::startSession(pLModel);
						if(!pSession.isNull())
#endif
						{
							QFile page(":/Resources/frontpage1.qml");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
							if(page.open(QIODevice::ReadOnly))
#else	
							if(page.open(QIODeviceBase::ReadOnly))
#endif
							{
								engine.rootContext()->setContextProperty("playListModel", pModel.data());
								engine.rootContext()->setContextProperty("albumModel", pAlbumModel.data());
								engine.rootContext()->setContextProperty("albumTrackModel", pAlbumModel->trackModel().data());
								engine.rootContext()->setContextProperty("playbackStateController", pModel->playbackState().data());
				
								PlayerUISettings settings;
							
								settings.registerModel(pModel.dynamicCast<QOmegaListModel>());
								settings.registerModel(pAlbumModel.dynamicCast<QOmegaListModel>());
								settings.registerModel(pAlbumModel->trackModel().dynamicCast<QOmegaListModel>());
							
								engine.rootContext()->setContextProperty("settings", &settings);
			
								QObject::connect(trackDBManager, SIGNAL(removetrack(const QString&)), pModel.data(), SLOT(deleteTrack(const QString&)));

								QObject::connect(trackDBManager, SIGNAL(newtrack(const QString&)), pAlbumModel.data(), SLOT(appendTrack(const QString&)));
								QObject::connect(trackDBManager, SIGNAL(removetrack(const QString&)), pAlbumModel.data(), SLOT(deleteTrack(const QString&)));

								QObject::connect(trackDBManager, SIGNAL(newtrack(const QString&)), pAlbumModel->trackModel().data(), SLOT(appendTrack(const QString&)));
								QObject::connect(trackDBManager, SIGNAL(removetrack(const QString&)), pAlbumModel->trackModel().data(), SLOT(deleteTrack(const QString&)));
									
								QObject::connect(pAlbumModel->trackModel().data(), SIGNAL(appendToPlaylist(const QString&)), pModel.data(), SLOT(appendTrack(const QString&)));
								
								engine.load(":/Resources/frontpage1.qml");
								app.exec();
							}
#if defined(OMEGA_IOS)
							pSession->endSession();
#endif
						}
					}
				}
				pAudioIO->quitDaemon();
			}
			trackDBManager->release();
		}
		common::DiskIF::release();
	}
	releaseCodecs();
	return 0;
}

//-------------------------------------------------------------------------------------------
