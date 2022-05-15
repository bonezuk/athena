//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_DLNA_DISKLAYERIF_H
#define __OMEGA_DLNA_DISKLAYERIF_H
//-------------------------------------------------------------------------------------------

#include "dlna/inc/DiskIF.h"

#include <QtCore/QPair>
#include <QtCore/QMap>
#include <QtCore/QSet>

//-------------------------------------------------------------------------------------------
namespace omega
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

class DLNA_EXPORT DiskLayerIF : public DiskIF
{
	public:
		DiskLayerIF();
		virtual ~DiskLayerIF();

		virtual bool isFile(const QString& name) const;
		virtual bool isDirectory(const QString& name) const;
		
		virtual DirHandle openDirectory(const QString& name) const;
		virtual QString nextDirectoryEntry(DirHandle h) const;
		virtual void closeDirectory(DirHandle h) const;

	protected:

        mutable QRecursiveMutex m_mutex;
#if defined(OMEGA_WIN32)
		mutable QMap<DiskIF::DirHandle,QPair<QString,WIN32_FIND_DATAW> > m_dirHandleMap;
#elif defined(OMEGA_POSIX)
		mutable QSet<DiskIF::DirHandle> m_dirHandleMap;
#endif
		
		virtual QString directoryName(const QString& name) const;
};

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

