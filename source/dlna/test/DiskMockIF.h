//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_DLNA_DISKMOCKIF_H
#define __ORCUS_DLNA_DISKMOCKIF_H
//-------------------------------------------------------------------------------------------

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "dlna/inc/DiskLayerIF.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace dlna
{
//-------------------------------------------------------------------------------------------

class DiskMockIF : public DiskIF
{
	public:
		DiskMockIF();
		virtual ~DiskMockIF();

		MOCK_CONST_METHOD1(isFile,bool(const QString& name));
		MOCK_CONST_METHOD1(isDirectory,bool(const QString& name));
		
		MOCK_CONST_METHOD1(openDirectory,DiskIF::DirHandle(const QString& name));
        MOCK_CONST_METHOD1(nextDirectoryEntry,QString(DiskIF::DirHandle h));
		MOCK_CONST_METHOD1(closeDirectory,void(DiskIF::DirHandle h));
};

//-------------------------------------------------------------------------------------------
} // namespace dlna
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------