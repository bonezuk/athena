//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_DLNA_DISKIF_H
#define __OMEGA_DLNA_DISKIF_H
//-------------------------------------------------------------------------------------------

#include "dlna/inc/UPnPIF.h"
#include "common/inc/DiskOps.h"

#if defined(OMEGA_POSIX)
#include <stdio.h>
#include <dirent.h>
#endif

#include <QtCore/QDir>

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

class DLNA_EXPORT DiskIF
{
	public:
	
#if defined(OMEGA_WIN32)
	typedef HANDLE DirHandle;
	static const DirHandle c_invalidDirectoryHandle;
#elif defined(OMEGA_POSIX)
    typedef DIR* DirHandle;
	static const DirHandle c_invalidDirectoryHandle;
#endif

	public:
		DiskIF();
		virtual ~DiskIF();
		
		static QSharedPointer<DiskIF> instance(const tchar *factoryName);
		static QSharedPointer<DiskIF> instance();
		static void release();
	
		static DirHandle invalidDirectory();
		static bool isValidDirectory(DirHandle h);

        static QString toNativeSeparators(const QString& iName);

		static QString mergeName(const tchar *dirName,const tchar *fileName);
		static QString mergeName(const tchar *dirName,const QString& fileName);
		static QString mergeName(const QString& dirName,const tchar *fileName);
		static QString mergeName(const QString& dirName,const QString& fileName);

		virtual bool isFile(const QString& name) const = 0;
		virtual bool isDirectory(const QString& name) const = 0;
		
		virtual DirHandle openDirectory(const QString& name) const = 0;
		virtual QString nextDirectoryEntry(DirHandle h) const = 0;
		virtual void closeDirectory(DirHandle h) const = 0;
		
	protected:
	
        static QSharedPointer<DiskIF> m_instance;
};

typedef QSharedPointer<DiskIF> DiskIFSPtr;

//-------------------------------------------------------------------------------------------

ABSTRACT_FACTORY_CLASS(DLNA_EXPORT,DiskIFFactory,DiskIF)

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

