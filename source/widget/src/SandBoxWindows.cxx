#include "widget/inc/SandBoxWindows.h"

//-------------------------------------------------------------------------------------------
#if defined(ORCUS_WIN32) || defined(OMEGA_LINUX)
//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace widget
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(common::SBServiceFactory,common::SBService, \
                            SBServiceWinFactory,SBServiceWin, \
                            "win",false)

//-------------------------------------------------------------------------------------------

SBServiceWin::SBServiceWin(QObject *parent) : SBServiceBase(parent)
{}

//-------------------------------------------------------------------------------------------

SBServiceWin::~SBServiceWin()
{}

//-------------------------------------------------------------------------------------------
} // namespace widget
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------