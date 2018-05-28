#include "player/inc/AutoUpdaterWin.h"

#if defined(OMEGA_WIN32)
#include "winsparkle.h"
#endif

//-------------------------------------------------------------------------------------------
#if defined(OMEGA_WIN32)
//-------------------------------------------------------------------------------------------
namespace orcus
{
namespace player
{
//-------------------------------------------------------------------------------------------

CONCRETE_FACTORY_CLASS_IMPL(AutoUpdaterFactory,AutoUpdater, \
                            AutoUpdaterWinFactory,AutoUpdaterWin, \
                            "win",false)

//-------------------------------------------------------------------------------------------

AutoUpdaterWin::AutoUpdaterWin() : AutoUpdater()
{
	m_url = "http://www.blackomega.co.uk/sparkle/appcast_win.xml";
	win_sparkle_set_appcast_url(m_url.toLatin1().constData());
	win_sparkle_init();
}

//-------------------------------------------------------------------------------------------

AutoUpdaterWin::~AutoUpdaterWin()
{
	win_sparkle_cleanup();
}

//-------------------------------------------------------------------------------------------

void AutoUpdaterWin::checkForUpdates()
{
	win_sparkle_check_update_with_ui();
}

//-------------------------------------------------------------------------------------------
} // namespace player
} // namespace orcus
//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
