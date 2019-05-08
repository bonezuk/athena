#include "dlna/inc/DiskIF.h"
#include "test/Test2Enviroment.h"
#include "common/inc/SBService.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace test
{
//-------------------------------------------------------------------------------------------

Test2Enviroment *Test2Enviroment::m_instance = 0;

//-------------------------------------------------------------------------------------------

Test2Enviroment::Test2Enviroment()
{}

//-------------------------------------------------------------------------------------------

Test2Enviroment::~Test2Enviroment()
{
	m_instance = 0;
}

//-------------------------------------------------------------------------------------------

Test2Enviroment *Test2Enviroment::instance()
{
	if(m_instance==0)
	{
		bool success = true;

		m_instance = new Test2Enviroment;
		
		QList<QPair<QString,QString> > blankList;
		if(!m_instance->setupDirectory(0,blankList))
		{
			success = false;
		}

		if(!success)
		{
			delete m_instance;
			m_instance = 0;
		}
	}
	return m_instance;
}

//-------------------------------------------------------------------------------------------

QString Test2Enviroment::root(int testCase)
{
	return getTempDirectoryPath(testCase);
}

//-------------------------------------------------------------------------------------------

QString Test2Enviroment::getTempDirectoryPath()
{
	return common::SBService::tempDirectory();
}

//-------------------------------------------------------------------------------------------

QString Test2Enviroment::getTempDirectoryPath(int testCase)
{
	QString path = getTempDirectoryPath() + "/" + "test2_" + QString::number(testCase);
	path = dlna::DiskIF::toNativeSeparators(path);
	return path;
}

//-------------------------------------------------------------------------------------------

bool Test2Enviroment::setupDirectory(int testCase,const QList<QPair<QString,QString> >& fileList)
{
	QString dPath = getTempDirectoryPath(testCase);
	bool res = true;
	
	if(common::DiskOps::path(dPath,true))
	{
		QList<QPair<QString,QString> >::const_iterator ppI;
		
		for(ppI=fileList.begin();ppI!=fileList.end() && res;ppI++)
		{
			const QPair<QString,QString>& p = *ppI;
			QString destPath = dPath + "/" + p.second;
			destPath = dlna::DiskIF::toNativeSeparators(destPath);
			if(common::DiskOps::path(destPath))
			{
				if(QFile::exists(destPath))
				{
					common::DiskOps::deleteFile(destPath);
				}
				QFile file(p.first);
                res = file.copy(destPath);
			}
			else
			{
				res = false;
			}
		}
	}
	else
	{
		res = false;
	}
	return res;
}

//-------------------------------------------------------------------------------------------

void Test2Enviroment::removeDirectory(int testCase)
{
	QString dPath = getTempDirectoryPath(testCase);
	common::DiskOps::deleteDirectory(dPath);
}

//-------------------------------------------------------------------------------------------
} // namespace test
} // namespace orcus
//-------------------------------------------------------------------------------------------
