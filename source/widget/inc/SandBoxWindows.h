//-------------------------------------------------------------------------------------------
#ifndef __ORCUS_WIDGET_SANDBOXWINDOWS_H
#define __ORCUS_WIDGET_SANDBOXWINDOWS_H
//-------------------------------------------------------------------------------------------

#include "widget/inc/SandBoxBase.h"

//-------------------------------------------------------------------------------------------
#if defined(OMEGA_WIN32) || defined(OMEGA_LINUX)
//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace widget
{
//-------------------------------------------------------------------------------------------

class WIDGET_EXPORT SBServiceWin : public SBServiceBase
{
	public:
		Q_OBJECT
		
	public:
		SBServiceWin(QObject *parent = 0);
		virtual ~SBServiceWin();
};

//-------------------------------------------------------------------------------------------
} // namespace widget
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------