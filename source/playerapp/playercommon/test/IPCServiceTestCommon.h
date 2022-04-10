//-------------------------------------------------------------------------------------------
#ifndef __PLAYERAPP_PLAYERCOMMON_IPCSERVICETESTCOMMON_H
#define __PLAYERAPP_PLAYERCOMMON_IPCSERVICETESTCOMMON_H
//-------------------------------------------------------------------------------------------

#include "gtest/gtest.h"

#include <QCoreApplication>
#include <QtTest/QtTest>

#include "playerapp/playercommon/inc/IPCService.h"
#include "playerapp/playercommon/inc/IPCInterfaceBase.h"

//-------------------------------------------------------------------------------------------
namespace orcus
{
//-------------------------------------------------------------------------------------------

const int c_testIPCServiceCountLimit = 100000000;

#define IPC_SERVICE_TEST_NAME  "ipctestservice"
#define IPC_SERVICE_PROGB_NAME "ipcprogbservice"

//-------------------------------------------------------------------------------------------
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
