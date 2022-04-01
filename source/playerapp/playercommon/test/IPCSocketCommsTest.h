//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_PLAYERCOMMON_IPCSOCKETCOMMSTEST_H
#define __OMEGA_PLAYERAPP_PLAYERCOMMON_IPCSOCKETCOMMSTEST_H
//-------------------------------------------------------------------------------------------

#include "network/inc/Resource.h"

#include "gtest/gtest.h"

#include <QCoreApplication>
#include <QtTest/QtTest>

#include "playerapp/playercommon/inc/IPCSocketComms.h"

//-------------------------------------------------------------------------------------------

class IPCSocketComms_QtTestClass : public QObject
{
	Q_OBJECT
	public:
		IPCSocketComms_QtTestClass(const QString& pathExec, const QString& socketPath, QObject *parent);
	
	private:
		QString m_pathToTestExec;
		QString m_socketPath;
		
		QString pathToTestProgramA();
	
	private slots:
		void sendAndReceiveFromClient();
};

//-------------------------------------------------------------------------------------------

class IPCSocketComms_QtTestApplication : public QCoreApplication
{
	Q_OBJECT
	public:
		IPCSocketComms_QtTestApplication(const QString& pathExec, const QString& socketPath, int argc,char **argv);
		bool testSucceeded() const;
	private:
		bool m_succeeded;
		QString m_pathToTestExec;
		QString m_socketPath;
	private slots:
		void runTests();
};

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
