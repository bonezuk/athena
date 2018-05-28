#include "dlna/inc/UPnPProvider.h"
#include "dlna/inc/DiskIF.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

QSharedPointer<UPnPProvider> UPnPProvider::m_instance;

//-------------------------------------------------------------------------------------------

UPnPProvider::UPnPProvider() : m_initFlag(false),
	m_webResourceDir()
{}

//-------------------------------------------------------------------------------------------

UPnPProvider::~UPnPProvider()
{}

//-------------------------------------------------------------------------------------------

QSharedPointer<UPnPProvider> UPnPProvider::instance(const QString& webResourceDir)
{
    if(m_instance.data()==0)
	{
		UPnPProviderSPtr nInstance(new UPnPProvider);
		if(nInstance->start(webResourceDir))
		{
			m_instance = nInstance;
		}
		else
		{
			nInstance->stop();
		}
	}
	return m_instance;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<UPnPProvider> UPnPProvider::instance()
{
	return m_instance;
}

//-------------------------------------------------------------------------------------------

void UPnPProvider::shutdown()
{
	if(m_instance.data()!=0)
	{
		UPnPProviderSPtr eInstance;
		m_instance->stop();
		m_instance = eInstance;
	}
}

//-------------------------------------------------------------------------------------------

bool UPnPProvider::start(const QString& webResourceDir)
{
	UPnPIFSPtr pAPI = UPnPIF::instance();
	bool res = true;
	
    if(pAPI->UpnpInit(0,0)==UPNP_E_SUCCESS)
	{
        m_initFlag = true;
	}
	else
	{
		res = false;
	}
	
	if(!res)
	{
		stop();
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void UPnPProvider::stop()
{
	if(m_initFlag)
	{
		UPnPIFSPtr pAPI = UPnPIF::instance();
        pAPI->UpnpFinish();
	}
}

//-------------------------------------------------------------------------------------------

QString UPnPProvider::getDeviceUDN() const
{
	QString udn;
	common::SHA1Digest digest;
	tbyte mem[common::c_SHA1HashSize];
	QVector<QPair<common::HardwareIdentification::HardwareType,QString> > hwIDList;

	if(common::HardwareIdentification::instance().getIdentity(hwIDList) && !hwIDList.isEmpty())
	{
		QVector<QPair<common::HardwareIdentification::HardwareType,QString> >::iterator ppI;
		for(ppI=hwIDList.begin();ppI!=hwIDList.end();ppI++)
		{
			const QPair<common::HardwareIdentification::HardwareType,QString>& p = *ppI;
			digest.input(p.second.toUtf8().constData(),p.second.toUtf8().length());
		}
	}
	if(digest.getDigestFinal(mem,common::c_SHA1HashSize))
	{
		QByteArray arr(mem,common::c_SHA1HashSize);
		udn = getDigestKey(arr);
	}
	return udn;
}

//-------------------------------------------------------------------------------------------
// Build key from digest
// XXXXX-XXXX-XXXX-XXXX-XXXXX
// 0 1 2  3 4  5 6  7 8  9 0
//-------------------------------------------------------------------------------------------

QString UPnPProvider::getDigestKey(const QByteArray& digest) const
{
	tint i;
	QString key;
	const tbyte *mem = digest.constData();
	
	if(mem!=0 && digest.size()>=11)
	{
		for(i=0;i<11;i++)
		{
			tubyte a,b,c;
			tchar t[2] = {'\0','\0'};
			
			a = static_cast<tubyte>(mem[i]);
			b = (a >> 4) & 0x0f;
			c = a & 0x0f;
			
			if(b>=0 && b<10)
			{
				t[0] = static_cast<tchar>(b) + '0';
			}
			else
			{
				t[0] = static_cast<tchar>(b-10) + 'A';
			}
			key += t;
			
			if(i==2 || i==4 || i==6 || i==8)
			{
				key += "-";
			}
			
			if(c>=0 && c<10)
			{
				t[0] = static_cast<tchar>(c) + '0';
			}
			else
			{
				t[0] = static_cast<tchar>(c-10) + 'A';
			}
			key += t;
		}
	}
	return key;
}

//-------------------------------------------------------------------------------------------
// The UPnP web service directory conforms to the following naming convention. The
// convention is used to what kind of service or type various components relate too.
//-------------------------------------------------------------------------------------------

bool UPnPProvider::initializeWebDirectory()
{
	// If no web directory then service probably relates to control point.
	if(m_webResourceDir.isEmpty())
	{
		return true;
	}
	return initScanWebDirectory(m_webResourceDir);
}

//-------------------------------------------------------------------------------------------

bool UPnPProvider::initScanWebDirectory(const QString& dirName)
{
	bool res = true;
	DiskIFSPtr diskIF = DiskIF::instance();
	
	if(!dirName.isEmpty() && diskIF->isDirectory(dirName))
	{
		DiskIF::DirHandle h = diskIF->openDirectory(dirName);

		if(h!=DiskIF::c_invalidDirectoryHandle)
		{
			QString name;

			while(name=diskIF->nextDirectoryEntry(h),!name.isEmpty() && res)
			{
				QString fullName;
				
				fullName = dirName + "/" + name;
                fullName = DiskIF::toNativeSeparators(fullName);
				if(diskIF->isFile(fullName))
				{
					res = initProcessWebFile(dirName,name);
				}
				else if(diskIF->isDirectory(fullName))
				{
					res = initScanWebDirectory(fullName);
				}
			}
			diskIF->closeDirectory(h);
		}
		else
		{
			res = false;
		}
	}
	else
	{
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

bool UPnPProvider::initProcessWebFile(const QString& dirName,const QString& fileName)
{
	bool res = true;

	if(fileName.length()>=7 && fileName.right(7).toLower()=="ddd.xml")
	{
		res = initDeviceFromDescription(DiskIF::mergeName(dirName,fileName));
	}
	return res;
}


//-------------------------------------------------------------------------------------------

bool UPnPProvider::initDeviceFromDescription(const QString& dddFileName)
{
	return true;
}

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace orcus
//-------------------------------------------------------------------------------------------
