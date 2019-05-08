//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_TEST_TEST2ENVIROMENT_H
#define __ORCUS_TEST_TEST2ENVIROMENT_H
//-------------------------------------------------------------------------------------------

#include <QtCore/QPair>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QList>

#include "common/inc/DiskOps.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace test
{
//-------------------------------------------------------------------------------------------
// Setup and provide a number web directories that provide test cases
// for testing the functionality of UPnP
//-------------------------------------------------------------------------------------------

class Test2Enviroment
{
	public:
		virtual ~Test2Enviroment();
		
		static Test2Enviroment *instance();
		
		QString root(int testCase);
		
	protected:
	
		static Test2Enviroment *m_instance;
	
		Test2Enviroment();
		QString getTempDirectoryPath();
		QString getTempDirectoryPath(int testCase);
		bool setupDirectory(int testCase,const QList<QPair<QString,QString> >& fileList);
		void removeDirectory(int testCase);
};

//-------------------------------------------------------------------------------------------
} // namespace test
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
