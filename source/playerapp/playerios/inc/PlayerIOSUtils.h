//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_PLAYERAPP_PLAYERIOS_PLAYERIOSUTILS_H
#define __OMEGA_PLAYERAPP_PLAYERIOS_PLAYERIOSUTILS_H
//-------------------------------------------------------------------------------------------

#include "common/inc/DiskOps.h"
#include <QStandardPaths>

//-------------------------------------------------------------------------------------------
namespace omega
{
//-------------------------------------------------------------------------------------------

class PlayerIOSUtils
{
	public:
		PlayerIOSUtils();
		
		static QString appDataDirectory();
		static QString musicDirectory();
		
	private:
		static void printError(const tchar *strR, const tchar *strE);
};

//-------------------------------------------------------------------------------------------
} // namespace omega
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
