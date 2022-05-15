#include "dlna/inc/DiskIF.h"

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

QSharedPointer<DiskIF> DiskIF::m_instance;


#if defined(OMEGA_WIN32)
const DiskIF::DirHandle DiskIF::c_invalidDirectoryHandle = INVALID_HANDLE_VALUE;
#elif defined(OMEGA_POSIX)
const DiskIF::DirHandle DiskIF::c_invalidDirectoryHandle = 0;
#endif

ABSTRACT_FACTORY_CLASS_IMPL(DiskIFFactory,DiskIF)

//-------------------------------------------------------------------------------------------

DiskIF::DiskIF()
{}

//-------------------------------------------------------------------------------------------

DiskIF::~DiskIF()
{}

//-------------------------------------------------------------------------------------------

DiskIF::DirHandle DiskIF::invalidDirectory()
{
	return c_invalidDirectoryHandle;
}

//-------------------------------------------------------------------------------------------

bool DiskIF::isValidDirectory(DirHandle h)
{
	return (h!=invalidDirectory());
}

//-------------------------------------------------------------------------------------------

QSharedPointer<DiskIF> DiskIF::instance(const tchar *factoryName)
{
    if(m_instance.data()==0)
	{
		QSharedPointer<DiskIF> nInstance(DiskIFFactory::createUnmanaged(factoryName));
		m_instance = nInstance;
	}
	return m_instance;
}

//-------------------------------------------------------------------------------------------

QSharedPointer<DiskIF> DiskIF::instance()
{
	return m_instance;
}

//-------------------------------------------------------------------------------------------

void DiskIF::release()
{
	QSharedPointer<DiskIF> eInstance;
	m_instance = eInstance;
}

//-------------------------------------------------------------------------------------------

QString DiskIF::toNativeSeparators(const QString& iName)
{
    QString oName;
    for(int i=0;i<iName.size();i++)
    {
#if defined(OMEGA_WIN32)
        if(iName.at(i)==QChar('/'))
        {
            oName += '\\';
        }
#else
        if(iName.at(i)==QChar('\\'))
        {
            oName += '/';
        }
#endif
        else
        {
            oName += iName.at(i);
        }
    }
    return oName;
}

//-------------------------------------------------------------------------------------------

QString DiskIF::mergeName(const tchar *dirName,const tchar *fileName)
{
	return mergeName(QString::fromUtf8(dirName),QString::fromUtf8(fileName));
}

//-------------------------------------------------------------------------------------------

QString DiskIF::mergeName(const tchar *dirName,const QString& fileName)
{
	return mergeName(QString::fromUtf8(dirName),fileName);
}

//-------------------------------------------------------------------------------------------

QString DiskIF::mergeName(const QString& dirName,const tchar *fileName)
{
	return mergeName(dirName,QString::fromUtf8(fileName));
}

//-------------------------------------------------------------------------------------------

QString DiskIF::mergeName(const QString& dirName,const QString& fileName)
{
	QString dName(dirName),fName(fileName),fullName;
	
	while(!dName.isEmpty() && (dName.at(dName.length()-1)==QChar('/') || dName.at(dName.length()-1)==QChar('\\')))
	{
		dName = dName.mid(0,dName.length() - 1);
	}
	while(!fName.isEmpty() && (fName.at(0)==QChar('/') || fName.at(0)==QChar('\\')))
	{
		fName = fName.mid(1,fName.length() - 1);
	}
	fullName = dName;
	if(!dName.isEmpty() && !fName.isEmpty())
	{
		fullName += "/";
	}
	fullName += fName;
	fullName = toNativeSeparators(fullName);
	return fullName;
}

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace omega
//-------------------------------------------------------------------------------------------
