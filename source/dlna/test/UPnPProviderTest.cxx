#include "network/inc/Resource.h"
#include "common/inc/DiskOps.h"
#include "dlna/inc/UPnPProvider.h"
#include "dlna/test/UPnPMockIF.h"
#include "dlna/test/UPnPProviderTestEnviroment.h"
#include "dlna/test/DiskMockIF.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"


using namespace orcus::dlna;
using namespace orcus;
using namespace testing;

//-------------------------------------------------------------------------------------------
// UPnPProvider
//-------------------------------------------------------------------------------------------

class UPnPProviderTest : public UPnPProvider
{
	public:
		UPnPProviderTest();
        virtual QString testGetDeviceUDN() const;
        virtual QString testGetDigestKey(const QByteArray& digest) const;
};

//-------------------------------------------------------------------------------------------

UPnPProviderTest::UPnPProviderTest()
{}

//-------------------------------------------------------------------------------------------

QString UPnPProviderTest::testGetDeviceUDN() const
{
    return getDeviceUDN();
}

//-------------------------------------------------------------------------------------------

QString UPnPProviderTest::testGetDigestKey(const QByteArray& digest) const
{
	return getDigestKey(digest);
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,instanceSuccessfulAndShutdown)
{
	UPnPIFSPtr pAPIEmpty;
	UPnPIFSPtr pMockAPI = UPnPIF::instance("mock");
    test::UPnPProviderTestEnviroment *env = test::UPnPProviderTestEnviroment::instance();
	
	{
        UPnPMockIF& pAPI = dynamic_cast<UPnPMockIF&>(*(pMockAPI.data()));
        EXPECT_CALL(pAPI,UpnpInit(0,0)).Times(1).WillOnce(Return(UPNP_E_SUCCESS));
        EXPECT_CALL(pAPI,UpnpFinish()).Times(1);

		{
			UPnPProviderSPtr provider = UPnPProvider::instance(env->root(0));
            EXPECT_TRUE(provider.data()!=0);
            EXPECT_TRUE(UPnPProvider::instance().data()!=0);
		}
	
		UPnPProvider::shutdown();
		
        EXPECT_TRUE(UPnPProvider::instance().data()==0);
	}
	
    pMockAPI = pAPIEmpty;
	UPnPIF::release();
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,instanceFailureAndShutdown)
{
	UPnPIFSPtr pAPIEmpty;
	UPnPIFSPtr pMockAPI = UPnPIF::instance("mock");
    test::UPnPProviderTestEnviroment *env = test::UPnPProviderTestEnviroment::instance();
	
	{
        UPnPMockIF& pAPI = dynamic_cast<UPnPMockIF&>(*(pMockAPI.data()));
        EXPECT_CALL(pAPI,UpnpInit(0,0)).Times(1).WillOnce(Return(UPNP_E_INIT_FAILED));

		{
			UPnPProviderSPtr provider = UPnPProvider::instance(env->root(0));
            EXPECT_TRUE(provider.data()==0);
		}
	
		UPnPProvider::shutdown();
		
        EXPECT_TRUE(UPnPProvider::instance().data()==0);
	}
	
    pMockAPI = pAPIEmpty;
	UPnPIF::release();
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,getDigestKeyNoMemory)
{
	QByteArray mem;
	UPnPProviderTest pro;
	QString key = pro.testGetDigestKey(mem);
	EXPECT_TRUE(key.isEmpty());
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,getDigestKeyProper)
{
	tubyte data[11] = { 0xf1, 0xe2, 0xd3, 0xc4, 0xb5, 0xa6, 0x97, 0x87, 0xb8, 0x69, 0x5a };
	QByteArray mem(reinterpret_cast<const char *>(data),11);
	UPnPProviderTest pro;
	QString key = pro.testGetDigestKey(mem);
	EXPECT_TRUE(key=="F1E2D-3C4B-5A69-787B-8695A");
}

//-------------------------------------------------------------------------------------------

class UPnPProviderHardwareIdentification : public orcus::common::HardwareIdentification
{
	public:
		UPnPProviderHardwareIdentification();
		virtual ~UPnPProviderHardwareIdentification();
		
		MOCK_CONST_METHOD1(getIdentity,bool(QVector<QPair<HardwareType,QString> >& idList));
};

//-------------------------------------------------------------------------------------------

UPnPProviderHardwareIdentification::UPnPProviderHardwareIdentification()
{
    m_instance = this;
}

//-------------------------------------------------------------------------------------------

UPnPProviderHardwareIdentification::~UPnPProviderHardwareIdentification()
{
	m_instance = 0;
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,getDeviceUDNNoHardwareID)
{
	tbyte mem[common::c_SHA1HashSize];
	common::SHA1Digest digest;
	ASSERT_TRUE(digest.getDigestFinal(mem,common::c_SHA1HashSize));
	QByteArray expectArray(mem,common::c_SHA1HashSize);
	
	UPnPProviderTest pro;
	QString expectUDN = pro.testGetDigestKey(expectArray);
	
	UPnPProviderHardwareIdentification mockHW;
	EXPECT_CALL(mockHW,getIdentity(_)).Times(1).WillOnce(Return(false));
	
	QString testUDN = pro.testGetDeviceUDN();
	EXPECT_TRUE(testUDN==expectUDN);
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,getDeviceUDNWithHardwareID)
{
    QVector<QPair<common::HardwareIdentification::HardwareType,QString> > hwList;
    hwList.append(QPair<common::HardwareIdentification::HardwareType,QString>(common::HardwareIdentification::e_Motherboard,"ID12345"));
    hwList.append(QPair<common::HardwareIdentification::HardwareType,QString>(common::HardwareIdentification::e_NetworkMACAddress,"01:23:45:67:89:AB:CD:EF"));

	tbyte mem[common::c_SHA1HashSize];
	common::SHA1Digest digest;
	digest.input(hwList.at(0).second.toUtf8().constData(),hwList.at(0).second.toUtf8().size());
	digest.input(hwList.at(1).second.toUtf8().constData(),hwList.at(1).second.toUtf8().size());
	ASSERT_TRUE(digest.getDigestFinal(mem,common::c_SHA1HashSize));
	QByteArray expectArray(mem,common::c_SHA1HashSize);

	UPnPProviderTest pro;
	QString expectUDN = pro.testGetDigestKey(expectArray);

	UPnPProviderHardwareIdentification mockHW;
    EXPECT_CALL(mockHW,getIdentity(_)).Times(1).WillOnce(DoAll(SetArgReferee<0>(hwList),Return(true)));

	QString testUDN = pro.testGetDeviceUDN();
	EXPECT_TRUE(testUDN==expectUDN);
}

//-------------------------------------------------------------------------------------------

class UPnPProviderInitScanWebDirectoryTest : public UPnPProvider
{
	public:
		MOCK_METHOD2(initProcessWebFile,bool(const QString& dirName,const QString& fileName));
		bool testInitScanWebDirectory(const QString& dirName);
};

//-------------------------------------------------------------------------------------------

bool UPnPProviderInitScanWebDirectoryTest::testInitScanWebDirectory(const QString& dirName)
{
	return initScanWebDirectory(dirName);
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,initScanWebDirectoryNoName)
{
    UPnPProviderInitScanWebDirectoryTest provider;
    EXPECT_FALSE(provider.testInitScanWebDirectory(QString("")));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,initScanWebDirectoryNotADirectory)
{
    DiskIFSPtr pMockAPI = DiskIF::instance("mock");
    DiskMockIF& pAPI = dynamic_cast<DiskMockIF&>(*(pMockAPI.data()));

	EXPECT_CALL(pAPI,isDirectory(QString("/usr/test"))).Times(1).WillOnce(Return(false));

    UPnPProviderInitScanWebDirectoryTest provider;
    EXPECT_FALSE(provider.testInitScanWebDirectory(QString("/usr/test")));

    DiskIF::release();
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,initScanWebDirectoryErrorOpeningDirectory)
{
    DiskIF::DirHandle invalidH = DiskIF::invalidDirectory();
    DiskIFSPtr pMockAPI = DiskIF::instance("mock");
    DiskMockIF& pAPI = dynamic_cast<DiskMockIF&>(*(pMockAPI.data()));

    EXPECT_CALL(pAPI,isDirectory(QString("/usr/test"))).Times(1).WillOnce(Return(true));
    EXPECT_CALL(pAPI,openDirectory(QString("/usr/test"))).Times(1).WillOnce(Return(invalidH));

    UPnPProviderInitScanWebDirectoryTest provider;
    EXPECT_FALSE(provider.testInitScanWebDirectory(QString("/usr/test")));

    DiskIF::release();
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,initScanWebDirectoryEmptyDirectory)
{
    DiskIFSPtr pMockAPI = DiskIF::instance("mock");
    DiskMockIF *pAPI = dynamic_cast<DiskMockIF *>(pMockAPI.data());

    EXPECT_CALL(*pAPI,isDirectory(QString("/usr/test"))).Times(1).WillOnce(Return(true));
    EXPECT_CALL(*pAPI,openDirectory(QString("/usr/test"))).Times(1).WillOnce(Return((DiskIF::DirHandle)(1)));
    EXPECT_CALL(*pAPI,nextDirectoryEntry((DiskIF::DirHandle)(1))).Times(1).WillOnce(Return(QString("")));
    EXPECT_CALL(*pAPI,closeDirectory((DiskIF::DirHandle)(1))).Times(1);

    UPnPProviderInitScanWebDirectoryTest provider;
    EXPECT_TRUE(provider.testInitScanWebDirectory(QString("/usr/test")));

    DiskIF::release();
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,initScanWebDirectoryProcessTwoFiles)
{
    DiskIFSPtr pMockAPI = DiskIF::instance("mock");
    DiskMockIF& pAPI = dynamic_cast<DiskMockIF&>(*(pMockAPI.data()));

    EXPECT_CALL(pAPI,isDirectory(QString("/usr/test"))).Times(1).WillOnce(Return(true));
    EXPECT_CALL(pAPI,openDirectory(QString("/usr/test"))).Times(1).WillOnce(Return((DiskIF::DirHandle)(1)));
    EXPECT_CALL(pAPI,nextDirectoryEntry((DiskIF::DirHandle)(1))).Times(3).WillOnce(Return(QString("fileA.html"))).WillOnce(Return(QString("fileB.html"))).WillRepeatedly(Return(QString("")));
    EXPECT_CALL(pAPI,closeDirectory((DiskIF::DirHandle)(1))).Times(1);
    EXPECT_CALL(pAPI,isFile(QString(DiskIF::toNativeSeparators("/usr/test/fileA.html")))).Times(1).WillOnce(Return(true));
    EXPECT_CALL(pAPI,isFile(QString(DiskIF::toNativeSeparators("/usr/test/fileB.html")))).Times(1).WillOnce(Return(true));

    UPnPProviderInitScanWebDirectoryTest provider;
    EXPECT_CALL(provider,initProcessWebFile(QString("/usr/test"),QString("fileA.html"))).Times(1).WillOnce(Return(true));
    EXPECT_CALL(provider,initProcessWebFile(QString("/usr/test"),QString("fileB.html"))).Times(1).WillOnce(Return(true));

    EXPECT_TRUE(provider.testInitScanWebDirectory(QString("/usr/test")));

    DiskIF::release();
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,initScanWebDirectoryProcessFileThenSubDir)
{
    DiskIFSPtr pMockAPI = DiskIF::instance("mock");
    DiskMockIF& pAPI = dynamic_cast<DiskMockIF&>(*(pMockAPI.data()));

    EXPECT_CALL(pAPI,isDirectory(QString("/usr/test"))).Times(1).WillOnce(Return(true));
    EXPECT_CALL(pAPI,openDirectory(QString("/usr/test"))).Times(1).WillOnce(Return((DiskIF::DirHandle)(1)));
    EXPECT_CALL(pAPI,nextDirectoryEntry((DiskIF::DirHandle)(1))).Times(3).WillOnce(Return(QString("images"))).WillOnce(Return(QString("fileA.html"))).WillRepeatedly(Return(QString("")));
    EXPECT_CALL(pAPI,closeDirectory((DiskIF::DirHandle)(1))).Times(1);
    EXPECT_CALL(pAPI,isFile(QString(DiskIF::toNativeSeparators("/usr/test/fileA.html")))).Times(1).WillOnce(Return(true));
    EXPECT_CALL(pAPI,isFile(QString(DiskIF::toNativeSeparators("/usr/test/images")))).Times(1).WillOnce(Return(false));
    EXPECT_CALL(pAPI,isDirectory(QString(DiskIF::toNativeSeparators("/usr/test/images")))).Times(2).WillRepeatedly(Return(true));

    EXPECT_CALL(pAPI,openDirectory(QString(DiskIF::toNativeSeparators("/usr/test/images")))).Times(1).WillOnce(Return((DiskIF::DirHandle)(2)));
    EXPECT_CALL(pAPI,nextDirectoryEntry((DiskIF::DirHandle)(2))).Times(2).WillOnce(Return(QString("fileB.html"))).WillRepeatedly(Return(QString("")));
    EXPECT_CALL(pAPI,closeDirectory((DiskIF::DirHandle)(2))).Times(1);
    EXPECT_CALL(pAPI,isFile(QString(DiskIF::toNativeSeparators("/usr/test/images/fileB.html")))).Times(1).WillOnce(Return(true));

    UPnPProviderInitScanWebDirectoryTest provider;
    EXPECT_CALL(provider,initProcessWebFile(QString("/usr/test"),QString("fileA.html"))).Times(1).WillOnce(Return(true));
    EXPECT_CALL(provider,initProcessWebFile(QString(DiskIF::toNativeSeparators("/usr/test/images")),QString("fileB.html"))).Times(1).WillOnce(Return(true));

    EXPECT_TRUE(provider.testInitScanWebDirectory(QString("/usr/test")));

    DiskIF::release();
}

//-------------------------------------------------------------------------------------------

class UPnPProviderInitProcessWebFile : public UPnPProvider
{
	public:
		MOCK_METHOD1(initDeviceFromDescription,bool(const QString& dddFileName));
		virtual bool testInitProcessWebFile(const QString& dirName,const QString& fileName);
};

//-------------------------------------------------------------------------------------------

bool UPnPProviderInitProcessWebFile::testInitProcessWebFile(const QString& dirName,const QString& fileName)
{
	return initProcessWebFile(dirName,fileName);
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,initProcessWebFileUnknown)
{
	UPnPProviderInitProcessWebFile provider;
	EXPECT_TRUE(provider.testInitProcessWebFile("/usr/temp","unknown.xml"));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,initProcessWebFileDDDSuccess)
{
	QString fileName = DiskIF::mergeName("/usr/temp","fileDDD.xml");
	UPnPProviderInitProcessWebFile provider;
	EXPECT_CALL(provider,initDeviceFromDescription(fileName)).Times(1).WillOnce(Return(true));
	EXPECT_TRUE(provider.testInitProcessWebFile("/usr/temp","fileDDD.xml"));
}

//-------------------------------------------------------------------------------------------

TEST(UPnPProvider,initProcessWebFileDDDFailure)
{
	QString fileName = DiskIF::mergeName("/usr/temp","fileDDD.xml");
	UPnPProviderInitProcessWebFile provider;
	EXPECT_CALL(provider,initDeviceFromDescription(fileName)).Times(1).WillOnce(Return(false));
	EXPECT_FALSE(provider.testInitProcessWebFile("/usr/temp","fileDDD.xml"));
}

//-------------------------------------------------------------------------------------------
