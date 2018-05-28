#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "dlna/test/UPnPProviderTestEnviroment.h"
#include "track/model/test/TrackDBTestEnviroment.h"

#include "engine/inc/Codec.h"
#include "engine/blackomega/inc/MPCodec.h"
#include "engine/silveromega/inc/SilverCodec.h"
#include "engine/whiteomega/inc/WhiteCodec.h"

#if defined(Q_OS_MAC)
#include "common/inc/Bundle.h"
#endif

#include "common/inc/SBService.h"

#include <QApplication>
#include <QSettings>

//-------------------------------------------------------------------------------------------
#if defined(_WIN32)
testing::internal::Mutex testing::internal::g_gmock_mutex;
testing::internal::Mutex testing::internal::g_linked_ptr_mutex;
testing::internal::ThreadLocal<class testing::Sequence *> testing::internal::g_gmock_implicit_sequence;
#endif
//-------------------------------------------------------------------------------------------

class GlobalEnviroment : public ::testing::Environment
{
	public:
		virtual ~GlobalEnviroment();
		
		virtual void SetUp();
		virtual void TearDown();
};

//-------------------------------------------------------------------------------------------

GlobalEnviroment::~GlobalEnviroment()
{}

//-------------------------------------------------------------------------------------------

void GlobalEnviroment::SetUp()
{
	orcus::dlna::test::UPnPProviderTestEnviroment::instance();
	orcus::track::model::TrackDBTestEnviroment *pTrackDBTest = orcus::track::model::TrackDBTestEnviroment::instance();
    ASSERT_TRUE(pTrackDBTest->load());
}

//-------------------------------------------------------------------------------------------

void GlobalEnviroment::TearDown()
{
	orcus::track::model::TrackDBTestEnviroment *pTrackDBTest = orcus::track::model::TrackDBTestEnviroment::instance();
	pTrackDBTest->unload();
	delete pTrackDBTest;
	orcus::dlna::test::UPnPProviderTestEnviroment *pUPnPProviderTest = orcus::dlna::test::UPnPProviderTestEnviroment::instance();
	delete pUPnPProviderTest;
}

//-------------------------------------------------------------------------------------------

/*
TEST(TrackDBTestEnviroment,buildTestDB)
{
    orcus::track::model::TrackDBTestEnviroment *pTrackDBTest = orcus::track::model::TrackDBTestEnviroment::instance();
    pTrackDBTest->buildTestDB("/Volumes/Iomega_HDD/TestMusicDB/track.db","/Volumes/Iomega_HDD/TestMusicDB");
    delete pTrackDBTest;
}
*/

//-------------------------------------------------------------------------------------------

int main(int argc,char **argv)
{
#if defined(Q_OS_MAC)
	QString pluginDir;
#if defined(OMEGA_MAC_STORE)
    orcus::common::Log::g_Log.print("Store Bundle Plugin\n");
    pluginDir = orcus::common::Bundle::pluginDirectory();
    orcus::common::Log::g_Log.print("%s\n",pluginDir.toLatin1().constData());
#else
    QFileInfo appFile(argv[0]);
	QDir d = appFile.absolutePath();
    orcus::common::Log::g_Log.print("%s\n",d.absolutePath().toLatin1().constData());
#if defined(OMEGA_BUNDLE_TEST)
    orcus::common::Log::g_Log.print("Bundle Plugin\n");
	d.cdUp();
    orcus::common::Log::g_Log.print("%s\n",d.absolutePath().toLatin1().constData());
#endif
	d.cd("Plugins");
    orcus::common::Log::g_Log.print("%s\n",d.absolutePath().toLatin1().constData());
    pluginDir = d.absolutePath();
#endif
	QApplication::setLibraryPaths(QStringList(pluginDir));
#else
	QFileInfo appFile(argv[0]);
	QDir d = appFile.absolutePath();
	QString pluginDir;
	d.cdUp();
	d.cd("plugins");
	pluginDir = d.absolutePath();
	QApplication::setLibraryPaths(QStringList(pluginDir));
#endif

#if defined(ORCUS_WIN32)
	LoadLibraryA("blueomega.dll");
	LoadLibraryA("silveromega.dll");
	LoadLibraryA("blackomega.dll");
	LoadLibraryA("whiteomega.dll");
	LoadLibraryA("redomega.dll");
	LoadLibraryA("greenomega.dll");
	LoadLibraryA("rtp_silveromega.dll");
	LoadLibraryA("widget.dll");
	LoadLibraryA("violetomega.dll");
	LoadLibraryA("wavpackomega.dll");
	LoadLibraryA("monkeyomega.dll");
	LoadLibraryA("toneomega.dll");
#endif

#if defined(OMEGA_MAC_STORE)
	QString settingPath = orcus::common::SBService::applicationDataDirectory();
	QSettings::setPath(QSettings::IniFormat,QSettings::UserScope,settingPath);
	QSettings::setPath(QSettings::NativeFormat,QSettings::UserScope,settingPath);
		
	QCoreApplication::setOrganizationName("Stuart MacLean");
	QCoreApplication::setOrganizationDomain("www.blackomega.co.uk");
	QCoreApplication::setApplicationName("Black Omega Test");
#else
	QCoreApplication::setOrganizationName("Tiger-Eye-Test");
	QCoreApplication::setOrganizationDomain("www.blackomega.co.uk");
	QCoreApplication::setApplicationName("BlackOmega2Test");
#endif

	orcus::engine::CodecInitialize::start();
	orcus::engine::blackomega::MPCodecInitialize::start();
	orcus::engine::silveromega::SilverCodecInitialize::start();
	orcus::engine::whiteomega::WhiteCodecInitialize::start();

    ::testing::InitGoogleTest(&argc,argv);
	::testing::InitGoogleMock(&argc,argv);
	::testing::AddGlobalTestEnvironment(new GlobalEnviroment);
	return RUN_ALL_TESTS();
}
