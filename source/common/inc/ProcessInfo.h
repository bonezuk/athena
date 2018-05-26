//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_COMMON_PROCESSINFO_H
#define __ORCUS_COMMON_PROCESSINFO_H
//-------------------------------------------------------------------------------------------

#include "common/inc/CommonTypes.h"
#include "common/inc/BOParse.h"

#include <QList>
#include <QString>
#include <QStringList>

//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace common
{
//-------------------------------------------------------------------------------------------

class COMMON_EXPORT ProcessInfo
{
	public:
		~ProcessInfo();
		
		static ProcessInfo& instance();
		
		bool getProcessNames(QStringList& nameList);
		
	protected:
		
		static ProcessInfo *m_instance;
		
#if defined(OMEGA_LINUX)
		BOParse m_sParse;
		tint m_sState[4];
#endif
		
		ProcessInfo();
		
		void printError(const tchar *strR,const tchar *strE) const;
		
#if defined(OMEGA_LINUX)
		bool parseProcessInfo(const QString& pidDir,QString& name);
#endif
};

//-------------------------------------------------------------------------------------------
} // namespace common
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------

